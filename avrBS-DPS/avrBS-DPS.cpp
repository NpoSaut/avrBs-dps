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
#include <avr/wdt.h>
//#include <cpp/interrupt-dynamic.h>
#include <cpp/io.h>
#include <cpp/universal.h>
#include <cpp/dispatcher.h>
#include <cpp/scheduler.h>
#include <cpp/timers.h>
#include <util/delay.h>

#define SMARTDOG_ALARM Alarm2
#define SMARTDOG_WDT_TIME WDTO_30MS
#define SMARTDOG_ALARM_TIME 19968
#include <cpp/smartdog.h>

#include "hw_defines.h"
#include "SautCom.h"
#include "SautDat.h"
#include "id.h"
#include "programming.h"
#include "dps.h"
#include "eeprom.h"
#include "cpp/can-dat.h"
#include "cpp/can-async.h"
#include "CanDesriptors.h"
#include "kpt.h"
#include "mph.h"
#include "neutral-insertion.h"
#include "diagnostic.h"

void Init (void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void Init (void)
{
	// Светодиоды.
	reg.portD.pin7.out();
	reg.portD.pin7 = true;
	configSelfComplectPin();
	lconfig();

	// Watchdog
	wdt_enable (WDTO_250MS);
	// smartdog будет инициализирован позже

	// Кнопка сброса
	reg.portB.pin5.in();
}

// ----------------------------------- Системные часы и планировщик -----------------------------►

typedef Clock< Alarm< Alarm3A, 1000 >, uint32_t > ClockType;  // Обнуление через 50 суток
ClockType clock;

typedef Scheduler< ClockType, clock, Dispatcher, dispatcher, 16, uint16_t > SchedulerType;
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

typedef INT_TYPELIST_5 (CanRx::MCO_STATE_A, CanRx::MCO_STATE_B,
						CanRx::MCO_LIMITS_A, CanRx::MCO_LIMITS_B,
						CanRx::MCO_DATA) MCO;
typedef INT_TYPELIST_8 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A,
						CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS;
typedef INT_TYPELIST_2 (CanRx::MM_DATA, CanRx::MM_NEUTRAL) MM;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA_A) INPUT;
typedef INT_TYPELIST_3 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B) DIAGNOSTICS;

typedef CanDat < LOKI_TYPELIST_7(					// Список дескрипторов для отправки
						IPD_STATE,
						SAUT_INFO,
						SYS_DATA_STATE_IPD_NEUTRAL,
						SYS_DATA_STATE2,
						MPH_STATE,
						AUX_RESOURCE_SYS_DATA_IPD_PARAM,
						Int2Type< CanTx::FU_DEV >
								),
				 LOKI_TYPELIST_8(
						 MCO,
						 Int2Type< CanRx::SYS_DATA_QUERY >,
						 MP_ALS,
						 MM,
						 INPUT,
						 DIAGNOSTICS,
						 Int2Type< CanRx::IPD_EMULATION >,
						 Int2Type< CanRx::FU_INIT >
						 	 	 ),
				 LOKI_TYPELIST_22(
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
						 Int2Type< CanRx::IPD_EMULATION >,
						 Int2Type< CanRx::FU_INIT >
								),
					128,
					NullType,
				 100 >									// BaudRate = 100 Кбит, SamplePoint = 75% (по умолчанию)
	CanDatType;
CanDatType canDat;

typedef CanAsync<CanDatType, canDat, Dispatcher, dispatcher, 16> CanAsyncType;
CanAsyncType canAsync;

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
	// �?сходящие
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

// ------------------------------- Работа с загрузчиком-программатором --------------------------►

