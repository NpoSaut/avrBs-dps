/*
 * mph.h
 *
 * Модуль постоянных характеристик
 * *******************************
 *
 *  Created on: 13.10.2011
 *      Author: nazemnyh
 */

#ifndef MPH_H_
#define MPH_H_

#include <cpp/universal.h>
#include <cpp/eeprom.h>
#include "CanDat.h"
#include "CanDesriptors.h"

// ------------------------------------------- Ячейка -------------------------------------------►

namespace EeCellStaticPrivate
{
	struct StatusField
	{
		struct
		{
			uint8_t	unWritten	:1; // 0 - записано, потому что изначально после стирания в eeprom все биты = 1
			uint8_t	crc7		:7;
		};
	};
	typedef Bitfield<StatusField> Status;

	bool activeWrite = false;
	Complex<uint32_t> data;
	Status status;
	SoftIntHandler afterWrite;
	SoftIntHandler isGoodResultGetter;
	SoftIntHandler isWrittenResultGetter;
}

class EeCell
{
public:
	bool write( const uint32_t& value, const SoftIntHandler& runAfterWriteEnd = SoftIntHandler() );
	bool read ( uint32_t& value );
	void isGood ( const SoftIntHandler& resultGetter );
	void isWritten ( const SoftIntHandler& resultGetter );

private:
	uint8_t crc7x2 (uint8_t crcx2, uint8_t data)
	{
		crcx2 ^= data;
	    for (uint8_t i = 0; i < 8; i++)
	        crcx2 = crcx2 & 0x80 ? (crcx2 << 1) ^ (0x37 * 2) : crcx2 << 1;
		return crcx2;
	}

	void writeData (uint16_t);
	void writeCrc (uint16_t);
	void lastControl (uint16_t);
	void goodDelayedRequest (uint16_t);
	void writtenDelayedRequest (uint16_t);

	Eeprom< EeCellStaticPrivate::Status > status;
	Eeprom<uint32_t> data;
};


bool EeCell::write (const uint32_t& value, const SoftIntHandler& runAfterWrite)
{
	using namespace EeCellStaticPrivate;
	if ( !activeWrite )
	{
		activeWrite = true;

		EeCellStaticPrivate::data = value;
		afterWrite = runAfterWrite;

		Status st = status;
		st.unWritten = 1; 	// В случае прерывания записи, чтобы ячейка не оказалась ложно испорченной
		if ( status.updateUnblock( st, SoftIntHandler::from_method<EeCell, &EeCell::writeData>(this) ) )
			return true;
		else
		{
			activeWrite = false;
			return false;
		}
	}
	else
		return false;
}

