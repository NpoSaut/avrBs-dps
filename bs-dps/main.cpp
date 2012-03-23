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




void Init (void) __attribute__ ((naked)) __attribute__ ((section (".init5")));
void Init (void)
{
	// Светодиоды.
	reg.portC.pin4.out();
	reg.portC.pin5.out();
	reg.portC.pin4 = true;
	reg.portC.pin5 = true;

	// Watchdog
	wdt_enable (WDTO_500MS);

	// Кнопка сброса
	reg.portB.pin5.in();
}

// ----------------------------------- Системные часы и планировщик -----------------------------►

typedef Clock< Alarm< Alarm3A, 1000 >, uint32_t > ClockType;  // Обнуление через 50 суток
ClockType clock;

typedef Scheduler< ClockType, clock, 10, uint16_t > SchedulerType;
SchedulerType scheduler;

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
	Club1				= SautPacketHead (10, true, 1),
	BprQuery			= SautPacketHead (13, false, 0),
	BprVelocity			= SautPacketHead (13, false, 3),
	// Исходящие
	DpsOut0 			= SautPacketHead (1, false, 0),
	DpsOut1 			= SautPacketHead (1, false, 1),
	DpsOut2 			= SautPacketHead (1, false, 2),
	DpsOut3 			= SautPacketHead (1, false, 3),
	ClubOut0			= SautPacketHead (10, false, 0),
	ClubOut1			= SautPacketHead (10, false, 1),
	ClubOut2			= SautPacketHead (10, false, 2),
	ClubOut3			= SautPacketHead (10, false, 3)
};

typedef Dat <	INT_TYPELIST_8 (	// Данные для приёма
						Dps0, Dps1, Dps2, Dps3, Club0, Club1, BprQuery, BprVelocity
								),
				INT_TYPELIST_8 (	// Данные для передачи
						DpsOut0, DpsOut1, DpsOut2, DpsOut3, ClubOut0, ClubOut1, ClubOut2, ClubOut3
								),
				INT_TYPELIST_4 (	// Данные, по приходу которых вызываются прерывания
						Dps0, Club0, Club1, BprVelocity
								)
			> DatType;
DatType	data;

typedef Com  <  &Register::usart1Control, &Register::usart1BaudRate, &Register::usart1Data,
		&Register::portD, 2, &Register::portD, 3, &Register::portD, 4, &Register::portB, 7,
		1,
		DatType, data >
		ComType;
ComType	com (USART1_RX_handler, USART1_TX_handler, USART1_UDRE_handler);


// ---------------------------------------- Страницы БС-КЛУБ ------------------------------------►

uint8_t clubPage[5][3];

void clubSendNextPage (uint16_t none)
{
	static uint8_t pageNumber;
	ATOMIC
	{
		Complex<uint16_t> out3 { clubPage[pageNumber][2], clubPage[pageNumber][1] };
		Complex<uint16_t> out2 { clubPage[pageNumber][0],
								 ( uint8_t(data.member<ClubOut2>()/256) & 0xF ) | uint8_t(pageNumber * 16)
								};
		data.member<ClubOut2>() = out2;
		data.member<ClubOut3>() = out3;
	}
	if (++pageNumber == 5) { pageNumber = 0; } // Страницы 0-4
}

// Ставит в очередь dispatcher'а clubSendNextPage () - чтобы не загрузить частично заполненную страницу
void clubSendNextPageInterrupt ()
{
	dispatcher.add( SoftIntHandler::from_function<&clubSendNextPage>(), 0 );
//	dispatcher.add( SoftIntHandler (&clubSendNextPage), 0 );
}

// ---------------------------------------------- CAN -------------------------------------------►

typedef INT_TYPELIST_2 (CanTx::IPD_STATE_A,	CanTx::IPD_STATE_B) IPD_STATE;
typedef INT_TYPELIST_2 (CanTx::SAUT_INFO_A,	CanTx::SAUT_INFO_B) SAUT_INFO;
typedef INT_TYPELIST_9 (CanTx::AUX_RESOURCE_BS_A,	CanTx::AUX_RESOURCE_BS_B,
						CanTx::AUX_RESOURCE_IPD_A,	CanTx::AUX_RESOURCE_IPD_B,
						CanTx::SYS_DATA,
						CanTx::MY_DEBUG_A, CanTx::MY_DEBUG_B,
						CanTx::MY_KPT_A, CanTx::MY_KPT_B ) AUX_RESOURCE_SYS_DATA;