typedef ProgrammingCan <CanDatType, canDat, CanRx::FU_INIT, CanTx::FU_DEV > ProgrammingCanType;
ProgrammingCanType programmingCan;

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
			uint8_t packet[5];
			if ( programmingCan.isAvailable () )
			{
				packet[0] =	(uint8_t) Answer::VERSION;
				packet[1] =	(uint8_t) programmingCan.getVersion();
				packet[2] =	(uint8_t) programmingCan.getSubversion();
				packet[3] =	(uint8_t) (programmingCan.getCheckSum() & 0xFF);
				packet[4] =	(uint8_t) (programmingCan.getCheckSum() >> 8);			
			}
			else
			{
				uint8_t idSize = pgm_read_byte(&id.idSize)*8; // Размер в словах
				uint16_t checkSumm = 0;
				for (uint8_t i = 0; i < idSize; i ++)
				checkSumm += pgm_read_word ((uint16_t *)&id + i);

				packet[0] =	(uint8_t) Answer::VERSION;
				packet[1] =	pgm_read_byte(&id.version);
				packet[2] =	0;
				packet[3] =	0;
				packet[4] =	uint8_t (checkSumm);
			}

			if (unit == Unit::IPD)
			{
				if (isSelfComplectA ())
					canDat.send<CanTx::AUX_RESOURCE_IPD_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_IPD_B>(packet);
			}
			else if (unit == Unit::BS_DPS)
			{
				if (isSelfComplectA ())
					canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
			}
		}
		else if ( request == Request::TEST_RUN && unit == Unit::BS_DPS )
		{
			uint8_t packet[5] = {
					(uint8_t) Answer::DATA,
					0,
					0,
					0,
					0
								};
			if (isSelfComplectA ())
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
	if (isSelfComplectA ()) // первый полукомплект
		kpt.rise();
}

void kptRiseB (uint16_t)
{
	if (!isSelfComplectA ()) // второй полукомплект
		kpt.rise();
}

void kptFallA (uint16_t)
{
	if (isSelfComplectA ()) // первый полукомплект
		kpt.fall();
}

void kptFallB (uint16_t)
{
	if (!isSelfComplectA ()) // второй полукомплект
		kpt.fall();
}

void kptRiseTimeA (uint16_t dataPointer)
{
	if (isSelfComplectA ()) // первый полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.rise (data[0]);
	}
}

void kptRiseTimeB (uint16_t dataPointer)
{
	if (!isSelfComplectA ()) // второй полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.rise (data[0]);
	}
}

void kptFallTimeA (uint16_t dataPointer)
{
	if (isSelfComplectA ()) // первый полукомплект
	{
		typedef const uint8_t Data[1];
		Data& data = *( (Data *)(dataPointer) );
		kpt.fall (data[0]);
	}
}

void kptFallTimeB (uint16_t dataPointer)
{
	if (!isSelfComplectA ()) // второй полукомплект
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
CeleritasSpatiumDimetior  < CanDatType, canDat,
							ClockType, clock,
							SchedulerType, scheduler >
DpsType;

DpsType	dps ( 	Delegate<void (bool)>::from_function< &lset<0> >(), Delegate<void (bool)>::from_function< &lset<1> >(), isSelfComplectA(),
				&Register::portC,
				com.decimeters, data.member<DpsOut0>(), data.member<DpsOut1>(),
				InterruptHandler::from_method <KptType, &KptType::lisPlusPlus> (&kpt),
				InterruptHandler::from_method <KptType, &KptType::correctKptDistancePlusPlus> (&kpt) );

// ---------------------------- Программирование по линии связи САТУ/RS-485 ---------------------►

Programming programming (
	data.member<Dps0>(),	data.member<Dps1>(),	data.member<Dps2>(),	data.member<Dps3>(),
	data.member<DpsOut0>(),	data.member<DpsOut1>(),	data.member<DpsOut2>(),	data.member<DpsOut3>() );

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

	// В задней кабине инвертировать направление, если это не маневровый
	uint32_t locoType = 0;
	if ( eeprom.club.property.category.read (locoType) )
		if ( locoType != 7 ) // не маневровый
			dps.constituoVersusInversio ( message[3] & (1 << 5) );

	// Контроль перезагрузки ЦО.
	static uint32_t lastTime = 0;
	uint32_t time = clock.getTime();
	if ( time - lastTime > 4000 )
		diagnostic_restart(RestartReason::CO_LOST);
	lastTime = time;

	// Контроль выхода из конфигурации
	if ( clock.getTime() > 7000 && 	// проработали больше 7 секунд
		dps.sicinActivus() && // активность модуля ДПС говорит о том, что мы не в режиме программирования и т.д.
		!dps.sicinBothBrake() ) // если оба датчика неисправны, то перезагрузку не делать
	{
		if( !(message[6] & (1 << 1)) )
			diagnostic_restart(RestartReason::IPD_OUT);
		if( !(message[7] & (1 << 6)) )
			diagnostic_restart(RestartReason::DPS_OUT);
	}

}

