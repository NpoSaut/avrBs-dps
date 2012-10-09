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
#include "mph.h"
#include "neutral-insertion.h"




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

typedef INT_TYPELIST_3	(CanTx::IPD_STATE_A,	CanTx::IPD_STATE_B, CanTx::IPD_DPS_FAULT) IPD_STATE;
typedef INT_TYPELIST_2	(CanTx::SAUT_INFO_A,	CanTx::SAUT_INFO_B) SAUT_INFO;
typedef INT_TYPELIST_3	(CanTx::SYS_DATA_STATE_A, CanTx::SYS_DATA_STATE_B,
						 CanTx::IPD_NEUTRAL) SYS_DATA_STATE_IPD_NEUTRAL;
typedef INT_TYPELIST_2	(CanTx::SYS_DATA_STATE2_A, CanTx::SYS_DATA_STATE2_B) SYS_DATA_STATE2;
typedef INT_TYPELIST_2	(CanTx::MPH_STATE_A, CanTx::MPH_STATE_B) MPH_STATE;
typedef INT_TYPELIST_12 (CanTx::AUX_RESOURCE_BS_A,	CanTx::AUX_RESOURCE_BS_B,
						 CanTx::AUX_RESOURCE_IPD_A,	CanTx::AUX_RESOURCE_IPD_B,
						 CanTx::SYS_DATA_A, CanTx::SYS_DATA_B,
						 CanTx::MY_DEBUG_A, CanTx::MY_DEBUG_B,
						 CanTx::MY_KPT_A, CanTx::MY_KPT_B,
						 CanTx::IPD_PARAM_A, CanTx::IPD_PARAM_B ) AUX_RESOURCE_SYS_DATA_IPD_PARAM;
typedef INT_TYPELIST_2 	(CanTx::PROGRAM_SLAVE_CTRL, CanTx::PROGRAM_SLAVE_DATA) PROGRAM_SLAVE;

typedef INT_TYPELIST_5 (CanRx::MCO_STATE_A, CanRx::MCO_STATE_B,
						CanRx::MCO_LIMITS_A, CanRx::MCO_LIMITS_B,
						CanRx::MCO_DATA) MCO;
typedef INT_TYPELIST_8 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A,
						CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS;
typedef INT_TYPELIST_2 (CanRx::MM_DATA, CanRx::MM_NEUTRAL) MM;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA_A) INPUT;
typedef INT_TYPELIST_3 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B) DIAGNOSTICS;
typedef INT_TYPELIST_2 (CanRx::PROGRAM_MASTER_CTRL, CanRx::PROGRAM_MASTER_DATA) PROGRAM_MASTER;

typedef CanDat < LOKI_TYPELIST_7(					// Список дескрипторов для отправки
						IPD_STATE,
						SAUT_INFO,
						SYS_DATA_STATE_IPD_NEUTRAL,
						SYS_DATA_STATE2,
						MPH_STATE,
						AUX_RESOURCE_SYS_DATA_IPD_PARAM,
						PROGRAM_SLAVE
								),
				 LOKI_TYPELIST_8(
						 MCO,
						 Int2Type< CanRx::SYS_DATA_QUERY >,
						 MP_ALS,
						 MM,
						 INPUT,
						 DIAGNOSTICS,
						 Int2Type< CanRx::IPD_EMULATION >,
						 PROGRAM_MASTER
						 	 	 ),
				 LOKI_TYPELIST_23(
						 Int2Type< CanRx::INPUT_DATA >,
						 Int2Type< CanRx::MCO_DATA >,
						 Int2Type< CanRx::BKSI_DATA >,
						 Int2Type< CanTx::SYS_DATA_A >,
						 Int2Type< CanRx::SYS_DATA_QUERY >,
						 Int2Type< CanRx::SYS_DIAGNOSTICS >,
						 Int2Type< CanRx::AUX_RESOURCE_MCO_A >,
						 Int2Type< CanRx::AUX_RESOURCE_MCO_B >,
						 Int2Type< CanRx::MCO_STATE_A >,
						 Int2Type< CanRx::MCO_STATE_B >,
						 Int2Type< CanRx::MM_DATA >,
						 Int2Type< CanRx::MP_ALS_ON_A >,
						 Int2Type< CanRx::MP_ALS_OFF_A >,
						 Int2Type< CanRx::MP_ALS_ON_B >,
						 Int2Type< CanRx::MP_ALS_OFF_B >,
						 Int2Type< CanRx::MP_ALS_ON_TIME_A >,
						 Int2Type< CanRx::MP_ALS_OFF_TIME_A >,
						 Int2Type< CanRx::MP_ALS_ON_TIME_B >,
						 Int2Type< CanRx::MP_ALS_OFF_TIME_B >,
						 Int2Type< CanRx::MM_NEUTRAL >,
						 Int2Type< CanRx::IPD_EMULATION>,
						 Int2Type< CanRx::PROGRAM_MASTER_CTRL>,
						 Int2Type< CanRx::PROGRAM_MASTER_DATA>
								),
					128,
					LOKI_TYPELIST_2(
						 Int2Type< CanTx::PROGRAM_SLAVE_CTRL>,
						 Int2Type< CanTx::PROGRAM_SLAVE_DATA>
								),
				 100 >									// BaudRate = 100 Кбит, SamplePoint = 75% (по умолчанию)
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


