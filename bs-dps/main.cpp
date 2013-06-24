/*
 * main.cpp
 *
 *  БС-ДПС -- программа подсчёта скорости и ускорения
 *  *************************************************
 *
 *  - Описание функциональности программы
 *
 */

#include <avr/eeprom.h>
#include <avr/pgmspace.h>

//#include <cpp/interrupt-dynamic.h>
#include <cpp/io.h>
#include <cpp/universal.h>
#include <cpp/dispatcher.h>
#include <cpp/scheduler.h>
#include <cpp/timers.h>
#include <util/delay.h>

#include "hw_defines.h"
#include "SautCom.h"
#include "SautDat.h"
#include "id.h"
#include "programming.h"
#include "dps.h"
#include "eeprom.h"
#include "CanDat.h"
#include "CanDesriptors.h"
#include "kpt.h"


void Init (void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void Init (void)
{
	// Светодиоды.
	reg.portC.pin4.out();
	reg.portC.pin5.out();
	reg.portD.pin7.out();
	reg.portC.pin4 = true;
	reg.portC.pin5 = true;
	reg.portD.pin7 = true;

	// Watchdog
	wdt_enable (WDTO_500MS);

	// Кнопка сброса
	reg.portB.pin5.in();
}

// ----------------------------------- Системные часы и планировщик -----------------------------►

typedef Clock< Alarm< Alarm3A, 1000 >, uint32_t > ClockType;  // Обнуление через 50 суток
ClockType clock;

typedef Scheduler< ClockType, clock, 16, uint16_t > SchedulerType;
SchedulerType scheduler;

// ---------------------------------------------- CAN -------------------------------------------►

typedef INT_TYPELIST_2	(CanTx::SAUT_INFO_A,			CanTx::SAUT_INFO_B) 		 SAUT_INFO;
typedef INT_TYPELIST_2	(CanRx::MLSB_SAUT_KPT_A,		CanRx::MLSB_SAUT_KPT_B)		 MLSB_SAUT_KPT;
typedef INT_TYPELIST_2	(CanRx::EKS_KLUB_MCO_STATE_A,	CanRx::EKS_KLUB_MCO_STATE_B) EKS_KLUB_MCO_STATE;
typedef INT_TYPELIST_2	(CanRx::VDS_STATE_A,			CanRx::VDS_STATE_B)			 VDS_STATE;


typedef CanDat < LOKI_TYPELIST_1(					// Список дескрипторов для отправки
						SAUT_INFO
								),
				 LOKI_TYPELIST_3(					// Список дескрипторов для приёма
						 MLSB_SAUT_KPT,
						 EKS_KLUB_MCO_STATE,
						 VDS_STATE
						 	 	 ),
				 LOKI_TYPELIST_6(					// Прерывания при приёме
						 Int2Type< CanRx::MLSB_SAUT_KPT_A >,
						 Int2Type< CanRx::MLSB_SAUT_KPT_B >,
						 Int2Type< CanRx::EKS_KLUB_MCO_STATE_A >,
						 Int2Type< CanRx::EKS_KLUB_MCO_STATE_B >,
						 Int2Type< CanRx::VDS_STATE_A >,
						 Int2Type< CanRx::VDS_STATE_B >
								),
					128,
					NullType,						// Прерывания при отправке
				 250 >								// BaudRate = 250 Кбит, SamplePoint = 75% (по умолчанию)
	CanDatType;
CanDatType canDat;

// -------------------------------------------- RS-485 ------------------------------------------►

using namespace Saut;

enum {		   					// adr, intput, port
	// Входящие
	DpsCommand 			= SautPacketHead (1, true, 0),
	Dps0 				= SautPacketHead (1, true, 0),
	DpsBandMeasLenght 	= SautPacketHead (1, true, 1),
	Dps1 				= SautPacketHead (1, true, 1),
	Dps2 				= SautPacketHead (1, true, 2),
	Dps3 				= SautPacketHead (1, true, 3),
	Club0				= SautPacketHead (10, true, 0),
	BprQuery			= SautPacketHead (13, false, 0),
	BprVelocity			= SautPacketHead (13, false, 3),
	// Исходящие
	DpsOut0 			= SautPacketHead (1, false, 0),
	DpsOut1 			= SautPacketHead (1, false, 1),
	DpsOut2 			= SautPacketHead (1, false, 2),
	DpsOut3 			= SautPacketHead (1, false, 3),
	ClubOut0			= SautPacketHead (10, false, 0),
	ClubOut1			= SautPacketHead (10, false, 1)
};

typedef Dat <	INT_TYPELIST_7 (	// Данные для приёма
						Dps0, Dps1, Dps2, Dps3, Club0, BprQuery, BprVelocity
								),
				INT_TYPELIST_6 (	// Данные для передачи
						DpsOut0, DpsOut1, DpsOut2, DpsOut3, ClubOut0, ClubOut1
								),
				INT_TYPELIST_3 (	// Данные, по приходу которых вызываются прерывания
						Dps0, Club0, BprVelocity
								)
			> DatType;
DatType	data;

typedef Com  <  &Register::usart1Control, &Register::usart1BaudRate, &Register::usart1Data,
				 &Register::portD, 2, &Register::portD, 3, &Register::portD, 4, &Register::portB, 7,
				 1,
				 DatType, data
			  > ComType;
ComType com (USART1_RX_handler, USART1_TX_handler, USART1_UDRE_handler);



// ---------------------------------------------- КПТ -------------------------------------------►

typedef Kpt<ClockType, clock, SchedulerType, scheduler, CanDatType, canDat > KptType;

KptType kpt ( _cast( Complex<uint16_t>, data.member<ClubOut1>() )[0], _cast( Complex<uint16_t>, data.member<ClubOut1>() )[1] );

void kptRiseA (uint16_t)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
		kpt.rise();
}