void mcoStateA (uint16_t pointer)
{
	if (isSelfComplectA ()) // первый полукомплект
		mcoState (pointer);
}

void mcoStateB (uint16_t pointer)
{
	if (!isSelfComplectA ()) // второй полукомплект
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
		diagnostic_restart(RestartReason::MPH_OUT);
	}
}

void mcoAuxResA (uint16_t pointer)
{
	if (isSelfComplectA ()) // первый полукомплект
		mcoAuxResControl (pointer);
}

void mcoAuxResB (uint16_t pointer)
{
	if (!isSelfComplectA ()) // второй полукомплект
		mcoAuxResControl (pointer);
}

// --------------------------------- Модуль постоянных характеристик ----------------------------►

typedef ConstValModule <CanAsyncType, canAsync, SchedulerType, scheduler> MPHType;
MPHType mph;

// ------------------------------------- Нейтральная вставка -------------------------------------►

typedef NeutralInsertion<CanDatType, canDat, SchedulerType, scheduler, DpsType, dps> NeutralInsertionType;
NeutralInsertionType neutralInsertion;

// --------------------------------------- Эмуляция движения ------------------------------------►

class Emulation
{
public:
	Emulation ()
		: engine ( 0x5555, InterruptHandler::from_method<Emulation, &Emulation::makeAStep>(this) ),
		  current (),
		  sautCurrentVelocity (0),
		  sautCurrentVirtualCanMessage (),
		  step (0)
	{
		scheduler.runIn( Command{ SoftIntHandler::from_method<Emulation, &Emulation::watchDog>(this), 0 }, 1000 );
	}

	void getSautVelocity ()
	{
		if ( _cast( Complex<uint16_t>, data.member<BprQuery>() )[1] & (1 << 1) )  // Команда на эмуляцию
		{
			uint8_t newVelocity = _cast( Complex<uint16_t>, data.member<BprVelocity>() )[1];

			if ( newVelocity != sautCurrentVelocity )
			{
				sautCurrentVelocity = newVelocity;

				// Выставляем направления вращения датчиков в зависимости от расположения таким образом, чтобы всегда ехать вперёд
				Bitfield<EepromData::Saut::Property::Configuration> config;
				config = eeprom.saut.property.configuration;
				sautCurrentVirtualCanMessage.sensor[0].clockwise = config.dps0Position == EepromData::DpsPosition::Right ? 1 : 0;
				sautCurrentVirtualCanMessage.sensor[1].clockwise = config.dps1Position == EepromData::DpsPosition::Right ? 1 : 0;

				for (uint8_t i = 0; i < 2; i ++)
				{
					// Рачитываем период импульсов
					sautCurrentVirtualCanMessage.sensor[0].freqX16 = uint32_t (5941784) * sautCurrentVelocity / dps.diametros(0) / 100;
																	//sautCurrentVelocity * 42 * 16 / 3,6 / 3,14 / dps.diametros(0) * 1000;
					sautCurrentVirtualCanMessage.sensor[i].broke1 = 0;
					sautCurrentVirtualCanMessage.sensor[i].broke2 = 0;
				}
			}

			getCanVelocity((uint16_t) &sautCurrentVirtualCanMessage);
		}
	}

