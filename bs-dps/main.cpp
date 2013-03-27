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
typedef INT_TYPELIST_12 (CanTx::AUX_RESOURCE_BS_A,	CanTx::AUX_RESOURCE_BS_B,
						 CanTx::AUX_RESOURCE_IPD_A,	CanTx::AUX_RESOURCE_IPD_B,
						 CanTx::SYS_DATA_A, CanTx::SYS_DATA_B,
						 CanTx::MY_DEBUG_A, CanTx::MY_DEBUG_B,
						 CanTx::MY_KPT_A, CanTx::MY_KPT_B,
						 CanTx::IPD_PARAM_A, CanTx::IPD_PARAM_B ) AUX_RESOURCE_SYS_DATA_IPD_PARAM;
//typedef INT_TYPELIST_2 	(CanTx::PROGRAM_SLAVE_CTRL, CanTx::PROGRAM_SLAVE_DATA) PROGRAM_SLAVE;
typedef INT_TYPELIST_2 	(CanTx::VDS_STATE_A, CanTx::VDS_STATE_B) VDS_SATE;

typedef INT_TYPELIST_5 (CanRx::MCO_STATE_A, CanRx::MCO_STATE_B,
						CanRx::MCO_LIMITS_A, CanRx::MCO_LIMITS_B,
						CanRx::MCO_DATA) MCO;
typedef INT_TYPELIST_8 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A,
						CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS;
typedef INT_TYPELIST_2 (CanRx::MM_DATA, CanRx::MM_NEUTRAL) MM;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA_A) INPUT;
typedef INT_TYPELIST_3 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B) DIAGNOSTICS;
//typedef INT_TYPELIST_2 (CanRx::PROGRAM_MASTER_CTRL, CanRx::PROGRAM_MASTER_DATA) PROGRAM_MASTER;

