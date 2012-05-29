/*
 * programming.h
 *
 *  Программирование соседней МК через лииню связи (Communicator)
 *  *************************************************************
 *
 *  - Обеспечивает интерфейс между управляющим устройствои и МК соседнего полукомплекта, по средствам которого возможно прошивание МК
 *  - Функция Parser должна вызываться после прихода каждого пакета. -- интерфейс с линией связи (модуль Communicator)
 *    Режим взаимодействия:
 *    0. Общие положения
 *       - Внутренний адрес "0" используется для передачи комманд, остальные (1,2,3) - для данных
 *       - В младшем байте команды содержится сама команда (упр. устройство -> модуль) или код возврата (модуль -> упр. устройство)
 *       - Старший байт команды служит флагом того, была ли комманда обработана и получен результат:
 *         в знак готовности модуль выставляет последний полученный старший байт на адрес "0"
 *    1. Вход в режим программирования:
 *       - на внутренний адрес "0" ожидается 0x0efe
 *       - в случае успеха возвращает 0x0, в случа неудачи 0x20
 *    2. Команды в режиме программирования (в скобках код команды):
 *       2.1 Чтение Флеш от адреса X до адреса Y (0x11)
 *       	 - Адресуются байты
 *           - До передачи команды адреса X и Y должны быть переданы модулю в следующем виде (3 байта на каждый адрес, нумерация от мл. к ст.):
 *             X: 1 байт - старший байт на адрес "1";
 *                2 байт - младший байт на адрес "1";
 *                3 байт - старший байт на адрес "2".
 *             Y: 1 байт - младший байт на адрес "2";
 *                2 байт - старший байт на адрес "3";
 *                3 байт - младший байт на адрес "3".
 *          - Данные читаются от X до Y группами по 3 слова. Каждая группа отправляется в следующем порядке:
 *            ст.б. адреса "1" -> мл.б. ардеса "1" -> ст.б. ардеса "2" -> мл.б. ардеса "2" -> ст.б. ардеса "3" -> мл.б. ардеса "3"
 *          - По занесению группы данных в буфер отправки модуль по адресу "0" старшим байтом выставляет готовность
 *            и ожидает команды 0x0 для подготовки следующей группы
 *          - После отправки последней группы модуль возвращает 0x0 и выставляет готовность
 *
 *		FlashRead = 0x11,
		FlashWrite = 0x12,
		EEPROMRead = 0x21,
		EEPROMWrite = 0x22,
		FUSERead = 0x31,
		FUSEWrite = 0x32,
		Clear = 0x03,
		Exit = 0x04
 *
 */

#ifndef PROGRAMMING_H_
#define PROGRAMMING_H_


#include <avr/io.h>
#include <util/delay.h>
#include <util/crc16.h>
#include <cpp/universal.h>
#include <cpp/io.h>
#include <cpp/dispatcher.h>

#include "ProgSpi.h"
#include "SautDat.h"
#include "CanDat.h"
#include "CanDesriptors.h"


template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
class ProgrammingCan
{
public:
	ProgrammingCan (Delegate<void ()> activateSystem, Delegate<void ()> disactivateSystem);

	void getCommand (uint16_t parametersArrayPointer);

	void getData (uint16_t dataArrayPointer);

	void sendData (uint16_t);

private:
	typedef const uint8_t Data[8];
	typedef ProgSpi::
	ProgSpiSimple  < &Register::spiStatusControl, &Register::spiData,
	 	 	 	 	 &Register::portB, 0, 1, 2, 3,
	 	 	 	 	 &Register::portB, 4
					>
	Controller;
	Controller controller;

	enum Action : uint8_t
	{
		write = 0,
		read = 1,
		erase = 2,
		reboot_neighbour = 3,
		ping = 4
	};
	enum Status : uint8_t
	{
		OK = 0,
		UNKNOWN_ERROR = 1,
		SIZE_LIMIT = 2,
		OUT_OF_RANGE = 3,
		CRC_ERROR = 4,
		UNKNOWN_MEMORY = 5,
		UNRECOGNIZED_DATA = 6,
		UNKNOWN_COMMAND = 7
	};
	enum MemoryType : uint8_t
	{
		flash = 0,
		eeprom = 1,
		fuse = 2
	};

	struct CommandMap
	{
		MemoryType memoryType	:3;
		Action action			:3;
		uint8_t init			:1;
		uint8_t					:1;
	};
	typedef Bitfield<CommandMap> Command;
	struct AnswerMap
	{
		Status	status			:3;
		Action	action			:3;
		uint8_t 				:1; // дл ответа этот флаг = 0
		uint8_t					:1; // резерв
	};
	typedef Bitfield<AnswerMap> Answer;