bool EeCell::read(uint32_t& value)
{
	if ( !EeCellStaticPrivate::activeWrite )
	{
		EeCellStaticPrivate::Status s = status;

		if (!s.unWritten)
		{
			Complex<uint32_t> d = (uint32_t) data;

			uint8_t crc = 0xFF;
			for (uint8_t i = 0; i < 4; i++)
				crc = crc7x2 (crc, d[i]);

			if (crc/2 == s.crc7)
			{
				value = d;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

void EeCell::isGood( const SoftIntHandler& resultGetter )
{
	if ( !EeCellStaticPrivate::activeWrite )
	{
		EeCellStaticPrivate::Status s = status;

		if (!s.unWritten)
		{
			Complex<uint32_t> d = (uint32_t) data;

			uint8_t crc = 0xFF;
			for (uint8_t i = 0; i < 4; i++)
				crc = crc7x2 (crc, d[i]);

			dispatcher.add( resultGetter, crc/2 == s.crc7 );
		}
		else
			dispatcher.add( resultGetter, true );
	}
	else
	{
		EeCellStaticPrivate::isGoodResultGetter = resultGetter;
		dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::goodDelayedRequest> (this), 0 );
	}
}

void EeCell::isWritten( const SoftIntHandler& resultGetter )
{
	if ( !EeCellStaticPrivate::activeWrite )
	{
		EeCellStaticPrivate::Status s = status;

		if (!s.unWritten)
			dispatcher.add( resultGetter, true );
		else
			dispatcher.add( resultGetter, false );
	}
	else
	{
		EeCellStaticPrivate::isWrittenResultGetter = resultGetter;
		dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::writtenDelayedRequest> (this), 0 );
	}
}

uint8_t EeCell::crc7x2 (uint8_t crcx2, uint8_t data)
{
	crcx2 ^= data;
    for (uint8_t i = 0; i < 8; i++)
        crcx2 = crcx2 & 0x80 ? (crcx2 << 1) ^ (0x37 * 2) : crcx2 << 1;
	return crcx2;
}

void EeCell::writeData (uint16_t )
{
	if ( !data.updateUnblock( EeCellStaticPrivate::data, SoftIntHandler::from_method<EeCell, &EeCell::writeCrc>(this) ) )
	{
		EeCellStaticPrivate::activeWrite = false;
		dispatcher.add( EeCellStaticPrivate::afterWrite, false );
	}
}

void EeCell::writeCrc (uint16_t )
{
	if ( EeCellStaticPrivate::data == data )
	{
		uint8_t crc = 0xFF;
		for (uint8_t i = 0; i < 4; i++)
			crc = crc7x2 (crc, EeCellStaticPrivate::data[i]);

		EeCellStaticPrivate::status.crc7 = crc/2;
		EeCellStaticPrivate::status.unWritten = 0;

		if ( !status.updateUnblock( EeCellStaticPrivate::status,  SoftIntHandler::from_method<EeCell, &EeCell::lastControl>(this) ) )
		{
			EeCellStaticPrivate::activeWrite = false;
			dispatcher.add( EeCellStaticPrivate::afterWrite, false );
		}
	}
	else
	{
		EeCellStaticPrivate::activeWrite = false;
		dispatcher.add( EeCellStaticPrivate::afterWrite, false );
	}
}

void EeCell::lastControl (uint16_t )
{
	EeCellStaticPrivate::activeWrite = false;
	dispatcher.add( EeCellStaticPrivate::afterWrite,
					(EeCellStaticPrivate::status == (EeCellStaticPrivate::Status)status) );
}

void EeCell::goodDelayedRequest (uint16_t )
{
	isGood ( EeCellStaticPrivate::isGoodResultGetter );
}

void EeCell::writtenDelayedRequest (uint16_t )
{
	isWritten ( EeCellStaticPrivate::isWrittenResultGetter );
}


// --------------------------------------- Структура данных -------------------------------------►

struct EepromData
{
	struct Saut
	{
		Eeprom<uint16_t>	string1Number1;   			// номер строки		--?
		Eeprom<uint8_t>		diameterAvarage; 			// Фактический диаметр бандажа в мм (максимальный из двух: ДПС1 или ДПС2, деленный на 10, плюс 1).
		Eeprom<uint8_t>		vMaxPassenger;				// Vmax пасс. + Vкж пасс
		Eeprom<uint8_t>		vMaxCargo;					// Vmax груз. + Vкж груз
		Eeprom<uint8_t>		vMaxElecticTrain;			// Vmax эл.поезд. + Vкж эл.поезд
		Eeprom<uint8_t>		blockMinLength;				// Минимальная длина блок-участка (деленная на 10).В БЭК2 не используется, в БЭК не используется начиная с версии 33
		Eeprom<uint8_t>		locoType;					// Тип локомотива.
		Eeprom<uint8_t>		diameterAvarageForLocoType;	// Средний диаметр бандажа данного типа локомотива (деленный на 10)
		struct Configuration
		{
			enum DpsPosition { Left = 0, Rigth = 1 };
			DpsPosition	dps0Position	:1;
			DpsPosition	dps1Position	:1;
			uint8_t		eks				:1;
			uint8_t		tapKM130		:1;				// Кран машиниста КМ-130 (1)
			uint8_t		club			:1;				// --?
			enum { CL = 0, KPD = 1 } velocityGauge	:1; // Измеритель скорости
			enum { ALS = 0, CKR = 1 } ifSignal		:1; // Источник ИФ сигнала
			enum { ALSN = 0, CLUB = 1 } alarmSystem	:1; // Локомотивная сигнализация
		};
		Eeprom< Bitfield<Configuration> > configuration;
		Eeprom<uint16_t>	reserv1;    				// Резерв
		Eeprom<uint8_t>		diameterCorrection[2]; 		// Уточнение диаметров бандажей соответственно для ДПС1 и ДПС2. Вычисляются как разность между точным значением
														// соответствующего диаметра бандажа в миллиметрах и средним значением, умноженным на 10.
														// Седьмой бит - знаковый. -- ВНИМАНИЕ -- это не дополнительный код
		Eeprom<uint16_t>	controlSumm1;     			// Контрольная сумма первой строки: CRC16 на базисе 0xA9EB
		//  ------------------------------- Строка 2 -------------------------------
		Eeprom<uint16_t>	stringNumber2; 				// Номер второй строки (601h или 401h).				--?
		Eeprom<uint8_t>		locoName1;  				// Название локомотива (DOS - кодировка), дополненное пробелами (символами с кодом 20h).
		Eeprom<uint8_t>		locoName2;
		Eeprom<uint8_t>		locoName3;
		Eeprom<uint8_t>		locoName4;
		Eeprom<uint8_t>		locoName5;
		Eeprom<uint8_t>		locoName6;
		Eeprom<uint8_t>		locoNumberH;				// Номер локомотива (сначала старший байт, затем младший)
		Eeprom<uint8_t>		locoNumberL;
		enum class FeatureCode : uint8_t				// Код особенностей локомотива
		{
			None = 0,									// Нормальные локомотивы
			TractionSignalFromBsCkr = 1,				// Сигнал "Тяга" от БС-ЦКР (ЧС2)
			TractionInversionSignal = 2					// Инверсный сигнал "Тяга" (ЧС4, ЧС4Т, ЧС7, ЭП1, ВЛ65, ВЛ85, ВЛ11)
		};
		Eeprom<FeatureCode>	featureCode;
		Eeprom<uint8_t>		sectionNumber;				// Секция
		Eeprom<uint8_t>		vConstruct;					// Конструктивная скорость локомотива (км/ч)
		Eeprom<uint8_t>		tip;						// Тип (код) подвижной единицы (см. файл "Типы локомотивов")
		Eeprom<uint16_t>	controlSumm2;				// Контрольная сумма второй строки.
	} saut; // [0x00 -- 0x1F] : 32 байта

	union
	{
		EeCell cell[128];
		struct
		{
			EeCell		start;						//  0 - для совместимости (счёт идёт с 1)
			EeCell		track; 						//  1 - Номер пути для хранения в МПХ ЭК
			EeCell		driver; 					//  2 - Номер машиниста
			EeCell		train; 						//  3 - Номер поезда
			EeCell		category; 					//  4 - Категория поезда
			EeCell		lengthWheel;				//  5 - Длина состава в осях
			EeCell		lengthWagon; 				//  6 - Длина состава в условных вагонах
			EeCell		number; 					//  7 - Номер локомотива или ведущей секции многосекционного локомотива
			EeCell		weigth; 					//  8 - масса поезда
			EeCell		coordStart;					//  9 - Начальная координата
			EeCell		time;						// 10 - Время
			EeCell		typeLoco; 					// 11 - Тип локомотива
			EeCell		vMax; 						// 12 - Допустимая скорость (на белый)
			EeCell		vRedYellow; 				// 13 - Скорость движения на КЖ
			EeCell		blockLength;				// 14 - Приведённая длина блок-участка «Дозор»
			EeCell		diameter1; 					// 15 - Диаметр бандажа колеса 1, мм
			EeCell		diameter2; 					// 16 - Диаметр бандажа колеса 2, мм
			EeCell		dpsDentos;					// 17 - Число зубьев ДС
			EeCell		configuration;				// 18 - Конфигурация
			EeCell		vGreen; 					// 19 - Допустимая скорость на Зелёный
			EeCell		dirCoord; 					// 20 - Направление изменения координаты
			EeCell		milage; 					// 21 - Пробег локомотива
			EeCell		clsdVersion;				// 22 - Информация о версии КЛУБ
			EeCell		trackMPH; 					// 23 - Номер пути для хранения в МПХ ВПД
			EeCell		vpdPrivate; 				// 24 - Параментр используемый только внутри программы ВПД-М
			EeCell		bilBrightnes;  				// 25 - Параметр яркости для модуля БИЛ
			EeCell		snsPosition1;				// 26 - Расположение СНС первой кабины
			EeCell		snsPosition2;				// 27 - Расположение СНС второй кабины
			//-----------------------Запись по MCO_DATA---------------------------------------------------------
			EeCell		ufirOutConf;				// 28 - УФИР выходил из конфигурации
			EeCell		tskbmOutConf;				// 29 - ТСКБМ выходил из конфигурации
			EeCell		sautOutConf;				// 30 - САУТ выходил из конфигурации
			EeCell		bilOutConf;					// 31 - БИЛ выходил из конфигурации
			EeCell		ipdOutConf;					// 32 - ИПД выходил из конфигурации
			EeCell		bvuOutConf;					// 33 - БВУ выходил из конфигурации
			EeCell		mmOutConf;					// 34 - ММ выходил из конфигурации
			EeCell		ecOutConf;					// 35 - ЭК выходил из конфигурации
			//-----------------------Запись по BKSI_DATA--------------------------------------------------------
			EeCell		krError;					// 36 - Наличие и исправность КР
			EeCell		dps1Error;					// 37 - Обрыв ДПС1
			EeCell		dps2Error;					// 38 - Обрыв ДПС2
			EeCell		snsError;					// 39 - Нет данных СНС
			EeCell		epkError;					// 40 - Обрыв ЭПК
			EeCell		konError;					// 41 - Обрыв КОН
			EeCell		epvError;					// 42 - Обрыв ЭПВ
			EeCell		eptError;					// 43 - Неисправен ЭПТ
			EeCell		pkm1TvError;				// 44 - Обрыв ПКМ1 ТВ
			EeCell		pkm1OvError;				// 45 - Обрыв ПКМ1 ОВ
			EeCell		pkm2TvError;				// 46 - Обрыв ПКМ2 ТВ
			EeCell		pkm2OvError;				// 47 - Обрыв ПКМ2 ОВ

//			EeCell		empty[124];					// [0xE0 -- 0xFF] : 128 байт - отступ в памяти (резерв)
//			EeCell		end;						// последняя ячейка
		};
	};

	// Исправности датчиков
	Eeprom<uint8_t> 		dps0Good;
	Eeprom<uint8_t>			dps1Good;

} eeprom EEMEM;


// --------------------------------------- Связь с CAN ------------------------------------------►

template < typename CanDatType, CanDatType& canDat>
class ConstValModule
{
public:
	ConstValModule ();

	void getWriteMessage (uint16_t getDataPointer);
	void getLeftDataMessage (uint16_t getDataPointer);
	void getQueryMessage (uint16_t getDataPointer);

private:
	void isWritten (uint16_t res);
	void confirm (uint16_t tryNumber);
	void write (uint16_t tryNumber);
	void read (uint16_t written);
	void answer (uint16_t status);

	struct Packet
	{
		uint8_t number;
		Complex<uint32_t> data;
	};
	Bitfield<Packet> activePacket;
};

template < typename CanDatType, CanDatType& canDat>
ConstValModule<CanDatType, canDat>::ConstValModule ()
	{}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::getWriteMessage (uint16_t getDataPointer)
{
	if (activePacket.number == 0) // свободны
	{
		Packet& packet = *((Packet *) getDataPointer);

		// Извращение КЛУБа
		if ( packet.number == 9 )
			return;
		if ( packet.number == 1 )
			packet.number = 23;

		activePacket = packet;
		eeprom.cell[packet.number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isWritten> (this) );
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::getLeftDataMessage (uint16_t getDataPointer)
{
	if (activePacket.number == 0) // свободны
	{
		Packet& packet = *((Packet *) getDataPointer);

		// Извращение КЛУБа
		if ( packet.number == 1 ) // Только по адресу 1 принимать левые данные
		{
			activePacket = packet;
			eeprom.cell[packet.number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isWritten> (this) );
		}
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::getQueryMessage (uint16_t getDataPointer)
{
	if (activePacket.number == 0) // свободны
	{
		uint8_t& number = *((uint8_t *) getDataPointer);

		activePacket.number = number;
		eeprom.cell[number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::read> (this) );
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::isWritten (uint16_t res)
{
	if (res)
		confirm (0);
	else
		write (0);
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::confirm (uint16_t tryNumber)
{
	uint32_t savedData;
	if ( eeprom.cell[activePacket.number].read (savedData) )
	{
		if (savedData == activePacket.data)
			answer (true);
		else
			write (0);
	}
	else
	{
		if (tryNumber < 65000)
			dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule<CanDatType, canDat>::confirm> (this), tryNumber+1 );
		else
			activePacket.number = 0;
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::write (uint16_t tryNumber)
{
	if ( !eeprom.cell[activePacket.number].write( activePacket.data,
												  SoftIntHandler::from_method <ConstValModule, &ConstValModule<CanDatType, canDat>::answer> (this) ) )
	{
		if ( tryNumber < 65000 )
			dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule<CanDatType, canDat>::write> (this), tryNumber+1 );
		else
			activePacket.number = 0;
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::read (uint16_t written)
{
	if (written)
	{
		if ( eeprom.cell[activePacket.number].read (activePacket.data) )
		{
			answer (true);
		}
		else
		{
			if (written < 65000)
				dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule<CanDatType, canDat>::read> (this), written+1 );
			else
				activePacket.number = 0;
		}
	}
}

template < typename CanDatType, CanDatType& canDat>
void ConstValModule<CanDatType, canDat>::answer (uint16_t status)
{
	if (status == true)
	{
		canDat.template send<CanTx::SYS_DATA>
			( {activePacket.number, activePacket.data[0], activePacket.data[1], activePacket.data[2], activePacket.data[3]} );
	}
	activePacket.number = 0; // освобождаемся
}


template < typename CanDatType, CanDatType& canDat,
		   typename Scheduler, Scheduler& scheduler,
		   typename SautCom, SautCom& sautCom >
class MPH
{
public:
	MPH (bool active)
	{
		if (active)
			scheduler.runIn(
					Command {SoftIntHandler::from_method<MPH,&MPH::sendState> (this), 0},
					500 );
	}

	void writeConfirm (uint16_t packetAdr)
	{
		write (packetAdr);
		read (packetAdr);
	}

	void write (uint16_t packetAdr)
	{
		Packet& packet = *( (Packet *)(packetAdr) );

		if ( packet.parameter == 9 )
			return;

		if ( packet.parameter == 1 )
			packet.parameter = 23;

		write (packet);
	}

	void read (uint16_t parameterAdr)
	{
		const uint8_t& parameter = *((uint8_t *) parameterAdr);

		if ( parameter == 9 || parameter == 1 )
			return;

		uint8_t* adr = (uint8_t *) ( (uint16_t)(&eeprom.club) + parameter*4 );
		uint8_t packet[5] = {
					parameter,
					eeprom_read_byte (adr+3),
					eeprom_read_byte (adr+2),
					eeprom_read_byte (adr+1),
					eeprom_read_byte (adr)
						};
		canDat.template send<CanTx::SYS_DATA> (packet);
	}

private:
	struct Packet
	{
		uint8_t parameter;
		uint8_t data[4];
	};

	void write (const Packet& packet)
	{
		uint8_t* adr = (uint8_t *) ( (uint16_t)(&eeprom.club) + packet.parameter*4 );

		eeprom_update_byte (adr  , packet.data[3]);
		eeprom_update_byte (adr+1, packet.data[2]);
		eeprom_update_byte (adr+2, packet.data[1]);
		eeprom_update_byte (adr+3, packet.data[0]);
	}

	// Посылает в CAN дескрипоторый SYS_DATA_STATE и SYS_DATA_STATE_2
	void sendState (uint16_t)
	{
		bool success =
		scheduler.runIn(
				Command {SoftIntHandler::from_method<MPH,&MPH::sendState> (this), 0},
				500 );

		uint8_t sysDataState[7] = {
				0, // Результаты выполнения тестов...
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.configuration),
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.category),
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.vRedYellow),
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.vGreen),
				(uint8_t) (
						( eeprom_read_byte ((const uint8_t *) ((uint8_t*)&eeprom.club.vGreen +1)) * 128 )
						| (eeprom_read_word ((const uint16_t *) &eeprom.club.blockLength) / 100)
							 ),
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.lengthWagon)
								};
		canDat.template send<CanTx::SYS_DATA_STATE> (sysDataState);

		uint8_t sysDataState2[8] = {
				0, // Результаты выполнения тестов...
				(uint8_t) eeprom_read_byte ((const uint8_t *) ((uint8_t*)&eeprom.club.configuration +1)), 	// старший
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.configuration),					// младший
				(uint8_t) eeprom_read_byte ((const uint8_t *) ((uint8_t*)&eeprom.club.typeLoco +1)), 	// старший
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.typeLoco),					// младший
				success,
				scheduler.fill,
				sautCom.termTime
								};
		canDat.template send<CanTx::SYS_DATA_STATE2> (sysDataState2);
		sautCom.termTime = 0;

		Complex<uint16_t> trainNumber = eeprom_read_word ((const uint16_t *) &eeprom.club.train);
		uint8_t ipdParam[8] = {
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.trackMPH),
				trainNumber[0],
				trainNumber[1],
				0,
				0,
				0,
				0,
				0
							};
		canDat.template send<CanTx::IPD_PARAM> (ipdParam);
	}

	Delegate<>
};


#endif /* MPH_H_ */
