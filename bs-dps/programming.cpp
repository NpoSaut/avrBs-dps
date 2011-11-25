/*
 * programming.cpp
 *
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 12000000UL 	// 12 MHz

#include "communicator.h"
#include "programming.h"




class Programming
{
public:
	Programming ();

	void CommParser (uint8_t Addr);


private:
	Queue<uint8_t,16> ProgQueue;
	void ProgFeed ();
	bool ProgStop;

	enum TCommand 														// Взаимодействие с управляющим устройством
	{
		InProcess = 0x00,
		FlashRead = 0x11,
		FlashWrite = 0x12,
		EEPROMRead = 0x21,
		EEPROMWrite = 0x22,
		FUSERead = 0x31,
		FUSEWrite = 0x32,
		Clear = 0x03,
		Exit = 0x04,
	};
	TCommand Command;
	inline void Return (uint8_t RetVal) {Communicator::DataOut[0] = (Communicator::DataIn[0] & 0xff00) | RetVal;} // Выставляет признак готовности в старший байт по адресу "0" и результат в младший

	struct MemAddr {static uint32_t Start, End, Parser, Prog;};
	inline void MemAddrFromCom ();

	void (* ReadCompleteOld) (uint8_t);									// Действующая функция обработки приходящих сообщений
																		// заменяется в режиме программирования собственным парсером
};

Programming::Programming ()
{
	ProgSPI::Init (ProgFeed);
	ReadCompleteOld = Communicator::ReadComplete;
	Communicator::ReadComplete = &Programming::CommParser;
}

void Programming::CommParser (uint8_t Addr)
{
	namespace Comm = Communicator;
	if (Addr == 0)
	{
		if (Comm::DataIn[0] == 0x0efe) 									// Входа в режим программирования
		{
			uint8_t success = 0;
			for (uint8_t i = 0; i < 3; i++)
			{
				ProgSPI::RebootInProg ();
				success = ProgSPI::InstructionEnableProg();

				if (success)
					break;
			}
			Return (success ? 0x0 : 0x20);
		}
		else
			switch (TCommand (Comm::DataIn[0] & 0x00ff))
			{
			case InProcess:
				switch (Command)
				{
				case FlashWrite:
					uint8_t BytesNumber = (MemAddr::End - MemAddr::Parser > 6) ? 6 : MemAddr::End - MemAddr::Parser;
					for (uint8_t i = 0; i < BytesNumber; i ++)
						ProgQueue.Push ( i%2 == 0 ? Comm::DataIn[1+i/2] >> 8 : Comm::DataIn[1+i/2] );

					MemAddr::Parser += BytesNumber;
					if (MemAddr::Parser == MemAddr::End)
					{
						ProgStop = true;
						ProgQueue.WaitEmpty ();
						Return (0);
					}
					else
						Return (0x02);
					break;

				case FlashRead:
					for (uint8_t i = 0; i < 6; i++)
					{
						if (ProgQueue.IsEmpty() && ProgStop)
						{
							Return (0);
							break;
						}
						else
							Comm::DataOut[1+i/2] |= (i%2 == 0) ? uint16_t (ProgQueue.Pop()) << 8 : ProgQueue.Pop();
					}
					if (!(ProgQueue.IsEmpty() && ProgStop))
						Return (1);
					break;
				}
				break;

			case FlashWrite:
				MemAddrFromCom ();
				Command = FlashWrite;
				ProgStop = false;
				ProgFeed ();
				goto InProcess;
				break;

			case FlashRead:
				MemAddrFromCom ();
				Command = FlashRead;
				ProgStop = false;
				ProgSPI::InstructionImProgmemRead(MemAddr::Prog);			// После чтения уйдём в прерывание, которое будет наполнять очередь
				goto InProcess;
				break;

			case EEPROMRead:

				break;


			case Clear:
				ProgSPI::InstructionErase ();
				Return (0);
				break;

			case Exit:
				ProgSPI::RebootInWork ();
				Comm::ReadComplete = ReadCompleteOld;// May be reboot myself is realy beter way>
				break;
			}
	}// Addr = 0
}

void Programming::ProgFeed ()
{
	switch (Command)
	{
	case FlashWrite:
		if (uint8_t (MemAddr::Prog) == 0x0) 						//  Если  закончилась страница
			ProgSPI::InstructionProgmemWritePage(MemAddr::Prog >> 8);

		if (ProgQueue.IsEmpty() && ProgStop)						// Если очередь исчерпана и больше элементов не ожидается
			ProgSPI::InstructionProgmemWritePage(MemAddr::Prog >> 8);
		else
		{
			ProgSPI::InstructionImProgmemLoadToPage(MemAddr::Prog, ProgQueue.Pop());
			MemAddr::Prog ++;
		}
		break;

	case FlashRead:
		ProgQueue.Push(ProgSPI::AnswerFlush());
		MemAddr::Prog ++;

		if (MemAddr::Prog != MemAddr::End)
			ProgSPI::InstructionImProgmemRead(MemAddr::Prog);
		else
			ProgStop = true;
		break;


	}
}

inline void Programming::MemAddrFromCom ()
{
	using namespace Communicator;
	MemAddr::Start = (DataIn[2] & 0xff00) | (DataIn[1] & 0x00ff);
	MemAddr::Start = (MemAddr::Start << 8) | (DataIn[1] >> 8);
	MemAddr::End  = uint32_t (DataIn[3] & 0x00ff) << 16;
	MemAddr::End |= (DataIn[3] & 0xff00) | (DataIn[2] & 0x00ff);

	MemAddr::Parser = MemAddr::Start;
	MemAddr::Prog = MemAddr::Start;
}