	static constexpr uint16_t flashPageSize = 128 * 2;
	static constexpr uint32_t flashSize = (uint32_t)512 * flashPageSize;
	static constexpr uint8_t  eepromPageSize = 8;
	static constexpr uint16_t eepromSize = (uint16_t)512 * eepromPageSize;

	void readDataToBuffer (uint8_t (&data) [8]);
	bool prepareController ();

	uint16_t counter; // обратный отсчёт байт до конца сеанса
	uint8_t txData[8]; // отправляемые данные
	Complex<uint16_t> crc;
	uint16_t wantedCrc;
	ProgSpi::MemType memType;
	enum Active : uint8_t // Активность сессии в данный момент
	{
		nothing = 0,
		writing = 1,
		reading = 2
	} active;
	Delegate<void ()> activateSystem, disactivateSystem;
};

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::ProgrammingCan (Delegate<void ()> activateSystem, Delegate<void ()> disactivateSystem)
	: controller (), counter (0), crc (0), wantedCrc (0), active (Active::nothing), activateSystem (activateSystem), disactivateSystem (disactivateSystem)
{
}

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
void ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::getCommand (uint16_t parametersArrayPointer)
{
	Data& data = *( (Data *)(parametersArrayPointer) );

	Command& command = _cast(Command, data[0]);

	if ( command.init )
	{
		active = Active::nothing; // Прерываем текущий сеанс
		activateSystem ();
		switch (command.action)
		{
			case Action::read:
			case Action::write:
				if (command.memoryType == MemoryType::fuse)
				{

				}
				else
				{
					if ( command.memoryType == MemoryType::flash )
						memType = ProgSpi::MemType::flash;
					else if ( command.memoryType == MemoryType::eeprom )
						memType = ProgSpi::MemType::eeprom;
					else
					{
						while ( !canDat.template send<controlDescriptor> ({ Answer({Status::UNKNOWN_MEMORY, command.action,}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
						break;
					}

					uint16_t& size = _cast (uint16_t, data[1]);
					uint32_t startAdr = Complex<uint32_t> {data[3], data[4], data[5], 0};
					uint16_t& getCrc = _cast (uint16_t, data[6]);

					// Проверки
					if ( size > 256 )
					{
						while ( !canDat.template send<controlDescriptor> ({ Answer({Status::SIZE_LIMIT, command.action}), 0, 1 ,0 ,0 ,0 ,0 ,0 }) );
						break;
					}

					if ( memType == ProgSpi::MemType::flash )
					{
						if ( startAdr + (uint32_t)size > flashSize )
						{
							while ( !canDat.template send<controlDescriptor> ({Answer({Status::OUT_OF_RANGE, command.action}),
										uint8_t(flashSize), uint8_t(flashSize/256), uint8_t(flashSize/256/256), uint8_t(flashSize/256/256/256), 0, 0, 0 }) );
							break;
						}
					}
					else
					{
						if ( startAdr + (uint32_t)size > eepromSize )
						{
							while ( !canDat.template send<controlDescriptor> ({Answer({Status::OUT_OF_RANGE, command.action}),
										uint8_t(eepromSize), uint8_t(eepromSize/256), 0, 0, 0, 0, 0 }) );
							break;
						}
					}

					if ( prepareController() )
					{
						controller.position = startAdr;
						counter = size;
						crc = 0;
						wantedCrc = getCrc;

						disactivateSystem ();

						if ( command.action == Action::read )
						{
							active = Active::reading;
							readDataToBuffer (txData);
							sendData (0);
						}
						else
							active = Active::writing;
					}
					else
						while ( !canDat.template send<controlDescriptor> ({ Answer({Status::UNKNOWN_ERROR, command.action}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				}
				break;

			case Action::erase:
				if ( prepareController() )
				{
					controller.erase();
					while ( !canDat.template send<controlDescriptor> ({ Answer({Status::OK, Action::erase}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				}
				else
					while ( !canDat.template send<controlDescriptor> ({ Answer({Status::UNKNOWN_ERROR, Action::erase}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				break;

			case Action::reboot_neighbour:
				if ( controller.isInProgState() )
					controller.flush<ProgSpi::MemType::flash>();
					controller.flush<ProgSpi::MemType::eeprom>();
				controller.rebootInWork();
				while ( !canDat.template send<controlDescriptor> ({ Answer({Status::OK, Action::reboot_neighbour}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				break;

			case Action::ping:
				while ( !canDat.template send<controlDescriptor> ({ Answer({Status::OK, Action::ping}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				break;

			default:
				while ( !canDat.template send<controlDescriptor> ({ Answer({Status::UNKNOWN_COMMAND, command.action}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				break;
		}
	}
}

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
void ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::getData (uint16_t dataArrayPointer)
{
	Data& data = *( (Data *)(dataArrayPointer) );

	if ( active == Active::writing )
	{
		for (const uint8_t& d : data)
		{
			if ( memType == ProgSpi::MemType::flash )
				controller.write<ProgSpi::MemType::flash>(d);
			else
				controller.write<ProgSpi::MemType::eeprom>(d);

			crc = _crc_ccitt_update(crc, d);

			if ( --counter == 0 ) // Конец сеанса
			{
				active = Active::nothing;
				activateSystem ();
				if (wantedCrc == crc)
				{
					if ( memType == ProgSpi::MemType::flash )
						controller.flush<ProgSpi::MemType::flash>();
					else
						controller.flush<ProgSpi::MemType::eeprom>();
					while ( !canDat.template send<controlDescriptor> ({ Answer({Status::OK, Action::write}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
				}
				else
					while ( !canDat.template send<controlDescriptor> ({ Answer({Status::CRC_ERROR, Action::write}), crc[0], crc[1] ,0 ,0 ,0 ,0 ,0 }) );
				break;
			}
		}
	}
	else
		while ( !canDat.template send<controlDescriptor> ({ Answer({Status::UNRECOGNIZED_DATA, Action::write}), 0, 0 ,0 ,0 ,0 ,0 ,0 }) );
}

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
void ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::sendData(uint16_t)
{
	if ( active == Active::reading )
	{
		while ( !canDat.template send<dataDescriptor> (txData) );
		if ( counter <= 8 ) // Последняя посылка
		{
			counter = 0;
			active = Active::nothing;
			activateSystem ();
			while ( !canDat.template send<controlDescriptor> ({Answer({Status::OK, Action::read}), crc[0], crc[1], 0, 0, 0, 0, 0}) );
		}
		else
		{
			counter -= 8;
			readDataToBuffer (txData);
		}
	}
}

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
void ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::readDataToBuffer(uint8_t (&data) [8])
{
		for (uint8_t i = 0; i < 8; i++)
		{
			if ( counter > i )
			{
				data[i] = ( memType == ProgSpi::MemType::flash ) ?
							controller.read<ProgSpi::MemType::flash>() :
							controller.read<ProgSpi::MemType::eeprom>();
				crc = _crc_ccitt_update(crc, data[i]);
			}
			else
				data[i] = 0;
		}
}

template <  typename CanDat, CanDat& canDat,
			uint16_t controlDescriptor, uint16_t dataDescriptor >
bool ProgrammingCan<CanDat, canDat, controlDescriptor, dataDescriptor>::prepareController ()
{
	if ( controller.isInProgState() )
		return true;
	else
		return controller.rebootInProg();
}


class Programming
{
public:
	Programming (	Safe<uint16_t>& inPort0_,	Safe<uint16_t>& inPort1_,	Safe<uint16_t>& inPort2_,	Safe<uint16_t>& inPort3_,
				 	Safe<uint16_t>& outPort0_,	Safe<uint16_t>& outPort1_,	Safe<uint16_t>& outPort2_,	Safe<uint16_t>& outPort3_ )
		: neighbour (),
		  inPort0(inPort0_),	inPort1(inPort1_),		inPort2(inPort2_),		inPort3(inPort3_),
		  outPort0(outPort0_),	outPort1(outPort1_),	outPort2(outPort2_),	outPort3(outPort3_)
	{}

	void enterProgramMode ()
	{
		Return( neighbour.rebootInProg() ? 0x0 : 0x20 );
	}
	void comParser ();

private:
	ProgSpi::
	ProgSpiSimple  < &Register::spiStatusControl, &Register::spiData,
	 	 	 	 	 &Register::portB, 0, 1, 2, 3,
	 	 	 	 	 &Register::portB, 4
					> neighbour;

	Safe<uint16_t>& inPort0;
	Safe<uint16_t>& inPort1;
	Safe<uint16_t>& inPort2;
	Safe<uint16_t>& inPort3;
	Safe<uint16_t>& outPort0;
	Safe<uint16_t>& outPort1;
	Safe<uint16_t>& outPort2;
	Safe<uint16_t>& outPort3;

	enum Command 														// Взаимодействие с управляющим устройством
	{
		InProcess = 0x00,
		FlashRead = 0x11,
		FlashWrite = 0x12,
		FlashErase = 0x13,
		FlashExit = 0x14,
		EEPROMRead = 0x21,
		EEPROMWrite = 0x22,
		EEPROMErase = 0x23,
		EEPROMExit = 0x24,
		FUSERead = 0x31,
		FUSEWrite = 0x32,
		FUSEErase = 0x33,
		FUSEExit = 0x34,
		Erase = 0x03,
		Exit = 0x04
	};
	Command command;

	void Return (uint8_t res) // Выставляет признак готовности в старший байт по адресу "0" и результат в младший
	{
		outPort0 <<= Complex<uint16_t>{ uint8_t(res), uint8_t(inPort0/256) };
	}

	uint32_t memAreaEnd;

	// Преобразует полученныые адресные границы к нормальному виду и запоминает их
	void memAreaSet ()
	{
		Complex<uint16_t> get1 = +inPort1;
		Complex<uint16_t> get2 = +inPort2;
		Complex<uint16_t> get3 = +inPort3;

		neighbour.position = Complex<uint32_t> {get1[1], get1[0], get2[1], 0};
		memAreaEnd = Complex<uint32_t> {get2[0], get3[1], get3[0], 0};
	}
};

//volatile uint8_t counter;


void Programming::comParser ()
{
	using namespace ProgSpi;
	using namespace Saut;

	switch (Command (inPort0 & 0x00ff))
	{
	case Erase:
	case FUSEErase:
	case EEPROMErase:
	case FlashErase:
		neighbour.erase ();
		Return (3);
		Return (0);
		break;

	case EEPROMExit:
		neighbour.flush<ProgSpi::eeprom> ();
	case FlashExit:
		neighbour.flush<ProgSpi::flash> ();
	case Exit:
	case FUSEExit:
		Return (4);
		neighbour.~ProgSpiSimple ();
		reboot();
		break;

	case FlashWrite:
		command = FlashWrite;
		memAreaSet ();
		Return (0x02);
		break;

	case FlashRead:
		command = FlashRead;
		memAreaSet ();
		Return (0x01);
		break;

	case EEPROMWrite:
		command = EEPROMWrite;
		memAreaSet ();
		Return (0x02);
		break;

	case EEPROMRead:
		command = EEPROMRead;
		memAreaSet ();
		Return (0x01);
		break;

	case FUSEWrite:
		command = FUSEWrite;
		Return (0x02);
		break;

	case FUSERead:
		command = FUSERead;
		Return (0x01);
		break;

	case InProcess:
		switch (command)
		{
		case FUSEWrite:
//			Complex<uint16_t> in1 = +inPort1;
//			Complex<uint16_t> in3 = +inPort3;
//
//			neighbour.fuseWriteLow( in1[1] );
//			neighbour.fuseWriteHigh( in1[0] );
//			neighbour.fuseWriteExt ( in3[1] );

			Return (0);
			break;

		case FUSERead:
			outPort1 <<= Complex<uint16_t> { neighbour.fuseReadHigh(), neighbour.fuseReadLow() };
			outPort2 <<= Complex<uint16_t> { 0						 , neighbour.fuseReadExt() };
			Return (0);
			break;

		case FlashWrite:
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::flash> (swap(+inPort1));
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::flash> (swap(+inPort2));
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::flash> (swap(+inPort3));

			if ( neighbour.position < memAreaEnd )
				Return (2);
			else
			{
				neighbour.flush<ProgSpi::flash> ();
				Return (0);
			}
			break;

		case EEPROMWrite:
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::eeprom> (swap(+inPort1));
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::eeprom> (swap(+inPort2));
			if ( neighbour.position < memAreaEnd )
				neighbour.write<ProgSpi::eeprom> (swap(+inPort3));

			if ( neighbour.position < memAreaEnd )
				Return (2);
			else
			{
				neighbour.flush<ProgSpi::eeprom> ();
				Return (0);
			}
			break;

		case FlashRead:
			outPort1 <<= swap( neighbour.readWord<ProgSpi::flash>() );
			outPort2 <<= swap( neighbour.readWord<ProgSpi::flash>() );
			outPort3 <<= swap( neighbour.readWord<ProgSpi::flash>() );
			if ( neighbour.position <= memAreaEnd )
				Return (1);
			else
				Return (0);
			break;

		case EEPROMRead:
			outPort1 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
			outPort2 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
			outPort3 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
			if ( neighbour.position <= memAreaEnd )
				Return (1);
			else
				Return (0);
			break;
		}
		break;
	}
}

// До лучших времён: ->
//	enum Operation : uint8_t
//	{
//		InProcess 	= 0,
//		Complete	= 0,
//		Read		= 1,
//		Write		= 2,
//		Erase		= 3,
//		Exit		= 4
//	};
//	enum Memory : uint8_t
//	{
//		Unknown 	= 0,
//		Flash		= 1,
//		Eeprom		= 2,
//		Fuse		= 3,
//	};
//	struct Command
//	{
//		Operation	operation	:4;
//		Memory		memory		:4;
//	};
//	Operation operationCurrent;
//	uint8_t confirmCode;
//
//
//Complex<uint16_t> get = inPort0;
//Bitfield<Command> commandGet;
//commandGet = get[0];
//confirmCode = get[1];
//
//if (commandGet.operation == Operation::InProcess)
//{
//	if (commandGet.memory == Memory::Fuse)
//	{
//		if (operationCurrent == Operation::Write)
//		{
////				Complex<uint16_t> in1 = +inPort1;
////				Complex<uint16_t> in3 = +inPort3;
////
////				neighbour.fuseWriteLow( in1[1] );
////				neighbour.fuseWriteHigh( in1[0] );
////				neighbour.fuseWriteExt ( in3[1] );
//////				neighbour.fuseWriteLow (uint8_t (+inPort1 >> 8));
//////				neighbour.fuseWriteHigh (+inPort1);
//////				neighbour.fuseWriteExt (+inPort3 >> 8);
//			Return (Operation::Complete);
//		}
//		else
//		{
//			outPort1 <<= Complex<uint16_t> { neighbour.fuseReadHigh(), neighbour.fuseReadLow() };
//			outPort2 <<= Complex<uint16_t> { 0						 , neighbour.fuseReadExt() };
//			Return (Operation::Complete);
//		}
//	}
//	if (commandGet.memory == Memory::Eeprom)
//	{
//		if (operationCurrent == Operation::Write)
//		{
//			neighbour.write<ProgSpi::eeprom> (swap(+inPort1));
//			neighbour.write<ProgSpi::eeprom> (swap(+inPort2));
//			neighbour.write<ProgSpi::eeprom> (swap(+inPort3));
//			if ( neighbour.position < memAreaEnd )
//				Return (Operation::Write);
//			else
//				neighbour.flush<ProgSpi::eeprom> ();
//				Return (Operation::Complete);
//		}
//		else
//		{
//			outPort1 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
//			outPort2 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
//			outPort3 <<= swap( neighbour.readWord<ProgSpi::eeprom>() );
//			if ( neighbour.position <= memAreaEnd )
//				Return (Operation::Read);
//			else
//				Return (Operation::Complete);
//		}
//	}
//	if (commandGet.memory == Memory::Flash)
//	{
//		if (operationCurrent == Operation::Write)
//		{
//			neighbour.write<ProgSpi::flash> (swap(+inPort1));
//			neighbour.write<ProgSpi::flash> (swap(+inPort2));
//			neighbour.write<ProgSpi::flash> (swap(+inPort3));
//			if ( neighbour.position < memAreaEnd )
//				Return (Operation::Write);
//			else
//				neighbour.flush<ProgSpi::flash> ();
//				Return (Operation::Complete);
//		}
//		else
//		{
//			outPort1 <<= swap( neighbour.readWord<ProgSpi::flash>() );
//			outPort2 <<= swap( neighbour.readWord<ProgSpi::flash>() );
//			outPort3 <<= swap( neighbour.readWord<ProgSpi::flash>() );
//			if ( neighbour.position <= memAreaEnd )
//				Return (Operation::Read);
//			else
//				Return (Operation::Complete);
//		}
//	}
//}
//else
//{
//	operationCurrent = commandGet.operation;
//	if (commandGet.operation == Operation::Write || commandGet.operation == Operation::Read)
//	{
//		if (commandGet.memory != Memory::Fuse)
//			memAreaSet();
//		Return (operationCurrent);
//	}
//	else if (commandGet.operation == Operation::Erase)
//	{
//		Return (Operation::Erase);
//		neighbour.erase ();
//		Return (Operation::Complete);
//	}
//	else if (commandGet.operation == Operation::Exit)
//	{
//		if (commandGet.memory == Memory::Eeprom)
//			neighbour.flush<ProgSpi::eeprom> ();
//		if (commandGet.memory == Memory::Flash)
//			neighbour.flush<ProgSpi::flash> ();
//
//		Return (Operation::Exit);
//		neighbour.~ProgSpiSimple ();
//		cli ();
//		do															// Перезагружаем самих себя
//		{
//			wdt_enable(WDTO_15MS);
//			for (;;) { asm volatile ("nop"); }
//		}
//		while (0);
//	}
//}



#endif /* PROGRAMMING_H_ */
