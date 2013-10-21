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
			uint16_t initDescriptor, uint16_t answerDescriptor >
class ProgrammingCan
{
public:
	ProgrammingCan ();

	uint16_t getVersion () const { return version; }
	uint8_t getSubversion () const { return subversion; }
	uint16_t getCheckSum () const { return checkSum; }

	uint8_t getSystemId () const { return systemId; }
	uint16_t getCellId () const { return cellId; }
	uint32_t getCellManufactureNumber () const { return cellManufactureNumber; }
	uint16_t getCellManufactureYear () const { return cellManufactureYear; }
	uint8_t getCellManufactureMonth () const { return cellManufactureMonth; }
	uint8_t getChannel () const { return channel; }
	uint8_t getCellModification () const { return cellModification; }

private:
	typedef int32_t (*GetPropertyFunction) (uint8_t key);
	const GetPropertyFunction getPropertyPointer;

	uint16_t version;
	uint8_t subversion;
	uint16_t checkSum;

	uint8_t systemId;
	uint16_t cellId;
	int32_t cellManufactureNumber;
	uint16_t cellManufactureYear;
	uint8_t cellManufactureMonth;
	uint8_t channel;
	uint8_t cellModification;

	void catchInitProgMessage (uint16_t addr);
	uint32_t getProperty (uint8_t key) const __attribute__((noinline));
};

template <  typename CanDat, CanDat& canDat,
			uint16_t initDescriptor, uint16_t answerDescriptor >
ProgrammingCan<CanDat, canDat, initDescriptor, answerDescriptor>::ProgrammingCan ()
	: getPropertyPointer ( *((GetPropertyFunction *) &reg.general1) )
{
	version = getProperty (1);
	subversion = getProperty (2);
	checkSum = getProperty (6);

	systemId = getProperty (128);
	cellId = getProperty (129);

	cellManufactureNumber = getProperty (131);
//	canDat.template send<answerDescriptor> ({ (uint8_t)(cellManufactureNumber >> 24),
//										      (uint8_t)(cellManufactureNumber >> 16),
//											  (uint8_t)(cellManufactureNumber >> 8),
//											  (uint8_t)(cellManufactureNumber),
//											   0, 0, 0, 0});

	cellManufactureYear = getProperty (132)/100;
	cellManufactureMonth = getProperty (132)%100;
	channel = getProperty (133);
	cellModification = getProperty (134);

	canDat.template rxHandler<initDescriptor>() =
			SoftIntHandler::from_method <ProgrammingCan, &ProgrammingCan::catchInitProgMessage> (this);
}

template <  typename CanDat, CanDat& canDat,
			uint16_t initDescriptor, uint16_t answerDescriptor >
uint32_t ProgrammingCan<CanDat, canDat, initDescriptor, answerDescriptor>::getProperty(uint8_t key) const
{
	union
	{
		struct
		{
			uint16_t low;
			uint16_t hi;
		};
		int32_t full;
	} res;

	asm (
			"cli"							"\n\t"
			"mov r16, %[key]"				"\n\t"
			"movw r30, %[addr]"				"\n\t"
			"icall"							"\n\t"
			"movw %[resL], r16"				"\n\t"
			"movw %[resH], r18"				"\n\t"
			"sei"
				: [resL] "=r" (res.low),
				  [resH] "=r" (res.hi)
				: [key] "r" (key),
				  [addr] "e" (getPropertyPointer)
				: "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
				  "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25"//, "r26", "r27"//, "r28", "r29"//, "r30", "r31"
		);

	return res.full;

}

template <  typename CanDat, CanDat& canDat,
			uint16_t initDescriptor, uint16_t answerDescriptor >
void ProgrammingCan<CanDat, canDat, initDescriptor, answerDescriptor>::catchInitProgMessage(uint16_t addr)
{
	union InitMessage
	{
	private:
		struct
		{
			uint16_t key					:16;
			uint16_t cellIdHiX4				:8;
			uint16_t modification			:4;
			uint16_t cellIdLow				:4;
			uint16_t program				:8;
			uint16_t manufactureNumberHiX16	:4;
			uint16_t channel				:4;
			uint16_t manufactureNumberMid	:8;
			uint16_t manufactureNumberLow	:8;
		};

	public:
		InitMessage () { key = 0x0107; }

		uint16_t getCellId () const { return (cellIdHiX4 << 4) + cellIdLow; }
		uint8_t getModificationNumber () const { return modification; }
		uint8_t getProgramNumber () const { return program;  }
		uint8_t getChannelNumber () const { return channel; }
		uint32_t getManufactureNumber () const { return ((uint32_t)manufactureNumberHiX16 << 16)
														+ (manufactureNumberMid << 8)
														+ manufactureNumberLow; }

		void setCellId (uint16_t id) { cellIdLow = id; cellIdHiX4 = id/16; }
		void setModificationNumber (uint8_t num) { modification = num; }
		void setProgramNumber (uint8_t num) { program = num; }
		void setChannelNumber (uint8_t num) { channel = num; }
		void setManufactureNumber (uint32_t num) {  manufactureNumberLow = num;
													manufactureNumberMid = num >> 8;
													manufactureNumberHiX16 = num>>16; }

		struct
		{
			uint8_t rawData[8];
		};
	};
	InitMessage &message = *((InitMessage *) addr);

	if (    ( message.getCellId()				== getCellId()					|| message.getCellId() 		 		== 0 )
		 && ( message.getModificationNumber()   == getCellModification()   		|| message.getModificationNumber()  == 0 )
		 && ( message.getProgramNumber()  		== 1             				|| message.getProgramNumber()  		== 0 )
		 && ( message.getChannelNumber()  		== getChannel()  				|| message.getChannelNumber()  		== 0 )
		 && ( message.getManufactureNumber() 	== getCellManufactureNumber()   || message.getManufactureNumber() 	== 0 )
	   )
	{
		if ( // Точно совпадает всё
				( message.getCellId()				== getCellId()					 )
			 && ( message.getModificationNumber()   == getCellModification()   		 )
			 && ( message.getProgramNumber()  		== 1             				 )
			 && ( message.getChannelNumber()  		== getChannel()  				 )
			 && ( message.getManufactureNumber() 	== getCellManufactureNumber()   )
			)
		{
			reboot();
		}
		else
		{
			message.rawData[1] = 0x14;
			message.setCellId( getCellId() );
			message.setModificationNumber( getCellModification() );
			message.setProgramNumber( 1 );
			message.setChannelNumber( getChannel() );
			message.setManufactureNumber( getCellManufactureNumber() );

			canDat.template send <answerDescriptor> (message.rawData);
		}
	}
	return;
}



// ----------------------------------------- Программирование по Линии Связи САУТ/RS-485 ----------------------------------

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
		neighbour.rebootInWork();
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