// ---------------------------------- SYS_DIAGNOSTICS / AUX_RESOURCE ----------------------------►

void sysDiagnostics (uint16_t a)
{
	enum class Unit : uint8_t
	{
		MCO = 1,
		MM,
		MP_ALS,
		IPD = 4,
		MPSU,
		RK,
		BIL,
		SAUT,
		TCKBM_KCAN,
		UKTOL,
		CANGate,
		BR_UMO = 14,
		TSKBM_PCAN,
		BS_DPS = 16,
		ALS_TKS,
		EPK
	};
	enum class Request : uint8_t
	{
		PASSIVE = 1,
		ACTIVE,
		TEST_RUN,
		READ,
		VERSION,
		DIST_TRAVEL_WRITE, // IPD
		ECARD_NUMBER_READ = 10, // MM
		DIST_TRAVEL_READ_A = 13, // IPD
		DIST_TRAVEL_READ_B, // IPD
		BIL_DIAG, // BIL
		BVU_ALSN_SEND_ON = 33, // ALS_TKS
		BVU_ALSN_SEND_OFF, // ALS_TKS,
		ALSN_DIAG_ON, // ALS_TKS
		ALSN_DIAG_OFF, // ALS_TKS
		ALS_EN_DIAG_ON, // ALS_TKS
		ALSN_EN_DIAG_OFF, // ALS_TKS
		RC_STATE_READ, // RK
		MCO_ACTIVE_N_READ // MCO
	};
	enum class Answer : uint8_t
	{
		VERSION = 0,
		ERROR = 1,
		WARNING = 2,
		DATA = 208,
		BUSY = 251,
		FAIL,
		REPEAT,
		UNKNOWN,
		OK
	};
	Unit unit = (Unit) canDat.get<CanRx::SYS_DIAGNOSTICS>() [0];
	Request request = (Request) canDat.get<CanRx::SYS_DIAGNOSTICS>() [1];


	if (unit == Unit::IPD || unit == Unit::BS_DPS)
	{
		if ( request == Request::VERSION  )
		{
			uint8_t idSize = pgm_read_byte(&id.idSize)*8; // Размер в словах
			uint16_t checkSumm = 0;
			for (uint8_t i = 0; i < idSize; i ++)
				checkSumm += pgm_read_word ((uint16_t *)&id + i);

			uint8_t packet[5] = {
					(uint8_t) Answer::VERSION,
					pgm_read_byte(&id.version),
					0,
					0,
					uint8_t (checkSumm)
								};
			if (unit == Unit::IPD)
			{
				if (reg.portB.pin7 == 0)
					canDat.send<CanTx::AUX_RESOURCE_IPD_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_IPD_B>(packet);
			}
			else if (unit == Unit::BS_DPS)
			{
				if (reg.portB.pin7 == 0)
					canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
			}
		}
//		else if ( request == Request::DIST_TRAVEL_WRITE )
//		{
//			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
//			eeprom_update_byte( adr  , canDat.get<CanRx::SYS_DIAGNOSTICS>() [5] );
//			eeprom_update_byte( adr+1, canDat.get<CanRx::SYS_DIAGNOSTICS>() [4] );
//			eeprom_update_byte( adr+2, canDat.get<CanRx::SYS_DIAGNOSTICS>() [3] );
//			eeprom_update_byte( adr+3, canDat.get<CanRx::SYS_DIAGNOSTICS>() [2] );
//		}
//		else if ( request == Request::DIST_TRAVEL_READ_A && reg.portB.pin7 == 0 )
//		{
//			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
//			uint8_t packet[5] = {
//					(uint8_t) Answer::DATA,
//					eeprom_read_byte (adr+3),
//					eeprom_read_byte (adr+2),
//					eeprom_read_byte (adr+1),
//					eeprom_read_byte (adr)
//								};
//
//			canDat.send<CanTx::AUX_RESOURCE_IPD_A> (packet);
//		}
//		else if ( request == Request::DIST_TRAVEL_READ_B && reg.portB.pin7 != 0 )
//		{
//			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
//			uint8_t packet[5] = {
//					(uint8_t) Answer::DATA,
//					eeprom_read_byte (adr+3),
//					eeprom_read_byte (adr+2),
//					eeprom_read_byte (adr+1),
//					eeprom_read_byte (adr)
//								};
//
//			canDat.send<CanTx::AUX_RESOURCE_IPD_B> (packet);
//		}
		else if ( request == Request::TEST_RUN && unit == Unit::BS_DPS )
		{
			uint8_t packet[5] = {
					(uint8_t) Answer::DATA,
					0,
					0,
					0,
					0
								};
			if (reg.portB.pin7 == 0)
				canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
			else
				canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
		}
	}
}


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

	// Определение, есть ли тяга
	if ( message[0] & (1 << 5) )
		dps.constituoNonTractus ();
	else
		dps.constituoTractus ();

	// В задней кабине инвертировать направление
	dps.constituoVersusInversio ( message[3] & (1 << 5) );

	// Контроль перезагрузки ЦО.
	static uint32_t lastTime = 0;
	uint32_t time = clock.getTime();
	if ( time - lastTime > 4000 )
		reboot();
	lastTime = time;

	// Контроль выхода из конфигурации
	if ( !(message[6] & (1 << 1) && message[7] & (1 << 6)) &&	// выход БС-ДПС или ИПД
			clock.getTime() > 7000 && 	// проработали больше 7 секунд
			dps.sicinActivus() && // активность модуля ДПС говорит о том, что мы не в режиме программирования и т.д.
			!dps.sicinCausarius() ) // если оба датчика неисправны, то перезагрузку не делать
	{
		reboot ();
	}

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