	void getCanVelocity (uint16_t pointer)
	{
		IpdEmulationMessage& target = *( (IpdEmulationMessage *)(pointer) );

		if ( !(target == current) )
		{
			current = target;

			if (current.sensor[0].freqX16 != 0)
				enable();
			else
				disable ();
		}

		getMessage = true;
	}

private:
	class IpdEmulationMessage
	{
	public:
		IpdEmulationMessage ()
		{
			*( (uint32_t *) this   ) = 0;
			*(((uint32_t *) this)+1) = 0;
		}
		bool operator == (const IpdEmulationMessage& b) const
		{
			return     *( (uint32_t *) this   ) == *( (uint32_t *) &b   )
					&& *(((uint32_t *) this)+1) == *(((uint32_t *) &b)+1);
		}

		struct Sensor
		{
			uint16_t freqX16		:16;
			uint16_t clockwise		:1;
			uint16_t broke1			:1;
			uint16_t broke2			:1;
			uint16_t				:13;
		};
		Sensor sensor[2];
	};

	void watchDog (uint16_t)
	{
		if ( getMessage == false )
			disable ();

		getMessage = false;
		scheduler.runIn( Command{ SoftIntHandler::from_method<Emulation, &Emulation::watchDog>(this), 0 }, 1000 );
	}
	void makeAStep ()
	{
		reg.general0 = currentOperationSequence[(step++) & 0x3];
	}
	void enable ()
	{
		// Формируем последовательность значений порта,
		// которые потом будем просто перебирать
		for (uint8_t i = 0; i < 4; i++)
		{
			currentOperationSequence[i] = 0;
			for (uint8_t iSensor = 0; iSensor < 2; iSensor ++)
				currentOperationSequence[i] |=
						( rotationClockwiseCode[current.sensor[iSensor].clockwise ? i : i^3] // ^3 делает из 0,1,2,3 -> 3,2,1,0
								& (current.sensor[iSensor].broke1 ? ~(1 << 0) : 0xFF)
								& (current.sensor[iSensor].broke2 ? ~(1 << 1) : 0xFF)
						) << (iSensor*2);
		}

		// Устанавливает период таймера, генерируещего импульсы
		uint32_t period = uint32_t(4000000) / current.sensor[0].freqX16;
		if (period > 150) // Чтобы не повесить систему слишком частым заходом
			engine.setPeriod ( period );

		// Натравливаем БС-ДПС
		dps.accessusPortus = (Port Register::*) (&Register::general0);

		// Поехали
		engine.enable();
	}
	void disable ()
	{
		dps.accessusPortus = &Register::portC;
		engine.disable();
	}

	// Перебор этих значений слева направо соответсвует вращению по часовой стрелке
	static const uint8_t rotationClockwiseCode [4];
	uint8_t currentOperationSequence [4];