typedef INT_TYPELIST_2 (CanRx::MCO_STATE_A, CanRx::MCO_LIMITS_A) MCO_STATE_LIMITS_A;
typedef INT_TYPELIST_2 (CanRx::MCO_STATE_B, CanRx::MCO_LIMITS_B) MCO_STATE_LIMITS_B;
typedef INT_TYPELIST_2 (CanRx::SYS_DATA_QUERY, CanRx::SYS_KEY) SYS;
typedef INT_TYPELIST_4 (CanRx::MP_ALS_ON_A, CanRx::MP_ALS_OFF_A, CanRx::MP_ALS_ON_TIME_A, CanRx::MP_ALS_OFF_TIME_A) MP_ALS_A;
typedef INT_TYPELIST_4 (CanRx::MP_ALS_ON_B, CanRx::MP_ALS_OFF_B, CanRx::MP_ALS_ON_TIME_B, CanRx::MP_ALS_OFF_TIME_B) MP_ALS_B;
typedef INT_TYPELIST_3 (CanRx::BKSI_DATA, CanRx::INPUT_DATA, CanTx::SYS_DATA) INPUT;
typedef INT_TYPELIST_3 (CanRx::SYS_DIAGNOSTICS, CanRx::AUX_RESOURCE_MCO_A, CanRx::AUX_RESOURCE_MCO_B) DIAGNOSTICS;

typedef CanDat < LOKI_TYPELIST_5(					// Список дескрипторов для отправки
						IPD_STATE,
						SAUT_INFO,
						Int2Type< CanTx::SYS_DATA_STATE >,
						Int2Type< CanTx::SYS_DATA_STATE2 >,
						AUX_RESOURCE_SYS_DATA
								),
				 LOKI_TYPELIST_10(
						 Int2Type< CanRx::MCO_DATA >,
						 MCO_STATE_LIMITS_A,
						 MCO_STATE_LIMITS_B,
						 SYS,
						 MP_ALS_A,
						 MP_ALS_B,
						 Int2Type< CanRx::IPD_DATE >,
						 Int2Type< CanRx::MM_DATA >,
						 INPUT,
						 DIAGNOSTICS
						 	 	 ),
				 LOKI_TYPELIST_20(
						 Int2Type< CanRx::INPUT_DATA >,
						 Int2Type< CanRx::MCO_DATA >,
						 Int2Type< CanRx::BKSI_DATA >,
						 Int2Type< CanTx::SYS_DATA >,
						 Int2Type< CanRx::SYS_DATA_QUERY >,
						 Int2Type< CanRx::SYS_DIAGNOSTICS >,
						 Int2Type< CanRx::AUX_RESOURCE_MCO_A >,
						 Int2Type< CanRx::AUX_RESOURCE_MCO_B >,
						 Int2Type< CanRx::MCO_STATE_A >,
						 Int2Type< CanRx::MCO_STATE_B >,
						 Int2Type< CanRx::IPD_DATE >,
						 Int2Type< CanRx::MP_ALS_ON_A >,
						 Int2Type< CanRx::MP_ALS_OFF_A >,
						 Int2Type< CanRx::MP_ALS_ON_B >,
						 Int2Type< CanRx::MP_ALS_OFF_B >,
						 Int2Type< CanRx::MP_ALS_ON_TIME_A >,
						 Int2Type< CanRx::MP_ALS_OFF_TIME_A >,
						 Int2Type< CanRx::MP_ALS_ON_TIME_B >,
						 Int2Type< CanRx::MP_ALS_OFF_TIME_B >,
						 Int2Type< CanRx::SYS_KEY >
								),
				 100 >									// BaudRate = 100 Кбит, SamplePoint = 75% (по умолчанию)
	CanDatType;