void kptRiseB (uint16_t)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
		kpt.rise();
}

void kptFallA (uint16_t)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
		kpt.fall();
}

void kptFallB (uint16_t)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
		kpt.fall();
}

void kptRiseTimeA (uint16_t dataPointer)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.rise (data[0]);
	}
}

void kptRiseTimeB (uint16_t dataPointer)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.rise (data[0]);
	}
}

void kptFallTimeA (uint16_t dataPointer)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.fall (data[0]);
	}
}

void kptFallTimeB (uint16_t dataPointer)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.fall (data[0]);
	}
}

void kptCommandParse ()
{
	uint8_t command = _cast( Complex<uint16_t>, data.member<Club0>() )[1];

	if ( (command & (1 << 0)) && (command & (1 << 3)) )
		kpt.setActive ();
	else
		kpt.setPassive ();
}

// ---------------------------------------------- ДПС -------------------------------------------►

typedef
CeleritasSpatiumDimetior  < &Register::portC, 4, 5, &Register::portB, 7,
							CanDatType, canDat,
							ClockType, clock,
							SchedulerType, scheduler >
DpsType;

DpsType	dps ( 	&Register::portC,
				com.decimeters, data.member<DpsOut0>(), data.member<DpsOut1>(),
				InterruptHandler::from_method <KptType, &KptType::lisPlusPlus> (&kpt),
				InterruptHandler::from_method <KptType, &KptType::correctKptDistancePlusPlus> (&kpt) );


// --------------------------------------------- mcoState ---------------------------------------►

void sysKeyRbRelease (uint16_t)
{
	ATOMIC data.member<ClubOut0>() &= ~(1 << 15);
}

void mcoState (uint16_t pointer)
{
	typedef const uint8_t Message[8];
	Message& message = *( (Message *)(pointer) );


	// Передача сигналов КЛУБ
	uint8_t signals = _cast( Complex<uint16_t>, data.member<ClubOut0>() )[1] & 0x80; // Сохраняем текущее значение РБ
	static uint8_t convertAls[16] = {0x10,0x08,0x04,0x02,0x01,0x08,0x00,0x00,0x00,0x08,0x04,0x02,0x01,0x01,0x01,0x01};
	signals |= convertAls[ message[5]&0xF ] | ( message[5] & (1 << 5) ); // Огни АЛС и ЭПК
	if ( message[5] & ((1 << 6) | (1 << 7)) ) // РБ или РБС
	{
		signals |= (1 << 7); // нажимаем РБ
		scheduler.runIn(
					Command{ SoftIntHandler::from_function<&sysKeyRbRelease>(), 0 },
					1000 );
	}
	_cast( Complex<uint16_t>, data.member<ClubOut0>() )[1] = signals;

	// В задней кабине инвертировать направление
	dps.constituoVersusInversio ( message[3] & (1 << 5) );

	// Контроль перезагрузки ЦО.
	static uint32_t lastTime = 0;
	uint32_t time = clock.getTime();
	if ( time - lastTime > 4000 )
		reboot();
	lastTime = time;
}

void mcoStateA (uint16_t pointer)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
		mcoState (pointer);
}

void mcoStateB (uint16_t pointer)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
		mcoState (pointer);
}

// ------------------------------------ Ввод дискретных сигналов --------------------------------►

void vdsState (uint16_t pointer)
{
	typedef const uint8_t Message[8];
	Message& message = *( (Message *)(pointer) );

	if ( message[2] & (1 << 2) ) // 1 - контроллер тяги выключен
		dps.constituoNonTractus ();
	else
		dps.constituoTractus ();
}

