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
#include "DiscreteInput.h"
#include "diagnostic.h"

void Init (void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void Init (void)
{
	// Светодиоды.
	reg.portG.pin3.out();
	reg.portG.pin4.out();
	reg.portG.pin3 = true;
	reg.portG.pin4 = true;
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
typedef INT_TYPELIST_18 (CanRx::SYS_DIAGNOSTICS,
						 CanTx::AUX_RESOURCE_BS_A,	CanTx::AUX_RESOURCE_BS_B,
						 CanTx::AUX_RESOURCE_IPD_A,	CanTx::AUX_RESOURCE_IPD_B,
						 CanTx::AUX_RESOURCE_VDS_A,	CanTx::AUX_RESOURCE_VDS_B,
						 CanTx::SYS_DATA_A, CanTx::SYS_DATA_B,
						 CanTx::MY_DEBUG_A, CanTx::MY_DEBUG_B,
						 CanTx::MY_KPT_A, CanTx::MY_KPT_B,
						 CanTx::IPD_PARAM_A, CanTx::IPD_PARAM_B,
						 CanTx::SYS_KEY,
						 CanTx::VDS_STATE_A, CanTx::VDS_STATE_B
						  ) AUX_RESOURCE_SYS_DATA_IPD_PARAM;

typedef INT_TYPELIST_5 (CanRx::MCO_STATE_A, CanRx::MCO_STATE_B,
						CanRx::MCO_LIMITS_A, CanRx::MCO_LIMITS_B,
						CanRx::MCO_DATA) MCO;
typedef INT_TYPELIST_8 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A,
						CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS;
typedef INT_TYPELIST_2 (CanRx::MM_DATA, CanRx::MM_NEUTRAL) MM;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA_A) INPUT;
typedef INT_TYPELIST_5 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B, CanTx::AUX_RESOURCE_BS_A, CanTx::AUX_RESOURCE_BS_B) DIAGNOSTICS;

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
				 LOKI_TYPELIST_24(
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
						 Int2Type< CanRx::FU_INIT >,
						 Int2Type< CanTx::AUX_RESOURCE_BS_A >,
						 Int2Type< CanTx::AUX_RESOURCE_BS_B >
								),
					128,
					NullType,
				 100 >									// BaudRate = 100 Кбит, SamplePoint = 75% (по умолчанию)
	CanDatType;
CanDatType canDat;

typedef CanAsync<CanDatType, canDat, Dispatcher, dispatcher, 16> CanAsyncType;
CanAsyncType canAsync;

// -------------------------------------------- RS-485 ------------------------------------------►

uint8_t sautDecimeters;
Safe<uint16_t> sautVelocity;
Safe<uint16_t> sautAcceleratio;

// ------------------------------- Работа с загрузчиком-программатором --------------------------►

typedef ProgrammingCan <CanDatType, canDat, CanRx::FU_INIT, CanTx::FU_DEV > ProgrammingCanType;
ProgrammingCanType programmingCan;

// ---------------------------------------------- КПТ -------------------------------------------►

InterruptHandler kptOdometerPluPlusHandler;

// ---------------------------------------------- ДПС -------------------------------------------►

typedef
CeleritasSpatiumDimetior  < CanDatType, canDat,
ClockType, clock,
SchedulerType, scheduler >
DpsType;

DpsType *dps = 0;

void unsetResetFlag (uint16_t)
{
	if (dps)
		dps->repeto = false;
}

// --------------------------------- Модуль постоянных характеристик ----------------------------►

typedef ConstValModule <CanAsyncType, canAsync, SchedulerType, scheduler> MPHType;
MPHType *mph = 0;

// ---------------------------------- SYS_DIAGNOSTICS / AUX_RESOURCE ----------------------------►

enum class SysDiagnosticsUnit : uint8_t
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
	EPK,
	VDS = 30
};

enum class SysDiagnosticsRequest : uint8_t
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

enum class SysDiagnosticsAnswer : uint8_t
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
	