// Для контроля выхода из конфигурации МПХ
void mcoAuxResControl (uint16_t pointer)
{
	typedef const uint8_t Message[5];
	Message& message = *( (Message *)(pointer) );

	if ( message[0] == 2 && message[1] == 3 && !(message[2] & (1 << 2)) && // Кодовая комбинация, означающая выход МПХ из конфиграции
			clock.getTime() > 7000 && 	// проработали больше 7 секунд
			dps.sicinActivus() ) // активность модуля ДПС говорит о том, что мы не в режиме программирования и т.д.
	{
		reboot ();
	}
}

void mcoAuxResA (uint16_t pointer)
{
	if (reg.portB.pin7 == 0) // первый полукомплект
		mcoAuxResControl (pointer);
}

void mcoAuxResB (uint16_t pointer)
{
	if (reg.portB.pin7 == 1) // второй полукомплект
		mcoAuxResControl (pointer);
}

// --------------------------------- Модуль постоянных характеристик ----------------------------►

typedef ConstValModule <CanDatType, canDat, SchedulerType, scheduler> MPHType;
MPHType mph;

// ------------------------------------- Нейтральная вставка -------------------------------------►

typedef NeutralInsertion<CanDatType, canDat, SchedulerType, scheduler, DpsType, dps> NeutralInsertionType;
NeutralInsertionType neutralInsertion;

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

typedef ProgrammingCan <CanDatType, canDat, CanTx::PROGRAM_SLAVE_CTRL, CanTx::PROGRAM_SLAVE_DATA > ProgrammingCanType;
ProgrammingCanType programmingCan (	Delegate<void ()>::from_method<DpsType, &DpsType::constituoActivus> (&dps),
										Delegate<void ()>::from_method<DpsType, &DpsType::constituoPassivus> (&dps),
										reg.portB.pin7 == 0
									);

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
	asm volatile ("nop"); // !!! 126 version hack !!!