void vdsStateA (uint16_t pointer)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
		vdsState (pointer);
}

void vdsStateB (uint16_t pointer)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
		vdsState (pointer);
}

// --------------------------------------- Эмуляция движения ------------------------------------►

class Emulation
{
public:
	Emulation ()
		: engine ( 0x5555, InterruptHandler::from_method<Emulation, &Emulation::makeAStep>(this) ), parity (false), inverseDirection (false)
//		: engine ( 0x5555, InterruptHandler(this, &Emulation::makeAStep) )
	{
		getConstants(0);
		watchDog(0);
	}

	void getVelocity ()
	{
		uint8_t newVelocity = _cast( Complex<uint16_t>, data.member<BprVelocity>() )[1];

		if ( _cast( Complex<uint16_t>, data.member<BprQuery>() )[1] & (1 << 1) &&  // Команда на эмуляцию
				newVelocity > 0 )
		{
			enable();
			if ( newVelocity != currentVelocity )
			{
				currentVelocity = newVelocity;

				uint32_t period = uint32_t(67320) * dps.diametros(0) / 1000 / newVelocity;
				if (period > 150) // Чтобы не повесить систему слишком частым заходом
					engine.setPeriod ( period );
			}
			if ( inverseDirection )
				changeDirection();
		}
		else
			disable ();

		getMessage = true;
	}
	void getCanVelocity (uint16_t pointerToData)
	{
		struct Request
		{
			uint16_t velocity	:9;
			uint16_t			:6;
			uint16_t inverseDir	:1;
		};
		Request& request = *((Request*) pointerToData);

		if ( request.velocity > 0 )
		{
			enable();
			if ( request.velocity != currentVelocity )
			{
				currentVelocity = request.velocity;

				uint32_t period = uint32_t(67320) * dps.diametros(0) / 1000 / currentVelocity;
				if (period > 150) // Чтобы не повесить систему слишком частым заходом
					engine.setPeriod ( period );
			}
//			if ( request.inverseDir != inverseDirection )
//				changeDirection();
		}
		else
			disable ();

		getMessage = true;
	}

private:
	void watchDog (uint16_t)
	{
		if ( getMessage == false )
			disable ();

		getMessage = false;
		scheduler.runIn( Command{ SoftIntHandler::from_method<Emulation, &Emulation::watchDog>(this), 0 }, 1000 );
//		scheduler.runIn( Command{ SoftIntHandler(this, &Emulation::watchDog), 0 }, 1000 );
	}
	void makeAStep ()
	{
		if (parity)
		{
			toggle (0);
			toggle (2);
		}
		else
		{
			toggle (1);
			toggle (3);
		}
		parity = !parity;
	}
	void getConstants (uint16_t )
	{
		scheduler.runIn(
				Command{ SoftIntHandler::from_method<Emulation, &Emulation::getConstants>(this), 0},
				500 );

			// Обновляем позицию датчиков
			uint32_t tmp;
			if ( eeprom.club.property.configuration.read (tmp) )
			{
				Bitfield<EepromData::Club::Property::Configuration> conf (tmp);

				if ( !engine.isEnable() )
				{
					reg.general0 = 0;
					if ( conf.dps0Position == EepromData::DpsPosition::Right )
						reg.general0 |= 0b0001;
					if ( conf.dps1Position == EepromData::DpsPosition::Right )
						reg.general0 |= 0b0100;

					inverseDirection = false;
					parity = false;
				}
			}
	}

	void enable ()
	{
		dps.accessusPortus = (Port Register::*) (&Register::general0);
		engine.enable();
	}
	void disable ()
	{
		dps.accessusPortus = &Register::portC;
		engine.disable();
	}

	void toggle (const uint8_t& n)
	{
		if ( reg.general0 & (1<<n) )
			reg.general0 &= ~(1<<n);
		else
			reg.general0 |= (1<<n);
	}

	AlarmAdjust<Alarm1A> engine;
	uint16_t currentVelocity;
	volatile bool getMessage;
	bool parity;

	bool inverseDirection;
	void changeDirection ()
	{
		inverseDirection = !inverseDirection;
		parity = !parity;
	}
};
Emulation emulation;

// ---------------------------------------- Программирование ------------------------------------►

Programming programming (
	data.member<Dps0>(),	data.member<Dps1>(),	data.member<Dps2>(),	data.member<Dps3>(),
	data.member<DpsOut0>(),	data.member<DpsOut1>(),	data.member<DpsOut2>(),	data.member<DpsOut3>() );


// ---------------------------------- Парсер команд по линии связи ------------------------------►