	AlarmAdjust<Alarm1A> engine;
	IpdEmulationMessage current;
	uint8_t sautCurrentVelocity;
	IpdEmulationMessage sautCurrentVirtualCanMessage;
	volatile bool getMessage;
	uint8_t step;
};
const uint8_t Emulation::rotationClockwiseCode[4] = {0, 1,  3, 2};
Emulation emulation;


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

		dps.constituoMittoSaut (true);
	}
	else
	{
		dps.constituoMittoSaut (false);

		if (command.idRead)
		{
			if ( programmingCan.isAvailable() )
			{
				data.member<DpsOut0>() = ( (uint16_t) programmingCan.getVersion() 			<< 8 ) | ( programmingCan.getCellManufactureYear() - 1980 );
				data.member<DpsOut1>() = ( (uint16_t) programmingCan.getCellModification()  << 8 ) | programmingCan.getCellManufactureMonth();
				data.member<DpsOut2>() = ( (uint16_t) programmingCan.getCellManufactureNumber()  );
				data.member<DpsOut3>() = (    uint16_t ( programmingCan.getVersion() & 0xFF )
											+ uint16_t ( (programmingCan.getCellManufactureYear()-1980) & 0xFF )
											+ uint16_t ( programmingCan.getCellModification() & 0xFF )
											+ uint16_t ( programmingCan.getCellManufactureMonth() & 0xFF )
											+ uint16_t ( (programmingCan.getCellManufactureNumber() >> 8) & 0xFF )
											+ uint16_t ( programmingCan.getCellManufactureNumber() & 0xFF )
										 );
			}
			else
			{
				data.member<DpsOut0>() = ( (uint16_t) pgm_read_byte(&id.version) << 8 ) | pgm_read_byte(&id.year);
				data.member<DpsOut1>() = ( (uint16_t) pgm_read_byte(&id.modif)   << 8 ) | pgm_read_byte(&id.manth);
				data.member<DpsOut2>() = ( (uint16_t) pgm_read_byte(&id.numberH) << 8 ) | ( (uint16_t) pgm_read_byte(&id.numberL) );
				data.member<DpsOut3>() = ( (uint16_t) pgm_read_byte(&id.parametersSummH)  << 8 ) | ( (uint16_t) pgm_read_byte(&id.parametersSummL) );
			}
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
		smartdog_off ();
		
		programming.enterProgramMode();
		data.interruptHandler<Dps0> () = InterruptHandler::from_method <Programming, &Programming::comParser> (&programming);
	}
}

void unsetResetFlag (uint16_t)
{
	dps.repeto = false;
}

// -------------------------------------- store restart reason ----------------------------------►

void storeRestartReason (uint8_t val)
{
	eeprom.restartReason = val;
}

uint8_t restoreRestartReason ()
{
	return eeprom.restartReason;
}

void canBusOffHandler ()
{
	diagnostic_restart (RestartReason::CAN_BUSOFF);
}

void dispatcherOverflowHandler (uint16_t detail)
{
	diagnostic_restart (RestartReason::DISPATCHER_OVER, detail);
}

void schedulerFullHandler ()
{
	diagnostic_sendInfo (RestartReason::SCHEDULER_FULL);
}

void programmingRebootHandler ()
{
	diagnostic_restart (RestartReason::PROGRAM_MODE);
}

void smartdogAlarm (uint16_t wdtPointer)
{
	Complex<uint32_t> ptr = uint32_t(dispatcher.getCurrentCommandPointer()) * 2;
	diagnostic_sendInfo (RestartReason::WATCHDOG_DISPATCHER_POINTER, ptr[2], ptr[1], ptr[0]);
	ptr = uint32_t(wdtPointer)*2;
	diagnostic_restart (RestartReason::WATCHDOG, ptr[2], ptr[1], ptr[0]);
}

// --------------------------------------------- main -------------------------------------------►

