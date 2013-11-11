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
#include "programming.h"
#include "CanDat.h"
#include "CanDesriptors.h"
#include "DiscreteInput.h"


void Init (void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void Init (void)
{
	// Светодиоды
	reg.portG.pin3.out();
	reg.portG.pin4.out();
	reg.portG.pin3 = true;
	reg.portG.pin4 = true;

	// Watchdog
	wdt_enable (WDTO_500MS);
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
typedef INT_TYPELIST_13 (CanTx::AUX_RESOURCE_BS_A,	CanTx::AUX_RESOURCE_BS_B,
						 CanTx::AUX_RESOURCE_IPD_A,	CanTx::AUX_RESOURCE_IPD_B,
						 CanTx::SYS_DATA_A, CanTx::SYS_DATA_B,
						 CanTx::MY_DEBUG_A, CanTx::MY_DEBUG_B,
						 CanTx::MY_KPT_A, CanTx::MY_KPT_B,
						 CanTx::IPD_PARAM_A, CanTx::IPD_PARAM_B,
						 CanTx::SYS_KEY ) AUX_RESOURCE_SYS_DATA_IPD_PARAM;
typedef INT_TYPELIST_2  (CanTx::VDS_STATE_A, CanTx::VDS_STATE_B) VDS_STATE;

typedef INT_TYPELIST_5 (CanRx::MCO_STATE_A, CanRx::MCO_STATE_B,
						CanRx::MCO_LIMITS_A, CanRx::MCO_LIMITS_B,
						CanRx::MCO_DATA) MCO;
typedef INT_TYPELIST_8 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A,
						CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS;
typedef INT_TYPELIST_2 (CanRx::MM_DATA, CanRx::MM_NEUTRAL) MM;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA_A) INPUT;
typedef INT_TYPELIST_3 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B) DIAGNOSTICS;

typedef CanDat < LOKI_TYPELIST_3(					// Список дескрипторов для отправки
						VDS_STATE,
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
			uint8_t packet[5] = {
					(uint8_t) Answer::VERSION,
					(uint8_t) programmingCan.getVersion(),
					(uint8_t) programmingCan.getSubversion(),
					(uint8_t) (programmingCan.getCheckSum() & 0xFF),
					(uint8_t) (programmingCan.getCheckSum() >> 8)
								};
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
//		else if ( request == Request::DIST_TRAVEL_WRITE )
//		{
//			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
//			eeprom_update_byte( adr  , canDat.get<CanRx::SYS_DIAGNOSTICS>() [5] );
//			eeprom_update_byte( adr+1, canDat.get<CanRx::SYS_DIAGNOSTICS>() [4] );
//			eeprom_update_byte( adr+2, canDat.get<CanRx::SYS_DIAGNOSTICS>() [3] );
//			eeprom_update_byte( adr+3, canDat.get<CanRx::SYS_DIAGNOSTICS>() [2] );
//		}
//		else if ( request == Request::DIST_TRAVEL_READ_A && isSelfComplectA () )
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
//		else if ( request == Request::DIST_TRAVEL_READ_B && !isSelfComplectA () )
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
			if (isSelfComplectA ())
				canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
			else
				canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
		}
	}
}

// ----------------------------------- Ввод дискретных сигналов ---------------------------------►

SoftIntHandler discreteInputA, discreteInputB;
void pushHandler (uint16_t num)
{
//	if ( num == 8 ) // РБ
//		canDat.send<CanTx::SYS_KEY> ({ (1 << 6) | 0x13 });
//	else if ( num == 9 ) // РБC
//		canDat.send<CanTx::SYS_KEY> ({ (1 << 6) | 0x1B });

	if ( num == 2 ) // Ж/Д ход
	{
//		dps.constituoRailWayRotae(true);
	}
	if ( num == 5 ) // Вперёд
	{
//		dps.constituoVersus (0);
	}
	if ( num == 4 ) // Назад
	{
//		dps.constituoVersus (1);
	}
}

void releaseHandler (uint16_t num)
{
//	if ( num == 8 ) // РБ
//		canDat.send<CanTx::SYS_KEY> ({ (2 << 6) | 0x13 });
//	else if ( num == 9 ) // РБC
//		canDat.send<CanTx::SYS_KEY> ({ (2 << 6) | 0x1B });

	if ( num == 2 ) // Ж/Д ход
	{
//		dps.constituoRailWayRotae(false);
	}
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

// --------------------------------------------- main -------------------------------------------►

int main ()
{
	asm volatile ("nop"); // !!! 126 version hack !!!
//	asm volatile ("nop"); // Для того чтобы сделать размер программы картным 6
//	asm volatile ("nop");
//	asm volatile ("nop");



	canDat.rxHandler<CanRx::SYS_DIAGNOSTICS>() = SoftIntHandler::from_function <&sysDiagnostics>();

	// ВДС
	inputSignalStateOut(0);

	sei();

	// После включения выдавать AUX_RESOURCE с версией
	{
		uint8_t packet[5] = {
				(uint8_t) 0,
				(uint8_t) programmingCan.getVersion(),
				(uint8_t) programmingCan.getSubversion(),
				(uint8_t) (programmingCan.getCheckSum() & 0xFF),
				(uint8_t) (programmingCan.getCheckSum() >> 8)
							};
		if (isSelfComplectA ())
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

    for (;;)
    {
    	dispatcher.invoke();
    	wdt_reset();
    	scheduler.invoke();
    	wdt_reset();
    }
}