void commandParser ()
{
	struct Command
	{
		uint8_t							:4;
		uint8_t	highSpeed				:1;
		uint8_t	eepromRead				:1;
		uint8_t	idRead					:1;
		uint8_t	block3Byte				:1;
	};
	Bitfield<Command> command;

//	command = uint8_t(get / 256);
	command = uint8_t(data.member<DpsCommand>() / 256);

	if (command.block3Byte)
		com.block3Byte = 1;

	if (!command.idRead && !command.eepromRead)	// стандартный режим работы ДПС
	{
		data.member<DpsOut2>() = dps.diametros(0); // выводим диаметры бандажа
		data.member<DpsOut3>() = dps.diametros(1);

		dps.constituoActivus ();
	}
	else
	{
		dps.constituoPassivus ();

		if (command.idRead)
		{
			data.member<DpsOut0>() = ( (uint16_t) pgm_read_byte(&id.version) << 8 ) | pgm_read_byte(&id.year);
			data.member<DpsOut1>() = ( (uint16_t) pgm_read_byte(&id.modif)   << 8 ) | pgm_read_byte(&id.manth);
			data.member<DpsOut2>() = ( (uint16_t) pgm_read_byte(&id.numberH) << 8 ) | ( (uint16_t) pgm_read_byte(&id.numberL) );
			data.member<DpsOut3>() = ( (uint16_t) pgm_read_byte(&id.parametersSummH)  << 8 ) | ( (uint16_t) pgm_read_byte(&id.parametersSummL) );
		}

		if (command.eepromRead)
		{
			uint8_t adr = (data.member<DpsCommand>() & 0x000F) * 8;// Адрес из линии RS-485 * 8
			data.member<DpsOut0>() = Complex<uint16_t> { mph.sautConvert.plainMap[adr+1], mph.sautConvert.plainMap[adr+0] };
			data.member<DpsOut1>() = Complex<uint16_t> { mph.sautConvert.plainMap[adr+3], mph.sautConvert.plainMap[adr+2] };
			data.member<DpsOut2>() = Complex<uint16_t> { mph.sautConvert.plainMap[adr+5], mph.sautConvert.plainMap[adr+4] };
			data.member<DpsOut3>() = Complex<uint16_t> { mph.sautConvert.plainMap[adr+7], mph.sautConvert.plainMap[adr+6] };
		}
	}

	if (data.member<DpsCommand>() == 0x0efe)			// Переход в режим программирования
	{
		dps.constituoPassivus ();

		programming.enterProgramMode();
		data.interruptHandler<Dps0> () = InterruptHandler::from_method <Programming, &Programming::comParser> (&programming);
	}
}

void unsetResetFlag (uint16_t)
{
	dps.repeto = false;
}

// --------------------------------------------- main -------------------------------------------►

int main ()
{
	// Контроль контрольной суммы
	if ( !(pgm_read_word (&id.size) == 0 && pgm_read_word (&id.controlSumm) == 0) ) // Если значения подставлены при программировании
	{
		uint16_t *size = (uint16_t *) (pgm_read_word (&id.size) * 16); // Размер в словах. Максимум 64 кб
		Complex<uint16_t> sum = 0;
		for ( uint16_t *i = 0; i < size; i ++ )
			sum += pgm_read_word (i);

		if (sum != 0) // В id.controlSumm хранится дополнение до 0
			reboot ();
	}

	data.interruptHandler<DpsCommand> () = InterruptHandler::from_function<&commandParser>();
	data.interruptHandler<Club0> () = InterruptHandler::from_function<&kptCommandParse>();
	data.interruptHandler<BprVelocity> () = InterruptHandler::from_method<Emulation, &Emulation::getVelocity> (&emulation);

	canDat.rxHandler<CanRx::MCO_STATE_A>() = SoftIntHandler::from_function <&mcoStateA>();
	canDat.rxHandler<CanRx::MCO_STATE_B>() = SoftIntHandler::from_function <&mcoStateB>();

	dps.constituoActivus();

	sei();

	scheduler.runIn( Command {SoftIntHandler::from_function<&unsetResetFlag>(), 0}, 7000 );

    for (;;)
    {
    	static bool resetButtonWasFree = false;
    	resetButtonWasFree |= reg.portB.pin5;
    	if ( resetButtonWasFree && !reg.portB.pin5 ) // Нажата кнопка сброса (а до этого была отпущена)
    	{
    		eeprom.dps0Good = 1;
    		eeprom.dps1Good = 1;
    		reboot();
    	}

    	dispatcher.invoke();
    	wdt_reset();
    	scheduler.invoke();
    	wdt_reset();
    }
}