void sysDiagnostics (uint16_t a)
{
	SysDiagnosticsUnit unit = (SysDiagnosticsUnit) canDat.get<CanRx::SYS_DIAGNOSTICS>() [0];
	SysDiagnosticsRequest request = (SysDiagnosticsRequest) canDat.get<CanRx::SYS_DIAGNOSTICS>() [1];


	if ( (dps && (unit == SysDiagnosticsUnit::IPD || unit == SysDiagnosticsUnit::BS_DPS)) || unit == SysDiagnosticsUnit::VDS)
	{
		if ( request == SysDiagnosticsRequest::VERSION  )
		{
			uint8_t packet[5];
			if ( programmingCan.isAvailable () )
			{
				packet[0] =	(uint8_t) SysDiagnosticsAnswer::VERSION;
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

				packet[0] =	(uint8_t) SysDiagnosticsAnswer::VERSION;
				packet[1] =	pgm_read_byte(&id.version);
				packet[2] =	0;
				packet[3] =	0;
				packet[4] =	uint8_t (checkSumm);
			}

			if (unit == SysDiagnosticsUnit::IPD)
			{
				if (isSelfComplectA ())
					canDat.send<CanTx::AUX_RESOURCE_IPD_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_IPD_B>(packet);
			}
			else if (unit == SysDiagnosticsUnit::BS_DPS)
			{
				if (isSelfComplectA ())
					canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
			}
			else if (unit == SysDiagnosticsUnit::VDS)
			{
				if (isSelfComplectA ())
					canDat.send<CanTx::AUX_RESOURCE_VDS_A>(packet);
				else
					canDat.send<CanTx::AUX_RESOURCE_VDS_B>(packet);
			}
		}
		else if ( request == SysDiagnosticsRequest::TEST_RUN && unit == SysDiagnosticsUnit::BS_DPS )
		{
			uint8_t packet[5] = {
					(uint8_t) SysDiagnosticsAnswer::DATA,
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

// --------------------------------------------- mcoState ---------------------------------------►

void mcoState (uint16_t pointer)
{
	typedef const uint8_t Message[8];
	Message& message = *( (Message *)(pointer) );

	if (dps)
	{
		// Определение, есть ли тяга
		if ( message[0] & (1 << 5) )
			dps->constituoNonTractus ();
		else
			dps->constituoTractus ();

		// В задней кабине инвертировать направление, если это не маневровый
		uint32_t locoType = 0;
		if ( eeprom.club.property.category.read (locoType) )
			if ( locoType != 7 ) // не маневровый
				dps->constituoVersusInversio ( message[3] & (1 << 5) );
				
		// Контроль выхода из конфигурации
		if ( clock.getTime() > 7000 && 	// проработали больше 7 секунд
			!dps->sicinBothBrake() ) // если оба датчика неисправны, то перезагрузку не делать
		{
			if( !(message[6] & (1 << 1)) )
				diagnostic_restart(RestartReason::IPD_OUT);
			if( !(message[7] & (1 << 6)) )
				diagnostic_restart(RestartReason::DPS_OUT);
		}
	}

	// Контроль перезагрузки ЦО.
	static uint32_t lastTime = 0;
	uint32_t time = clock.getTime();
	if ( time - lastTime > 4000 )
		diagnostic_restart(RestartReason::CO_LOST);
	lastTime = time;

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

	if (mph)
	{
		if ( message[0] == 2 && message[1] == 3 && !(message[2] & (1 << 2)) && // Кодовая комбинация, означающая выход МПХ из конфиграции
			clock.getTime() > 7000 ) 	// проработали больше 7 секунд
		{
			diagnostic_restart(RestartReason::MPH_OUT);
		}
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
			currentOperationSequence[i] *= 2;
		}

		// Устанавливает период таймера, генерируещего импульсы
		uint32_t period = uint32_t(4000000) / current.sensor[0].freqX16;
		if (period > 150) // Чтобы не повесить систему слишком частым заходом
			engine.setPeriod ( period );

		// Натравливаем БС-ДПС
		if (dps)
			dps->accessusPortus = (Port Register::*) (&Register::general0);

		// Поехали
		engine.enable();
	}
	void disable ()
	{
		if (dps)
			dps->accessusPortus = &Register::portC;
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

// ----------------------------------- Ввод дискретных сигналов ---------------------------------►

SoftIntHandler discreteInputA, discreteInputB;
void pushHandler (uint16_t num)
{
//	if ( num == 8 ) // РБ
//		canDat.send<CanTx::SYS_KEY> ({ (1 << 6) | 0x13 });
//	else if ( num == 9 ) // РБC
//		canDat.send<CanTx::SYS_KEY> ({ (1 << 6) | 0x1B });

	if ( num == 5 ) // Вперёд
	{
		dps->constituoVersus (0);
	}
	if ( num == 4 ) // Назад
	{
		dps->constituoVersus (1);
	}
}

void releaseHandler (uint16_t num)
{
//	if ( num == 8 ) // РБ
//		canDat.send<CanTx::SYS_KEY> ({ (2 << 6) | 0x13 });
//	else if ( num == 9 ) // РБC
//		canDat.send<CanTx::SYS_KEY> ({ (2 << 6) | 0x1B });
}

typedef DiscreteInput<ClockType, clock> DiscreteInputType;
DiscreteInputType discreteInput ( !isSelfComplectA(), SoftIntHandler::from_function<&pushHandler>(), SoftIntHandler::from_function<&releaseHandler>() );

void inputSignalStateOut (uint16_t )
{
	struct OutMessageFields
	{
		uint16_t railwayMode			:1;
		uint16_t tifon					:1;
		uint16_t siren					:1;
		uint16_t emergencyStop			:1;
		uint16_t vigilanceButton		:1;
		uint16_t engineWork				:1;
		uint16_t forwardTransmission 	:1;
		uint16_t backwardTransmission	:1;
		uint16_t tractionDisable		:1;
		uint16_t						:1;
		uint16_t epkKey					:1;
		uint16_t 						:5;
	};
	typedef Bitfield<OutMessageFields> OutMessage;
	OutMessage outMessage;

	DiscreteInputType::Inputs state = discreteInput.getState();

	outMessage.railwayMode = state.in2;
	outMessage.tifon = state.in7;
	outMessage.siren = 0; // Сирена заведена на 27 вход ячейки, но она не заведена на процессор
	outMessage.emergencyStop = state.in6;
	outMessage.vigilanceButton = state.in8;
	outMessage.engineWork = !state.in6; // работа двигателя как инверсия сигнала "остановка двигателя"
	outMessage.forwardTransmission = state.in5;
	outMessage.backwardTransmission = state.in4;
	outMessage.tractionDisable = (state.in5 == 0) && (state.in4 == 0);
	outMessage.epkKey = 1;

	if ( isSelfComplectA() )
		canDat.send<CanTx::VDS_STATE_A> ({uint8_t(outMessage/256), uint8_t(outMessage)});
	else
		canDat.send<CanTx::VDS_STATE_B> ({uint8_t(outMessage/256), uint8_t(outMessage)});

	scheduler.runIn( Command {SoftIntHandler::from_function<&inputSignalStateOut>(), 0}, 500 );
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

// ----------------------------------------- Поиск БС-ДПС ----------------------------------------►

uint8_t startDpsAndMphDelayProcess = 0;

void requestBsDps ()
{
	uint8_t packet [7] = {
		(uint8_t) SysDiagnosticsUnit::BS_DPS,
		(uint8_t) SysDiagnosticsRequest::VERSION,
		0, 0, 0, 0, 0
	};
	while ( !canAsync.send<CanRx::SYS_DIAGNOSTICS>(packet) );
}

void catchBsDpsVersion (uint16_t )
{
	scheduler.deleteId (startDpsAndMphDelayProcess);
}

void startDpsAndMph (uint16_t )
{
	dps = new DpsType ( Delegate<void (bool)>::from_function< &lset<0> >(), Delegate<void (bool)>::from_function< &lset<1> >(), isSelfComplectA(),
		&Register::portC,
		sautDecimeters, sautVelocity, sautAcceleratio,
		kptOdometerPluPlusHandler,
		kptOdometerPluPlusHandler );
	canDat.rxHandler<CanRx::MM_DATA>() = SoftIntHandler::from_method <DpsType, &DpsType::takeEcDataForAdjust> (dps);
			
	mph = new MPHType ();
	canDat.rxHandler<CanRx::INPUT_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (mph);
	canDat.rxHandler<CanRx::MCO_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (mph);
	canDat.rxHandler<CanRx::BKSI_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::getWriteMessage> (mph);
	canDat.rxHandler<CanTx::SYS_DATA_A>() = SoftIntHandler::from_method <MPHType, &MPHType::getLeftDataMessage> (mph); // Если кто-то ещё ответит, то обновить мои данные
	canDat.rxHandler<CanRx::SYS_DATA_QUERY>() = SoftIntHandler::from_method <MPHType, &MPHType::getQueryMessage> (mph);	
}

// --------------------------------------------- main -------------------------------------------►

int main ()
{	
	// Поиск БС-ДПС
	if ( isSelfComplectA() )
		canDat.rxHandler<CanTx::AUX_RESOURCE_BS_A>() = SoftIntHandler::from_function <&catchBsDpsVersion> ();
	else
		canDat.rxHandler<CanTx::AUX_RESOURCE_BS_B>() = SoftIntHandler::from_function <&catchBsDpsVersion> ();
	requestBsDps ();
	startDpsAndMphDelayProcess = scheduler.runIn( Command {SoftIntHandler::from_function<&startDpsAndMph>(), 0}, 1000 );
	
	canDat.rxHandler<CanRx::IPD_EMULATION>() = SoftIntHandler::from_method <Emulation, &Emulation::getCanVelocity>(&emulation);

	canDat.rxHandler<CanRx::SYS_DIAGNOSTICS>() = SoftIntHandler::from_function <&sysDiagnostics>();

	canDat.rxHandler<CanRx::MCO_STATE_A>() = SoftIntHandler::from_function <&mcoStateA>();
	canDat.rxHandler<CanRx::MCO_STATE_B>() = SoftIntHandler::from_function <&mcoStateB>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_A>() = SoftIntHandler::from_function <&mcoAuxResA>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_B>() = SoftIntHandler::from_function <&mcoAuxResB>();

	// ВДС
	inputSignalStateOut(0);

	// Причина перезагрузки
	diagnostic_storeDelegate = Delegate<void (uint8_t)>::from_function <&storeRestartReason> ();
	diagnostic_restoreDelegate = Delegate<uint8_t ()>::from_function <&restoreRestartReason> ();
	if (isSelfComplectA())
		diagnostic_sendMessageDelegate = AuxResourceMessage::from_method< CanDatType, &CanDatType::send<CanTx::AUX_RESOURCE_VDS_A> > (&canDat);
	else
		diagnostic_sendMessageDelegate = AuxResourceMessage::from_method< CanDatType, &CanDatType::send<CanTx::AUX_RESOURCE_VDS_B> > (&canDat);
	diagnostic_watchdogResetDelegate = Delegate<void ()>::from_function <&smartdog_reset> ();
	canDat.setBusOffHandler( Delegate<void ()>::from_function <&canBusOffHandler> () );
	dispatcher.overflowHandler = Delegate<void (uint16_t)>::from_function <&dispatcherOverflowHandler> ();
	scheduler.fullHandler = Delegate<void ()>::from_function <&schedulerFullHandler> ();
	programmingCan.reboot = Delegate<void ()>::from_function <&programmingRebootHandler> ();
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
			if (dps)
				while ( !canAsync.send<CanTx::AUX_RESOURCE_IPD_A>(packet) );
			while ( !canAsync.send<CanTx::AUX_RESOURCE_VDS_A>(packet) );
		}
		else
		{
			if (dps)
				while ( !canAsync.send<CanTx::AUX_RESOURCE_IPD_B>(packet) );
			while ( !canAsync.send<CanTx::AUX_RESOURCE_VDS_B>(packet) );
		}
	}
	
	diagnostic_sendReasonOfPreviousRestart();

	scheduler.runIn( Command {SoftIntHandler::from_function<&unsetResetFlag>(), 0}, 7000 );
	
	if (dps)	
		dps->constituoActivus();

    for (;;)
    {
    	dispatcher.invoke();
    	smartdog_reset();
    }
}