//	asm volatile ("nop"); // Для того чтобы сделать размер программы картным 6
//	asm volatile ("nop");
//	asm volatile ("nop");

	data.interruptHandler<DpsCommand> () = InterruptHandler::from_function<&commandParser>();
	data.interruptHandler<Club0> () = InterruptHandler::from_function<&kptCommandParse>();
	data.interruptHandler<BprVelocity> () = InterruptHandler::from_method<Emulation, &Emulation::getVelocity> (&emulation);
	canDat.rxHandler<CanRx::IPD_EMULATION>() = SoftIntHandler::from_method <Emulation, &Emulation::getCanVelocity>(&emulation);

	canDat.rxHandler<CanRx::INPUT_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanRx::MCO_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanRx::BKSI_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanTx::SYS_DATA_A>() = SoftIntHandler::from_method <MPHType, &MPHType::getLeftDataMessage> (&mph); // Если кто-то ещё ответит, то обновить мои данные
	canDat.rxHandler<CanRx::SYS_DATA_QUERY>() = SoftIntHandler::from_method <MPHType, &MPHType::getQueryMessage> (&mph);

	if (reg.portB.pin7 == 0) // первый полукомплект
	{
		canDat.rxHandler<CanRx::MM_NEUTRAL>() = SoftIntHandler::from_method <NeutralInsertionType, &NeutralInsertionType::getEcData> (&neutralInsertion);
	}

	canDat.rxHandler<CanRx::SYS_DIAGNOSTICS>() = SoftIntHandler::from_function <&sysDiagnostics>();

	canDat.rxHandler<CanRx::MCO_STATE_A>() = SoftIntHandler::from_function <&mcoStateA>();
	canDat.rxHandler<CanRx::MCO_STATE_B>() = SoftIntHandler::from_function <&mcoStateB>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_A>() = SoftIntHandler::from_function <&mcoAuxResA>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_B>() = SoftIntHandler::from_function <&mcoAuxResB>();

	canDat.rxHandler<CanRx::MM_DATA>() = SoftIntHandler::from_method <DpsType, &DpsType::takeEcDataForAdjust> (&dps);

	canDat.rxHandler<CanRx::MP_ALS_ON_A>() = SoftIntHandler::from_function <&kptRiseA>();
	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_A>() = SoftIntHandler::from_function <&kptRiseTimeA>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_A>() = SoftIntHandler::from_function <&kptFallA>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_A>() = SoftIntHandler::from_function <&kptFallTimeA>();
	canDat.rxHandler<CanRx::MP_ALS_ON_B>() = SoftIntHandler::from_function <&kptRiseB>();
	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_B>() = SoftIntHandler::from_function <&kptRiseTimeB>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_B>() = SoftIntHandler::from_function <&kptFallB>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_B>() = SoftIntHandler::from_function <&kptFallTimeB>();

	// Программирование по CAN
	canDat.rxHandler<CanRx::PROGRAM_MASTER_CTRL>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::getCommand> (&programmingCan);
	canDat.rxHandler<CanRx::PROGRAM_MASTER_DATA>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::getData> (&programmingCan);
	canDat.txHandler<CanTx::PROGRAM_SLAVE_DATA>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::sendData> (&programmingCan);

		dps.constituoActivus();

	sei();

	// После включения выдавать AUX_RESOURCE с версией
	{
		uint8_t idSize = pgm_read_byte(&id.idSize)*8; // Размер в словах
		uint16_t checkSumm = 0;
		for (uint8_t i = 0; i < idSize; i ++)
			checkSumm += pgm_read_word ((uint16_t *)&id + i);

		uint8_t packet[5] = {
				0,
				pgm_read_byte(&id.version),
				0,
				0,
				uint8_t (checkSumm)
							};
		if (reg.portB.pin7 == 0)
		{
			canDat.send<CanTx::AUX_RESOURCE_IPD_A>(packet);
			_delay_ms (10);
			canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
		}
		else
		{
			canDat.send<CanTx::AUX_RESOURCE_IPD_B>(packet);
			_delay_ms (10);
			canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
		}
	}

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

//    	static uint16_t ctr = 0;
//    	if (ctr++ > 2000)
//    	{
//    		sei();
//    		ctr = 0;
//			uint8_t sysDataState2[8] = {
//					0, // Результаты выполнения тестов... здесь не выводим
//					0,
//					0,
//					0,
//					0,
//					scheduler.fill,
//					dispatcher.maxSize,
//					0
//									};
//			dispatcher.maxSize = 0;
//			if (reg.portB.pin7 == 0) // первый полукомплект
//				canDat.send<CanTx::SYS_DATA_STATE2_A> (sysDataState2);
//			else
//				canDat.send<CanTx::SYS_DATA_STATE2_B> (sysDataState2);
//    	}

    	dispatcher.invoke();
    	wdt_reset();
    	scheduler.invoke();
    	wdt_reset();
    }
}