int main ()
{
	data.interruptHandler<DpsCommand> () = InterruptHandler::from_function<&commandParser>();
	data.interruptHandler<Club0> () = InterruptHandler::from_function<&kptCommandParse>();
	data.interruptHandler<BprVelocity> () = InterruptHandler::from_method<Emulation, &Emulation::getSautVelocity> (&emulation);
	canDat.rxHandler<CanRx::IPD_EMULATION>() = SoftIntHandler::from_method <Emulation, &Emulation::getCanVelocity>(&emulation);

	canDat.rxHandler<CanRx::INPUT_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanRx::MCO_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanRx::BKSI_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (&mph);
	canDat.rxHandler<CanTx::SYS_DATA_A>() = SoftIntHandler::from_method <MPHType, &MPHType::getLeftDataMessage> (&mph); // Если кто-то ещё ответит, то обновить мои данные
	canDat.rxHandler<CanRx::SYS_DATA_QUERY>() = SoftIntHandler::from_method <MPHType, &MPHType::getQueryMessage> (&mph);

	if (isSelfComplectA ()) // первый полукомплект
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
	
	// Причина перезагрузки
	diagnostic_storeDelegate = Delegate<void (uint8_t)>::from_function <&storeRestartReason> ();
	diagnostic_restoreDelegate = Delegate<uint8_t ()>::from_function <&restoreRestartReason> ();
	if (isSelfComplectA())
		diagnostic_sendMessageDelegate = AuxResourceMessage::from_method< CanDatType, &CanDatType::send<CanTx::AUX_RESOURCE_IPD_A> > (&canDat);
	else
		diagnostic_sendMessageDelegate = AuxResourceMessage::from_method< CanDatType, &CanDatType::send<CanTx::AUX_RESOURCE_IPD_B> > (&canDat);
	diagnostic_watchdogResetDelegate = Delegate<void ()>::from_function <&smartdog_reset> ();
	canDat.setBusOffHandler( Delegate<void ()>::from_function <&canBusOffHandler> () );
	dispatcher.overflowHandler = Delegate<void (uint16_t)>::from_function <&dispatcherOverflowHandler> ();
	scheduler.fullHandler = Delegate<void ()>::from_function <&schedulerFullHandler> ();
	programmingCan.reboot = Delegate<void ()>::from_function <&programmingRebootHandler> ();
	programming.reboot = Delegate<void ()>::from_function <&programmingRebootHandler> ();
	smartdog_deathAlarm = &smartdogAlarm;
	smartdog_on();

	sei();

	// После включения выдавать AUX_RESOURCE с версией
	{
		uint8_t packet[5];
		if ( programmingCan.isAvailable () )
		{
			packet[0] =	(uint8_t) 0;
			packet[1] =	(uint8_t) programmingCan.getVersion();
			packet[2] =	(uint8_t) programmingCan.getSubversion();
			packet[3] =	(uint8_t) (programmingCan.getCheckSum() & 0xFF);
			packet[4] =	(uint8_t) (programmingCan.getCheckSum() >> 8);
		}
		else
		{
			uint8_t idSize = pgm_read_byte(&id.idSize)*8; // Размер в словах
			uint16_t checkSumm = 0;
			for (uint8_t i = 0; i < idSize; i ++)
			checkSumm += pgm_read_word ((uint16_t *)&id + i);

			packet[0] =	(uint8_t) 0;
			packet[1] =	pgm_read_byte(&id.version);
			packet[2] =	0;
			packet[3] =	0;
			packet[4] =	uint8_t (checkSumm);
		}

		if (isSelfComplectA ())
		{
			while ( !canAsync.send<CanTx::AUX_RESOURCE_IPD_A>(packet) );
			while ( !canAsync.send<CanTx::AUX_RESOURCE_BS_A>(packet) );
		}
		else
		{
			while ( !canAsync.send<CanTx::AUX_RESOURCE_IPD_B>(packet) );
			while ( !canAsync.send<CanTx::AUX_RESOURCE_BS_B>(packet) );
		}
	}
	
	diagnostic_sendReasonOfPreviousRestart();

	scheduler.runIn( Command {SoftIntHandler::from_function<&unsetResetFlag>(), 0}, 7000 );
		
	dps.constituoActivus();

    for (;;)
    {
    	static bool resetButtonWasFree = false;
    	resetButtonWasFree |= reg.portB.pin5;
    	if ( resetButtonWasFree && !reg.portB.pin5 ) // Нажата кнопка сброса (а до этого была отпущена)
    	{
    		eeprom.dps0Good = 1;
    		eeprom.dps1Good = 1;
    		diagnostic_restart(RestartReason::BUTTON_PRESS);
    	}

    	dispatcher.invoke();
    	smartdog_reset();
    }
}