CanDatType canDat;


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
			uint8_t checkSum = (( pgm_read_byte(&ID.crc) - pgm_read_byte(&ID.ks) ) >> 8) |
								( pgm_read_byte(&ID.crc) - pgm_read_byte(&ID.ks) );
			uint8_t packet[5] = {
					(uint8_t) Answer::VERSION,
					pgm_read_byte(&ID.version),
					0,
					0,
					checkSum
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
		else if ( request == Request::DIST_TRAVEL_WRITE )
		{
			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
			eeprom_update_byte( adr  , canDat.get<CanRx::SYS_DIAGNOSTICS>() [5] );
			eeprom_update_byte( adr+1, canDat.get<CanRx::SYS_DIAGNOSTICS>() [4] );
			eeprom_update_byte( adr+2, canDat.get<CanRx::SYS_DIAGNOSTICS>() [3] );
			eeprom_update_byte( adr+3, canDat.get<CanRx::SYS_DIAGNOSTICS>() [2] );
		}
		else if ( request == Request::DIST_TRAVEL_READ_A && reg.portB.pin7 == 0 )
		{
			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
			uint8_t packet[5] = {
					(uint8_t) Answer::DATA,
					eeprom_read_byte (adr+3),
					eeprom_read_byte (adr+2),
					eeprom_read_byte (adr+1),
					eeprom_read_byte (adr)
								};

			canDat.send<CanTx::AUX_RESOURCE_IPD_A> (packet);
		}
		else if ( request == Request::DIST_TRAVEL_READ_B && reg.portB.pin7 != 0 )
		{
			uint8_t* adr = (uint8_t *) &eeprom.club.milage;
			uint8_t packet[5] = {
					(uint8_t) Answer::DATA,
					eeprom_read_byte (adr+3),
					eeprom_read_byte (adr+2),
					eeprom_read_byte (adr+1),
					eeprom_read_byte (adr)
								};

			canDat.send<CanTx::AUX_RESOURCE_IPD_B> (packet);
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
			if (reg.portB.pin7 == 0)
				canDat.send<CanTx::AUX_RESOURCE_BS_A>(packet);
			else
				canDat.send<CanTx::AUX_RESOURCE_BS_B>(packet);
		}
	}
}

// ------------------------------------- Рукоятки: CAN -> RS-485 --------------------------------►

//void sysKeyRelease ()
//{
//	if (++sysKeyReleaseCounter == 46)
//	{
//		sysKeySelfControl.disable ();
//		ATOMIC	data.member<ClubOut2>() &= 0xF0FF; // Сбрасываем состояние кнопок
//		ATOMIC	data.member<ClubOut0>() &= ~(1 << 15); // Сбрасываем РБ
//		sysKeyReleaseCounter = 0;
//	}
//}

void sysKeyRelease (uint16_t bitN)
{
	ATOMIC data.member<ClubOut2>() &= ~(1 << bitN);
}
void sysKeyRbRelease (uint16_t)
{
	ATOMIC data.member<ClubOut0>() &= ~(1 << 15);
}

void sysKey (uint16_t a)
{
	enum class Key : uint8_t
	{
		RB 		= 0x13,
		BK		= 0x14,
		RBS		= 0x1B,
		RMP		= 0x16,
		ALARM	= 0x19,
		F		= 0x1C,
		PULL_UP	= 0x21,
		DEPART	= 0x25,
		OS		= 0x27,
		K20		= 0x29,
		RESET_1	= 0x30,
		RESET_2	= 0x31
	};

	if ( canDat.get<CanRx::SYS_KEY>()[0] & (1 << 6) ) // нажата кнопка (рукоятка)
	{
		switch ( (Key) (canDat.get<CanRx::SYS_KEY>()[0] & 0x3F) )
		{
			case Key::K20:
				ATOMIC data.member<ClubOut2>() |= (1 << 8);
				scheduler.runIn(
							Command{ SoftIntHandler::from_function<&sysKeyRelease>(), 8 },
//							Command{ SoftIntHandler( [](uint16_t a){ATOMIC data.member<ClubOut2>() &= ~(1 << 8); } ), 0},
							1000 );
				break;
			case Key::PULL_UP:
				ATOMIC data.member<ClubOut2>() |= (1 << 9);
				scheduler.runIn(
							Command{ SoftIntHandler::from_function<&sysKeyRelease>(), 9 },
//							Command{ SoftIntHandler( [](uint16_t a){ATOMIC data.member<ClubOut2>() &= ~(1 << 9); } ), 0},
							1000 );
				break;
			case Key::DEPART:
				ATOMIC data.member<ClubOut2>() |= (1 << 10);
				scheduler.runIn(
							Command{ SoftIntHandler::from_function<&sysKeyRelease>(), 10 },
//							Command{ SoftIntHandler( [](uint16_t a){ATOMIC data.member<ClubOut2>() &= ~(1 << 10); } ), 0},
							1000 );
				break;
			case Key::OS:
				ATOMIC data.member<ClubOut2>() |= (1 << 11);
				scheduler.runIn(
							Command{ SoftIntHandler::from_function<&sysKeyRelease>(), 11 },
//							Command{ SoftIntHandler( [](uint16_t a){ATOMIC data.member<ClubOut2>() &= ~(1 << 11); } ), 0},
							1000 );
				break;
		}
	}
}