typedef CanDat < LOKI_TYPELIST_8(					// Список дескрипторов для отправки
						IPD_STATE,
						SAUT_INFO,
						SYS_DATA_STATE_IPD_NEUTRAL,
						SYS_DATA_STATE2,
						MPH_STATE,
						AUX_RESOURCE_SYS_DATA_IPD_PARAM,
						VDS_SATE,
						Int2Type <CanTx::SYS_KEY>
								),
				 LOKI_TYPELIST_7(
						 MCO,
						 Int2Type< CanRx::SYS_DATA_QUERY >,
						 MP_ALS,
						 MM,
						 INPUT,
						 DIAGNOSTICS,
						 Int2Type< CanRx::IPD_EMULATION >
//						 PROGRAM_MASTER
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
						 Int2Type< CanRx::IPD_EMULATION >,
						 Int2Type< CanRx::PROGRAM_MASTER_CTRL >,
						 Int2Type< CanRx::PROGRAM_MASTER_DATA >
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

//using namespace Saut;
//
//enum {		   					// adr, intput, port
//	// Входящие
//	DpsCommand 			= SautPacketHead (1, true, 0),
//	Dps0 				= SautPacketHead (1, true, 0),
//	DpsBandMeasLenght 	= SautPacketHead (1, true, 1),
//	Dps1 				= SautPacketHead (1, true, 1),
//	Dps2 				= SautPacketHead (1, true, 2),
//	Dps3 				= SautPacketHead (1, true, 3),
//	Club0				= SautPacketHead (10, true, 0),
//	BprQuery			= SautPacketHead (13, false, 0),
//	BprVelocity			= SautPacketHead (13, false, 3),
//	// Исходящие
//	DpsOut0 			= SautPacketHead (1, false, 0),
//	DpsOut1 			= SautPacketHead (1, false, 1),
//	DpsOut2 			= SautPacketHead (1, false, 2),
//	DpsOut3 			= SautPacketHead (1, false, 3),
//	ClubOut0			= SautPacketHead (10, false, 0),
//	ClubOut1			= SautPacketHead (10, false, 1)
//};
//
//typedef Dat <	INT_TYPELIST_7 (	// Данные для приёма
//						Dps0, Dps1, Dps2, Dps3, Club0, BprQuery, BprVelocity
//								),
//				INT_TYPELIST_6 (	// Данные для передачи
//						DpsOut0, DpsOut1, DpsOut2, DpsOut3, ClubOut0, ClubOut1
//								),
//				INT_TYPELIST_3 (	// Данные, по приходу которых вызываются прерывания
//						Dps0, Club0, BprVelocity
//								)
//			> DatType;
//DatType	data;
//
//typedef Com  <  &Register::usart1Control, &Register::usart1BaudRate, &Register::usart1Data,
//				 &Register::portD, 2, &Register::portD, 3, &Register::portD, 4, &Register::portB, 7,
//				 1,
//				 DatType, data
//			  > ComType;
//ComType com (USART1_RX_handler, USART1_TX_handler, USART1_UDRE_handler);

uint8_t sautDecimeters;
Safe<uint16_t> sautVelocity;
Safe<uint16_t> sautAcceleratio;


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

// ---------------------------------------------- КПТ -------------------------------------------►

InterruptHandler kptOdometerPluPlusHandler;

// ---------------------------------------------- ДПС -------------------------------------------►

typedef
CeleritasSpatiumDimetior  < &Register::portG, 3, 4, &Register::portA, 0,
							CanDatType, canDat,
							ClockType, clock,
							SchedulerType, scheduler >
DpsType;

DpsType	dps ( 	&Register::portC,
				sautDecimeters, sautVelocity, sautAcceleratio,
				kptOdometerPluPlusHandler,
				kptOdometerPluPlusHandler );

void unsetResetFlag (uint16_t)
{
	dps.repeto = false;
}

// --------------------------------------------- mcoState ---------------------------------------►


void mcoState (uint16_t pointer)
{
	typedef const uint8_t Message[8];
	Message& message = *( (Message *)(pointer) );

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
	if ( !(message[6] & (1 << 1) && message[7] & (1 << 6)) && //message[7] & (1 << ?) && 	// выход БС-ДПС или ИПД или ВДС
			clock.getTime() > 7000 && 	// проработали больше 7 секунд
			dps.sicinActivus() && // активность модуля ДПС говорит о том, что мы не в режиме программирования и т.д.
			!dps.sicinCausarius() ) // если оба датчика неисправны, то перезагрузку не делать
	{
		reboot ();
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
		reboot ();
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

//typedef ProgrammingCan <CanDatType, canDat, CanTx::PROGRAM_SLAVE_CTRL, CanTx::PROGRAM_SLAVE_DATA > ProgrammingCanType;
//ProgrammingCanType programmingCan (	Delegate<void ()>::from_method<DpsType, &DpsType::constituoActivus> (&dps),
//										Delegate<void ()>::from_method<DpsType, &DpsType::constituoPassivus> (&dps),
//										isSelfComplectA ()
//									);

// ----------------------------------- Ввод дискретных сигналов ---------------------------------►

SoftIntHandler discreteInputA, discreteInputB;
void pushHandler (uint16_t num)
{
	if ( num == 8 ) // РБ
		canDat.send<CanTx::SYS_KEY> ({ (1 << 5) | 0x13 });
	else if ( num == 9 ) // РБC
		canDat.send<CanTx::SYS_KEY> ({ (1 << 5) | 0x1B });
}

void releaseHandler (uint16_t num)
{
	if ( num == 8 ) // РБ
		canDat.send<CanTx::SYS_KEY> ({ (2 << 5) | 0x13 });
	else if ( num == 9 ) // РБC
		canDat.send<CanTx::SYS_KEY> ({ (2 << 5) | 0x1B });
}

typedef DiscreteInput<ClockType, clock> DiscreteInputType;
DiscreteInputType discreteInput ( !isSelfComplectA(), SoftIntHandler::from_function<&pushHandler>(), SoftIntHandler::from_function<&releaseHandler>() );

void inputSignalStateOut (uint16_t )
{
	struct OutMessageFields
	{
		uint8_t pnevmoMode				:1;
		uint8_t tifon					:1;
		uint8_t siren					:1;
		uint8_t emergencyStop			:1;
		uint8_t vigilanceButton			:1;
		uint8_t engineWork				:1;
		uint8_t forwardTransmission 	:1;
		uint8_t backwardTransmission	:1;
	};
	typedef Bitfield<OutMessageFields> OutMessage;
	OutMessage outMessage;

	DiscreteInputType::Inputs state = discreteInput.getState();

	outMessage.pnevmoMode = !state.in2; // Пневмо ход, если нет ЖД хода
	outMessage.tifon = state.in7;
	outMessage.siren = 0; // Сирена заведена на 27 вход ячейки, но она не заведена на процессор
	outMessage.emergencyStop = 0; // Аварйная остановка заведена на 26 вход ячейки, но не заведена на процессор
	outMessage.vigilanceButton = state.in8;
	outMessage.engineWork = !state.in6; // работа двигателя как инверсия сигнала "остановка двигателя"
	outMessage.forwardTransmission = state.in5;
	outMessage.backwardTransmission = state.in4;

	if ( isSelfComplectA() )
		canDat.send<CanTx::VDS_STATE_A> ({0, uint8_t(outMessage)});
	else
		canDat.send<CanTx::VDS_STATE_B> ({0, uint8_t(outMessage)});

	scheduler.runIn( Command {SoftIntHandler::from_function<&inputSignalStateOut>(), 0}, 500 );
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

//	// Программирование по CAN
//	canDat.rxHandler<CanRx::PROGRAM_MASTER_CTRL>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::getCommand> (&programmingCan);
//	canDat.rxHandler<CanRx::PROGRAM_MASTER_DATA>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::getData> (&programmingCan);
//	canDat.txHandler<CanTx::PROGRAM_SLAVE_DATA>() = SoftIntHandler::from_method <ProgrammingCanType, &ProgrammingCanType::sendData> (&programmingCan);

	// ВДС
	inputSignalStateOut(0);

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

	scheduler.runIn( Command {SoftIntHandler::from_function<&unsetResetFlag>(), 0}, 7000 );

    for (;;)
    {
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
//			if (isSelfComplectA ()) // первый полукомплект
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