// --------------------------------------------- ipdDate ----------------------------------------►

void ipdDate (uint16_t none)
{
	clubPage[2][0] = canDat.get<CanRx::IPD_DATE>()[4];
	clubPage[2][1] = canDat.get<CanRx::IPD_DATE>()[5];
	clubPage[2][2] = canDat.get<CanRx::IPD_DATE>()[6];
	clubPage[4][0] = canDat.get<CanRx::IPD_DATE>()[1];
	clubPage[4][1] = canDat.get<CanRx::IPD_DATE>()[2];
	clubPage[4][2] = canDat.get<CanRx::IPD_DATE>()[3];
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

uint16_t bandDiam (const uint8_t* avarage, const uint8_t* correction)
{
	uint16_t av = eeprom_read_byte (avarage);
	uint8_t cor = eeprom_read_byte (correction);
	if (cor & (1<<7)) // отрицательное число по понятиям САУТ
		return av * 10 - (cor & 0x7F);
	else
		return av * 10 + (cor & 0x7F);
}

typedef
CeleritasSpatiumDimetior  < &Register::portC, 4, 5, &Register::portB, 7,
							CanDatType, canDat,
							ClockType, clock,
							SchedulerType, scheduler >
DpsType;

DpsType	dps ( 	&Register::portC,
				com.decimeters, data.member<DpsOut0>(), data.member<DpsOut1>(),
				InterruptHandler::from_method <KptType, &KptType::lisPlusPlus> (&kpt),
				InterruptHandler::from_method <KptType, &KptType::correctKptDistancePlusPlus> (&kpt),
//				clubPage[1], clubPage[3],
				bandDiam (&eeprom.saut.DiameterAvarage, &eeprom.saut.DiameterCorrection[0]),
				bandDiam (&eeprom.saut.DiameterAvarage, &eeprom.saut.DiameterCorrection[1]) );


// --------------------------------------------- mcoState ---------------------------------------►

void mcoState (uint16_t pointer)
{
	typedef const uint8_t Message[8];
	Message& message = *( (Message *)(pointer) );

	// Передача данных в страницы БС-КЛУБ
	clubPage[0][0] = message[2]; // целевая скорость
	clubPage[0][1] = message[1]; // допустимая скорость
	clubPage[0][2] = (message[0] & 0x4F) | // целевая скорость и ключь ЭПК 1-ой кабины
					 ((message[3] & 0xC0) >> 2);

	clubPage[1][1] |= message[3] & 0xF;	// Расстояние до цели
	clubPage[1][2] = message[4];		//

	// Передача сигналов КЛУБ
	uint8_t signals = _cast( Complex<uint16_t>, data.member<ClubOut0>() )[1] & 0x80; // Сохраняем текущее значение РБ
	static uint8_t convertAls[16] = {0x10,0x08,0x04,0x02,0x01,0x08,0x00,0x00,0x00,0x08,0x04,0x02,0x01,0x01,0x01,0x01};
	signals |= convertAls[ message[5]&0xF ] | ( message[5] & (1 << 5) ); // Огни АЛС и ЭПК
	if ( message[5] & ((1 << 6) | (1 << 7)) ) // РБ или РБС
	{
		signals |= (1 << 7); // нажимаем РБ
		scheduler.runIn(
					Command{ SoftIntHandler::from_function<&sysKeyRbRelease>(), 0 },
//					Command{ SoftIntHandler( [](uint16_t a){ ATOMIC	data.member<ClubOut0>() &= ~(1 << 15); } ), 0},
					1000 );
//		sysKeyReleaseCounter = 0;
//		sysKeySelfControl.enable (); // через секунду отпускаем
	}
	_cast( Complex<uint16_t>, data.member<ClubOut0>() )[1] = signals;

	// Определение, есть ли тяга
	dps.tractus = !(message[0] & (1 << 5));

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

// --------------------------------------- Эмуляция движения ------------------------------------►

class Emulation
{
public:
	Emulation ()
		: engine ( 0x5555, InterruptHandler::from_method<Emulation, &Emulation::makeAStep>(this) )
//		: engine ( 0x5555, InterruptHandler(this, &Emulation::makeAStep) )
	{
		scheduler.runIn( Command{ SoftIntHandler::from_method<Emulation, &Emulation::watchDog>(this), 0 }, 1000 );
//		scheduler.runIn( Command{ SoftIntHandler(this, &Emulation::watchDog), 0 }, 1000 );
	}

	void getVelocity ()
	{
		uint8_t newVelocity = _cast( Complex<uint16_t>, data.member<BprVelocity>() )[1];
		if ( _cast( Complex<uint16_t>, data.member<BprQuery>() )[1] & (1 << 1) &&  // Команда на эмуляцию
				newVelocity >= 2 ) // Меньше 8-битный таймер не позволяет
		{
			enable();
			if ( newVelocity != currentVelocity )
			{
				currentVelocity = newVelocity;

				uint32_t period = uint32_t(67320) * dps.diametros(0) / 1000 / newVelocity;
				if (period > 150) // Чтобы не повесить систему слишком частым заходом
					engine.setPeriod ( period );
			}
		}
		else
			disable ();

		getMessage = true;
	}

private:
	void watchDog (uint16_t)
	{
		if ( !( _cast( Complex<uint16_t>, data.member<BprQuery>() )[1] & (1 << 1) ) ||
				getMessage == false )
			disable ();

		getMessage = false;
		scheduler.runIn( Command{ SoftIntHandler::from_method<Emulation, &Emulation::watchDog>(this), 0 }, 1000 );
//		scheduler.runIn( Command{ SoftIntHandler(this, &Emulation::watchDog), 0 }, 1000 );
	}
	void makeAStep ()
	{
		if (parity)
		{
			toggle (1);
			toggle (2);
		}
		else
		{
			toggle (0);
			toggle (3);
		}
		parity = !parity;
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
	uint8_t currentVelocity;
	bool getMessage;
	bool parity;
};
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

	if (command->block3Byte)
		com.block3Byte = 1;

	if (!command->idRead && !command->eepromRead)	// стандартный режим работы ДПС
	{
		data.member<DpsOut2>() = dps.diametros(0); // выводим диаметры бандажа
		data.member<DpsOut3>() = dps.diametros(1);

		dps.constituoActivus ();
	}
	else
	{
		dps.constituoPassivus ();

		if (command->idRead)
		{
			data.member<DpsOut0>() = ( (uint16_t) pgm_read_byte(&ID.version) << 8 ) | pgm_read_byte(&ID.year);
			data.member<DpsOut1>() = ( (uint16_t) pgm_read_byte(&ID.modif)   << 8 ) | pgm_read_byte(&ID.manth);
			data.member<DpsOut2>() = ( ( pgm_read_word(&ID.number) & 0xFF)   << 8 ) | ( pgm_read_word(&ID.number) /256 );
			data.member<DpsOut3>() = ( ( pgm_read_word(&ID.ks)	   & 0xFF)   << 8 ) | ( pgm_read_word(&ID.ks)     /256 );
		}

		if (command->eepromRead)
		{
			uint16_t *Adr = (uint16_t *) ((data.member<DpsCommand>() & 0x00ff)*8);// Адрес из линии RS-485 * 8
			data.member<DpsOut0>() = swap( eeprom_read_word (Adr++) );
			data.member<DpsOut1>() = swap( eeprom_read_word (Adr++) );
			data.member<DpsOut2>() = swap( eeprom_read_word (Adr++) );
			data.member<DpsOut3>() = swap( eeprom_read_word (Adr++) );
		}
	}

	if (data.member<DpsCommand>() == 0x0efe)			// Переход в режим программирования
	{
		dps.constituoPassivus ();

		Programming* P1 = new Programming (
				data.member<Dps0>(),	data.member<Dps1>(),	data.member<Dps2>(),	data.member<Dps3>(),
				data.member<DpsOut0>(),	data.member<DpsOut1>(),	data.member<DpsOut2>(),	data.member<DpsOut3>() );
		data.interruptHandler<Dps0> () = InterruptHandler::from_method <Programming, &Programming::comParser> (P1);
//		data.interruptHandler<Dps0> () = InterruptHandler( P1, &Programming::comParser);
	}
}

void dispatcherSizeReset (uint16_t)
{
	dispatcher.maxSize = 0;
	scheduler.runIn( Command {SoftIntHandler::from_function<&dispatcherSizeReset>(), 0},	10000 );
//	scheduler.runIn( Command {&dispatcherSizeReset, 0},	10000 );
}

void unsetResetFlag (uint16_t)
{
	dps.repeto = false;
}

// --------------------------------------------- main -------------------------------------------►

int main ()
{
//	data.interruptHandler<DpsCommand> () = InterruptHandler (&commandParser);
//	data.interruptHandler<Club0> () = InterruptHandler (&kptCommandParse);
//	data.interruptHandler<Club1> () = InterruptHandler (&clubSendNextPageInterrupt);
//	data.interruptHandler<BprVelocity> () = InterruptHandler (&emulation, &Emulation::getVelocity);
//
//	canDat.rxHandler<CanRx::INPUT_DATA>() = SoftIntHandler (&canDataGet);
//	canDat.rxHandler<CanRx::MCO_DATA>() = SoftIntHandler (&canDataGet);
//	canDat.rxHandler<CanRx::BKSI_DATA>() = SoftIntHandler (&canDataGet);
//	canDat.rxHandler<CanRx::SYS_DATA_QUERY>() = SoftIntHandler (&canDataSend);
//
//	canDat.rxHandler<CanRx::MM_DATA>() = SoftIntHandler (&eCardParser);
//
//	canDat.rxHandler<CanRx::SYS_DIAGNOSTICS>() = SoftIntHandler (&sysDiagnostics);
//
//	canDat.rxHandler<CanRx::MCO_STATE_A>() = SoftIntHandler (&mcoStateA);
//	canDat.rxHandler<CanRx::MCO_STATE_B>() = SoftIntHandler (&mcoStateB);
//
//	canDat.rxHandler<CanRx::IPD_DATE>() = SoftIntHandler (&ipdDate);
//	canDat.rxHandler<CanRx::SYS_KEY>() = SoftIntHandler (&sysKey);
//
//	canDat.rxHandler<CanRx::MP_ALS_ON_A>() = SoftIntHandler (&kptRiseA);
//	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_A>() = SoftIntHandler (&kptRiseTimeA);
//	canDat.rxHandler<CanRx::MP_ALS_OFF_A>() = SoftIntHandler (&kptFallA);
//	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_A>() = SoftIntHandler (&kptFallTimeA);
//	canDat.rxHandler<CanRx::MP_ALS_ON_B>() = SoftIntHandler (&kptRiseB);
//	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_B>() = SoftIntHandler (&kptRiseTimeB);
//	canDat.rxHandler<CanRx::MP_ALS_OFF_B>() = SoftIntHandler (&kptFallB);
//	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_B>() = SoftIntHandler (&kptFallTimeB);

	data.interruptHandler<DpsCommand> () = InterruptHandler::from_function<&commandParser>();
	data.interruptHandler<Club0> () = InterruptHandler::from_function<&kptCommandParse>();
	data.interruptHandler<Club1> () = InterruptHandler::from_function<&clubSendNextPageInterrupt>();
	data.interruptHandler<BprVelocity> () = InterruptHandler::from_method<Emulation, &Emulation::getVelocity> (&emulation);

	// ------------------------------ Хранение постоянных характеристик -----------------------------►
	if (reg.portB.pin7 == 0) // первый полукомплект
	{
		typedef MPH <CanDatType, canDat, SchedulerType, scheduler, ComType, com> MPHType;
		MPHType mph;

		canDat.rxHandler<CanRx::INPUT_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::writeConfirm> (&mph);
		canDat.rxHandler<CanRx::MCO_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::writeConfirm> (&mph);
		canDat.rxHandler<CanRx::BKSI_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::writeConfirm> (&mph);
		canDat.rxHandler<CanTx::SYS_DATA>() = SoftIntHandler::from_method <MPHType, &MPHType::write> (&mph); // Если кто-то ещё ответит, то обновить мои данные
		canDat.rxHandler<CanRx::SYS_DATA_QUERY>() = SoftIntHandler::from_method <MPHType, &MPHType::read> (&mph);
	}

	canDat.rxHandler<CanRx::SYS_DIAGNOSTICS>() = SoftIntHandler::from_function <&sysDiagnostics>();

	canDat.rxHandler<CanRx::MCO_STATE_A>() = SoftIntHandler::from_function <&mcoStateA>();
	canDat.rxHandler<CanRx::MCO_STATE_B>() = SoftIntHandler::from_function <&mcoStateB>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_A>() = SoftIntHandler::from_function <&mcoAuxResA>();
	canDat.rxHandler<CanRx::AUX_RESOURCE_MCO_B>() = SoftIntHandler::from_function <&mcoAuxResB>();

	canDat.rxHandler<CanRx::IPD_DATE>() = SoftIntHandler::from_function <&ipdDate>();
	canDat.rxHandler<CanRx::SYS_KEY>() = SoftIntHandler::from_function <&sysKey>();

	canDat.rxHandler<CanRx::MP_ALS_ON_A>() = SoftIntHandler::from_function <&kptRiseA>();
	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_A>() = SoftIntHandler::from_function <&kptRiseTimeA>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_A>() = SoftIntHandler::from_function <&kptFallA>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_A>() = SoftIntHandler::from_function <&kptFallTimeA>();
	canDat.rxHandler<CanRx::MP_ALS_ON_B>() = SoftIntHandler::from_function <&kptRiseB>();
	canDat.rxHandler<CanRx::MP_ALS_ON_TIME_B>() = SoftIntHandler::from_function <&kptRiseTimeB>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_B>() = SoftIntHandler::from_function <&kptFallB>();
	canDat.rxHandler<CanRx::MP_ALS_OFF_TIME_B>() = SoftIntHandler::from_function <&kptFallTimeB>();

	dps.constituoActivus();

	sei();

	// После включения выдавать AUX_RESOURCE с версией
	{
		uint8_t checkSum = (( pgm_read_byte(&ID.crc) - pgm_read_byte(&ID.ks) ) >> 8) |
							( pgm_read_byte(&ID.crc) - pgm_read_byte(&ID.ks) );
		uint8_t packet[5] = {
				0,
				pgm_read_byte(&ID.version),
				0,
				0,
				checkSum
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

	scheduler.runIn( Command {SoftIntHandler::from_function<&dispatcherSizeReset>(), 0},	10000 );
//	scheduler.runIn( Command {&dispatcherSizeReset, 0},	10000 );
	scheduler.runIn( Command {SoftIntHandler::from_function<&unsetResetFlag>(), 0}, 7000 );

    for (;;)
    {
    	static bool resetButtonWasFree = false;
    	resetButtonWasFree |= reg.portB.pin5;
    	if ( resetButtonWasFree && !reg.portB.pin5 ) // Нажата кнопка перезагрузки (а до этого была отпущена)
    		reboot();

    	dispatcher.invoke();
    	wdt_reset();
    	scheduler.invoke();
    	wdt_reset();

//    	asm volatile ("nop");
    }
}


