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
#include <cpp/scheduler.h>
#include "CanDat.h"
#include "CanDesriptors.h"

// ------------------------------------------- Ячейка -------------------------------------------►
//
// ~~~ Проблема: ~~~
// Хотелось бы иметь ячейку памяти в eeprom, которая бы помимо хранения значения ещё бы:
// 1. Хранила информацию о том, была ли она записана (то есть содержит реальные данные)
// 2. Контролировала целостность хранимых данных.
// 3. Взяла бы на себя функции по ожиданию готовности eeprom и прочее.
//
//  ~~~ Задача: ~~~
// 1. Создать класс, объект которого должен содержать доп. информацию (флаг записи и контрольную сумму)
// 2. Его функции чтения/записи берут на себя контроль контрльной суммы и выставление флага записи.
// 3. Являются неблокирующими и сами дожидаются освобождения eeprom.
//
// ~~~ Решение: ~~~
// 1. Класс содержит 4-х байтное значение и один служебный байт.
// 2. В служебном байте: 1 бит - флаг записанности, 7 бит - crc7 на полиноме 0x37
// 3. Если идёт операция с одним из объектов EeCell, то блокируется доступ ко всем другим.
// 4. Операция чтения возвращает значение сразу же (не повисает в цикле ожидания), но запускается если:
//		- данные доступны для чтения
//		- они записаны
//		- они записаны корректно (сходиться контрольная сумма)
// 5. Операция записи неблокирующая, начинает выполняться если
//		- нет активных операций с лбюбым объектом класса EeCell
//	  по завершению записи гарантированно вызывает пользовательский делегат.
//    Делегату в качестве параметра передаётя статус завершения (успешность записи).
// 6. Операция определения записанности неблокирующая, запускатеся всегда,
//    по завершению вызывает делегат, которому передаёт результат работы.
// 7. Операция проверки целостности неблокирующая, запускается всегда,
//    по завершению вызывает делегат, которому передаёт результат работы.
// 8. Операция прерывания всех текущих неблокирующих операций.
//    Прерывает все операции так быстро, как это возможно. После этого вызывает делегат.
//
/* ~~~ Пример: ~~~
	EeCell cell;


	void periodicalFunction (uint16_t)
	{
		static uint32_t data = 0;
		cell.read (data);	// data будет прочитано, если получится
							// в противном случае, значение data просто сохраниться

		// ... работа с data ...
	}

	void getWrittenResult (uint16_t);
	void getGoodnessResult (uint16_t);
	void exactlyGetData (uint16_t)
	{
		static uint32_t data = 0;
		if ( cell.read (data) ) // если получилось
		{
			// OK
		}
		else
		{
			cell.isWritten( SoftIntHandler::from_function<&getWrittenResult>() );
		}
	}

	void getWrittenResult (uint16_t result)
	{
		if (result) // записан?
		{
			// ячейка записана. имеет смысл проверить целостность
			 cell.isGood( SoftIntHandler::from_function<&getGoodnessResult>() );
		}
		else
		{
			// к сожалению, ячейка не записана...
		}

	}

	void getGoodnessnResult (uint16_t result)
	{
		if (result) // без ошибок?
		{
			// ячейка корректно записана.
			// чтение не удалось из-за занятости. повторим ещё раз.
			dispatcher.add ( SoftIntHandler::from_function<&exactlyGetData>(), 0 );
		}
		else
		{
			// к сожалению, память испорчена, данные повреждены...
		}
	}

	void afterWrite (uint16_t result);
	void writeValue (uint16_t value)
	{
		if ( cell.write (value) ) // пытаемся записать
		{
			// процесс началася. Запись идёт в фоне, после неё будет вызыван делегат.
		}
		else
		{
			// память занята. что же, попробуем ещё раз.
			dispatcher.add( SoftIntHandler::from_fucntion<&writeValue>(), value );
		}
	}

	void afterWrite (uint16_t result)
	{
		if (result) // записана корректно
		{
			// примите мои поздравления!
		}
		else
		{
			// примите мои соболезнования...
		}
	}

	int main ()
	{
		for (;;)
			dispatcher.invoke ();
	}
 *
 */

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

	bool eepromOpRunning;
	enum ResetRequest
	{
		No	= 0,
		SelfWaitCycle = 1,
		AfterEepromOp = 2
	};
	ResetRequest resetRequest (No);
	SoftIntHandler afterReset;
}

class EeCell
{
public:
	bool write( const uint32_t& value, const SoftIntHandler& runAfterWriteEnd = SoftIntHandler() );
	bool read ( uint32_t& value );
	void isGood ( const SoftIntHandler& resultGetter );
	void isWritten ( const SoftIntHandler& resultGetter );
	void reset ( const SoftIntHandler& runAfterReset = SoftIntHandler() );

private:
	uint8_t crc7x2 (uint8_t crcx2, uint8_t data);

	void writeStatus (uint16_t);
	void writeData (uint16_t);
	void calcCrc (uint16_t);
	void writeCrc (uint16_t);
	void lastControl (uint16_t);
	void goodDelayedRequest (uint16_t);
	void writtenDelayedRequest (uint16_t);
	void runAfterReset (uint16_t);

	Eeprom< EeCellStaticPrivate::Status > status;
	Eeprom<uint32_t> data;
};


bool EeCell::write (const uint32_t& value, const SoftIntHandler& runAfterWrite)
{
	namespace Static = EeCellStaticPrivate;
	if ( !Static::activeWrite )
	{
		Static::activeWrite = true;

		Static::data = value;
		Static::afterWrite = runAfterWrite;

		Static::status = status;
		Static::status.unWritten = 1; 	// В случае прерывания записи, чтобы ячейка не оказалась ложно испорченной
		writeStatus (0);

		return true;
	}
	else
		return false;
}

bool EeCell::read(uint32_t& value)
{
	if ( !EeCellStaticPrivate::activeWrite  && status.isReady() && data.isReady() )
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
	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		if ( !EeCellStaticPrivate::activeWrite )
		{
			uint8_t sreg = reg.status;
			cli();
			if ( status.isReady() )
			{
				EeCellStaticPrivate::Status s = status;
				reg.status = sreg;

				if (!s.unWritten)
				{
					uint8_t sreg = reg.status;
					cli();
					if ( data.isReady() )
					{
						Complex<uint32_t> d = (uint32_t) data;
						reg.status = sreg;

						uint8_t crc = 0xFF;
						for (uint8_t i = 0; i < 4; i++)
							crc = crc7x2 (crc, d[i]);

						dispatcher.add( resultGetter, crc/2 == s.crc7 );
						return;
					}
					else
						reg.status = sreg;
				}
				else
				{
					dispatcher.add( resultGetter, true );
					return;
				}
			}
			else
				reg.status = sreg;
		}
		// Если не ушли по return выше
		EeCellStaticPrivate::isGoodResultGetter = resultGetter;
		dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::goodDelayedRequest> (this), 0 );
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::isWritten( const SoftIntHandler& resultGetter )
{
	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		if ( !EeCellStaticPrivate::activeWrite )
		{
			uint8_t sreg = reg.status;
			cli();
			if ( status.isReady() )
			{
				EeCellStaticPrivate::Status s = status;
				reg.status = sreg;

				if (!s.unWritten)
					dispatcher.add( resultGetter, true );
				else
					dispatcher.add( resultGetter, false );

				return;
			}
			else
				reg.status = sreg;
		}
		// Если не ушли по return выше
		EeCellStaticPrivate::isWrittenResultGetter = resultGetter;
		dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::writtenDelayedRequest> (this), 0 );
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::reset( const SoftIntHandler& runAfterReset )
{
	using namespace EeCellStaticPrivate;
	afterReset = runAfterReset;

	if (eepromOpRunning)
		resetRequest = ResetRequest::AfterEepromOp;
	else
	{
		resetRequest = ResetRequest::SelfWaitCycle;
		// После всех задач в очереди запустить:
		dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::runAfterReset> (this), 0 );
	}
}

uint8_t EeCell::crc7x2 (uint8_t crcx2, uint8_t data)
{
	crcx2 ^= data;
    for (uint8_t i = 0; i < 8; i++)
        crcx2 = crcx2 & 0x80 ? (crcx2 << 1) ^ (0x37 * 2) : crcx2 << 1;
	return crcx2;
}


void EeCell::writeStatus (uint16_t )
{
	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		if ( status.updateUnblock( EeCellStaticPrivate::status, SoftIntHandler::from_method<EeCell, &EeCell::writeData>(this) ) )
			EeCellStaticPrivate::eepromOpRunning = true;
		else
				dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::writeStatus> (this), 0 );
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::writeData (uint16_t )
{
	EeCellStaticPrivate::eepromOpRunning = false;

	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		if ( data.updateUnblock( EeCellStaticPrivate::data, SoftIntHandler::from_method<EeCell, &EeCell::calcCrc>(this) ) )
			EeCellStaticPrivate::eepromOpRunning = true;
		else
			dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::writeData> (this), 0 );
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::calcCrc (uint16_t )
{
	EeCellStaticPrivate::eepromOpRunning = false;

	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		namespace Static = EeCellStaticPrivate;
		uint8_t crc = 0xFF;
		for (uint8_t i = 0; i < 4; i++)
			crc = crc7x2 (crc, Static::data[i]);

		Static::status.crc7 = crc/2;
		Static::status.unWritten = 0;

		writeCrc (0);
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::writeCrc (uint16_t )
{
	EeCellStaticPrivate::eepromOpRunning = false;

	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		if ( status.updateUnblock( EeCellStaticPrivate::status,  SoftIntHandler::from_method<EeCell, &EeCell::lastControl>(this) ) )
			EeCellStaticPrivate::eepromOpRunning = true;
		else
			dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::writeCrc> (this), 0 );
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}
}

void EeCell::lastControl (uint16_t )
{
	namespace Static = EeCellStaticPrivate;

	Static::eepromOpRunning = false;

	if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::No)
	{
		uint8_t sreg = reg.status;
		cli ();
		if ( status.isReady() && data.isReady() )
		{
			Static::Status s = status;
			uint32_t d = data;
			reg.status = sreg;

			dispatcher.add( EeCellStaticPrivate::afterWrite,
							(Static::status == s) && (Static::data == d) );

			Static::activeWrite = false;
		}
		else
		{
			reg.status = sreg;
			dispatcher.add( SoftIntHandler::from_method<EeCell, &EeCell::lastControl>(this), 0 );
		}
	}
	else if (EeCellStaticPrivate::resetRequest == EeCellStaticPrivate::ResetRequest::SelfWaitCycle)
	{
		runAfterReset (0);
	}

}

void EeCell::goodDelayedRequest (uint16_t )
{
	isGood ( EeCellStaticPrivate::isGoodResultGetter );
}

void EeCell::writtenDelayedRequest (uint16_t )
{
	isWritten ( EeCellStaticPrivate::isWrittenResultGetter );
}

void EeCell::runAfterReset (uint16_t)
{
	EeCellStaticPrivate::resetRequest = EeCellStaticPrivate::ResetRequest::No;
	dispatcher.add( EeCellStaticPrivate::afterReset, 0 );
	EeCellStaticPrivate::activeWrite = false;
}


// --------------------------------------- Структура данных -------------------------------------►

struct EepromData
{
	EepromData () {}

	enum DpsPosition { Left = 0, Right = 1 };
	enum VelocityGauge { CL = 0, KPD = 1 }; 	// �?змеритель скорости
	enum IfSignal { ALS = 0, CKR = 1 };
	enum AlarmSystem { ALSN = 0, CLUB = 1 };

	union Saut
	{
		Saut () {}

		struct
		{
			Eeprom<uint8_t> data [14];
			Eeprom<uint16_t> crc; // CRC16 на базисе 0xEBA9
		} string [2];

		struct Property
		{
			Eeprom<uint16_t>	string1Number;   			// + номер первой строки (0x0600)BigEndian
			Eeprom<uint8_t>		diameterAverageDiv10; 		// + Фактический диаметр бандажа в мм (максимальный из двух: ДПС1 или ДПС2, деленный на 10, плюс 1).
			struct SpeedLimitation
			{
				uint8_t redYellowDiv10Minus1	:4;
				uint8_t maxDiv10Minus1			:4;
			};
			Eeprom< Bitfield<SpeedLimitation> >		vPassenger;			// +! Vmax пасс. + Vкж пасс
			Eeprom< Bitfield<SpeedLimitation> >		vCargo;				// +! Vmax груз. + Vкж груз
			Eeprom< Bitfield<SpeedLimitation> >		vElecticTrain;		// +! Vmax эл.поезд. + Vкж эл.поезд
			Eeprom<uint8_t>		blockMinLength;				// Минимальная длина блок-участка (деленная на 10).В БЭК2 не используется, в БЭК не используется начиная с версии 33
			Eeprom<uint8_t>		locoType;					// +! Тип локомотива.
			Eeprom<uint8_t>		diameterAvarageForLocoType;	// Средний диаметр бандажа данного типа локомотива (деленный на 10)
			struct Configuration
			{
				DpsPosition		dps0Position	:1;				// +
				DpsPosition		dps1Position	:1;				// +
				uint8_t			eks				:1;				// --- спросить (всегда ДА)
				uint8_t			tapKM130		:1;				// + Кран машиниста КМ-130 (1)
				uint8_t			club			:1;				// Флаг КЛУБ-У для локомотивной сигнализации (для БЛОК всегда)
				VelocityGauge	velocityGauge	:1; 			// �?змеритель скорости
				IfSignal		ifSignal		:1; 			// �?сточник �?Ф сигнала (для БЛОК всегда АЛС)
				AlarmSystem		alarmSystem		:1; 			// Локомотивная сигнализация (для БЛОК всегда КЛУБ)
			};
			Eeprom< Bitfield<Configuration> > configuration;// +!
			Eeprom<uint16_t>	reserv1;    				// Резерв
			Eeprom<int8_t>		diameterCorrection[2]; 		// + Уточнение диаметров бандажей соответственно для ДПС1 и ДПС2. Вычисляются как разность между точным значением
															//   соответствующего диаметра бандажа в миллиметрах и средним значением, умноженным на 10.
															//   Седьмой бит - знаковый. -- ВН�?МАН�?Е -- это не дополнительный код
			Eeprom<uint16_t>	controlSumm1;     			// + Контрольная сумма первой строки: CRC16 на базисе 0xA9EB
			//  ------------------------------- Строка 2 -------------------------------
			Eeprom<uint16_t>	string2Number; 				// + Номер второй строки (использую:0x0601, можно(?) 0x0401)BigEndian.
			Eeprom<uint32_t>	locoName14;  				// +! Название локомотива (DOS - кодировка), дополненное пробелами (символами с кодом 20h). 1-4 символы.
			Eeprom<uint16_t>	locoName56;					// +! Название локомотива. 5 и 6 символы.
			Eeprom<uint16_t>	locoNumberBigEndian;		// + Номер локомотива (сначала старший байт, затем младший)
			enum class FeatureCode : uint8_t				// Код особенностей локомотива
			{
				None = 0,									// Нормальные локомотивы
				TractionSignalFromBsCkr = 1,				// Сигнал "Тяга" от БС-ЦКР (ЧС2)
				TractionInversionSignal = 2					// �?нверсный сигнал "Тяга" (ЧС4, ЧС4Т, ЧС7, ЭП1, ВЛ65, ВЛ85, ВЛ11)
			};
			Eeprom<FeatureCode>	featureCode;
			Eeprom<uint8_t>		sectionNumber;				// +! Секция
			Eeprom<uint8_t>		vConstruct;					// +! Конструктивная скорость локомотива (км/ч)
			Eeprom<uint8_t>		locoTip;					// +! Тип (код) подвижной единицы (см. файл "Типы локомотивов")
			Eeprom<uint16_t>	controlSumm2;				// + Контрольная сумма второй строки.
		} property;
	} saut; // [0x00 -- 0x1F] : 32 байта

//	EeCell cell[128];
	union Club
	{
		Club () {}
		EeCell cell[128];
		struct Property
		{
			EeCell		start;						//  0 - для совместимости (счёт идёт с 1)
			EeCell		track; 						//  1 - Номер пути для хранения в МПХ ЭК
			EeCell		driver; 					//  2 - Номер машиниста
			EeCell		train; 						//  3 - Номер поезда
			EeCell		category; 					//  4 - Категория поезда
			EeCell		lengthWheel;				//  5 - Длина состава в осях
			EeCell		lengthWagon; 				//  6 - Длина состава в условных вагонах
			EeCell		locoNumber; 				//  7 - Номер локомотива или ведущей секции многосекционного локомотива
			EeCell		weigth; 					//  8 - масса поезда
			EeCell		coordStart;					//  9 - Начальная координата
			EeCell		time;						// 10 - Время
			EeCell		typeLoco; 					// 11 - Тип локомотива
			EeCell		vWhite; 						// 12 - Допустимая скорость (на белый)
			EeCell		vRedYellow; 				// 13 - Скорость движения на КЖ
			EeCell		blockLength;				// 14 - Приведённая длина блок-участка «Дозор»
			EeCell		diameter0; 					// 15 - Диаметр бандажа колеса 1, мм
			EeCell		diameter1; 					// 16 - Диаметр бандажа колеса 2, мм
			EeCell		dpsDentos;					// 17 - Число зубьев ДС
			struct Configuration
			{
				DpsPosition		dps0Position		:1; // 0 ---
				uint32_t		cabNumber			:1; // 1
				DpsPosition		dps1Position		:1; // 2
				uint32_t		dpsNumber			:1; // 3
				uint32_t		tskbm				:1; // 4
				uint32_t		aloneLogModule		:1; // 5
				uint32_t		staticDpsCheck		:1; // 6
				uint32_t		kupol				:1; // 7
				uint32_t		eks2				:1; // 8 ---
				enum TractusType { Autonomus = 0, Electric = 1 };
				TractusType		tractusType			:1; // 9
				uint32_t		autonomusModules	:1; // 10
				uint32_t							:1; // 11
				uint32_t		mpsu				:1; // 12
				uint32_t		uktol				:1; // 13
				uint32_t		epk151d				:1; // 14
				uint32_t							:1; // 15
				uint32_t							:16;
			};
			EeCell		configuration;				// 18 - Конфигурация
			EeCell		vGreen; 					// 19 - Допустимая скорость на Зелёный
			EeCell		dirCoord; 					// 20 - Направление изменения координаты
			EeCell		milage; 					// 21 - Пробег локомотива
			EeCell		clsdVersion;				// 22 - �?нформация о версии КЛУБ
			EeCell		trackMPH; 					// 23 - Номер пути для хранения в МПХ ВПД
			EeCell		vpdPrivate; 				// 24 - Параментр используемый только внутри программы ВПД-М
			EeCell		bilBrightnes;  				// 25 - Параметр яркости для модуля Б�?Л
			EeCell		snsPosition1;				// 26 - Расположение СНС первой кабины
			EeCell		sndPosition2;				// 27 - Расположение СНС второй кабины
			//-----------------------Запись по MCO_DATA---------------------------------------------------------
			EeCell		ufirOutConf;				// 28 - УФ�?Р выходил из конфигурации
			EeCell		tskbmOutConf;				// 29 - ТСКБМ выходил из конфигурации
			EeCell		sautOutConf;				// 30 - САУТ выходил из конфигурации
			EeCell		bilOutConf;					// 31 - Б�?Л выходил из конфигурации
			EeCell		ipdOutConf;					// 32 - �?ПД выходил из конфигурации
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

			EeCell		empty[52];					// (резерв)
			//-------------------------- Для САУТа -------------------------------------------------------------
			EeCell		vMaxPassenger;				// 100
			EeCell		vRedYellowPassenger;		// 101
			EeCell		vMaxCargo;					// 102
			EeCell		vRedYellowCargo;			// 103
			EeCell		vMaxElectricTrain;			// 104
			EeCell		vRedYellowElectricTrain;	// 105
			EeCell		vConstruct;					// 106

			EeCell		locoType;					// 107
			EeCell		locoTip;					// 108
			EeCell		locoName1;					// 109
			EeCell		locoName2;					// 110
			EeCell		section;					// 111

			EeCell		dps0Position;				// 112 conf.0
			EeCell		dps1Position;				// 113 conf.2
			EeCell		eks;						// 114
			EeCell		tapKM130;					// 115
			EeCell		velocityGauge;				// 116
			EeCell		ifSignalSource;				// 117
			EeCell		alarmSystem;				// 118
			struct SautConfiguration
			{
				uint32_t		tapKM130			:1; // 0
				VelocityGauge	velocityGauge		:1; // 1
				uint32_t		eks					:1; // 2
				uint32_t		club				:1; // 3
				IfSignal		ifSignal			:1; // 4
				AlarmSystem		alarmSystem			:1; // 5
				uint32_t							:10;
				uint32_t							:16;
			};
			EeCell		sautConfiguration;			// 119

//			EeCell		end;						// последняя ячейка
		} property;
	} club;

	// �?справности датчиков
	Eeprom<uint8_t> 		dps0Good;
	Eeprom<uint8_t>			dps1Good;

} eeprom EEMEM;


// ------------------------------------ Конвертация в САУТ --------------------------------------►
//
// ~~~ Проблема: ~~~
// Характеристики записываются и читаются из ячеек по алгоритму КЛУБа. Ячейки описаны выше в структуре Club,
// Но для совместимости с САУТом нужно хранить записанную в ячейки информацию ещё и в формате САУТ. (см. выше Saut)
// САУТ только читает эту информацию, но не записывает.
//
// ~~~ Задача: ~~~
// 1. Каждый раз при записи любой ячейки КЛУБа проверять, что за ячейка (и какая информация) записывается.
// 2. Если эта информация содержиться в базе САУТ, то преобразовывать её в нужный формат и записывать туда.
//
// ~~~ Решение: ~~~
// 1. После всех шагов по записи данных в ячейку КЛУБ, но до выдачи подтверждения вставить этап конвертации в САУТ. На этом этапе:
//		- Если записываемая информация содержиться в САУТ, то сконвертировать её.
//		- Записать в нужные поля области САУТ.
//		- Обновить crc соответсвующей строки.
//		- Проверить наличие заголовка строки.
// 2. При этом отсутсвует контроль целостности САУТовской части (это может делать САУТ, при этом crc нужно не забывать обновлять нам).
// 3. В случае прерывания процесса записи обеспечить возможность прервать запись в САУТ.
// 5. Для быстрого ответа на запрос от САУТа на чтение eeprom нужно хранить копию в оперативке (вдруг не будет доступа к eeprom)
//    �? обновлять эту копию регулярно.
//
// ~~~ �?нтерфейс: ~~~
// 1. updateCell (number, data, afterUpdate)
//    - Функция, которая должна быть вызвана в звене этапов записи после записи в КЛУБ и перед выдачей результата.
//    - Производит всю работу по конвертации и записи (если нужно).
//    - Является неблокирующей. После выполнения своей работы вызывает делегат afterUpdate.
//    - Нужно строго следить, чтобы не запустить эту функцию ещё раз до того, как она отработала предыдущий раз!
// 2. reset (afterReset)
//    - Функция прерывает текущую работу (если идёт) и после этого вызывает делегат afterReset.
//    - Что при этом останется в eeprom не известно. Возможно не успеет обновиться crc.
//      Тогда она восстановится только при следующей записи этой строки.
// 3. plainMap[32]
//    - побайтный массив в оперативной памяти, отражающий текущее состояние eeprom.
//

class SautConvert
{
public:
	SautConvert ();
	void updateCell (uint8_t number, Complex<uint32_t> data, const SoftIntHandler& afterUpdate);
	void reset (SoftIntHandler afterReset);

	uint8_t plainMap[32]; // отображение в ram данных из eeprom

private:
	void init1StringPlainMap (uint16_t );
	void dataUpdate (uint16_t);
	void diametersWriteStep1 (uint16_t);
	void diametersWriteStep2 (uint16_t);
	void diametersWriteStep3 (uint16_t);
	void updateStringCrc (uint16_t pointer);
	void readNextStringByte (uint16_t byteStringNumber);
	template<uint16_t poly>
	uint16_t crcUpdate (uint16_t crc, const uint8_t& data);
	void runAfterReset (uint16_t);

	uint8_t diamAvrDiv10;
	int8_t diamCor[2];

	uint16_t crc;
	uint8_t stringNumber;
	uint8_t cellNumber;
	Complex<uint32_t> data;
	SoftIntHandler runAfter;
	bool eepromOpRunning;
	enum ResetRequest
	{
		No	= 0,
		SelfWaitCycle = 1,
		AfterEepromOp = 2
	};
	ResetRequest resetRequest;
	SoftIntHandler afterReset;

};

SautConvert::SautConvert ()
	: eepromOpRunning (false), resetRequest (No)
{
	stringNumber = 0;
	runAfter = SoftIntHandler::from_method<SautConvert, &SautConvert::init1StringPlainMap>(this);
	updateStringCrc (0);
}

void SautConvert::init1StringPlainMap (uint16_t	)
{
	runAfter = SoftIntHandler();
	stringNumber = 1;
	updateStringCrc (0);
}

void SautConvert::updateCell (uint8_t number, Complex<uint32_t> data, const SoftIntHandler& afterUpdate)
{
	cellNumber = number;
	SautConvert::data = data;
	runAfter = afterUpdate;

	dataUpdate (0);
}

void SautConvert::reset (SoftIntHandler afterReset)
{
	SautConvert::afterReset = afterReset;

	if (eepromOpRunning)
		resetRequest = ResetRequest::AfterEepromOp;
	else
	{
		resetRequest = ResetRequest::SelfWaitCycle;
		// после всех задач в очереди запустить:
		dispatcher.add( SoftIntHandler::from_method<SautConvert, &SautConvert::runAfterReset>(this), 0 );
	}

}

void SautConvert::dataUpdate (uint16_t )
{
	if (resetRequest == ResetRequest::No)
	{
		eepromOpRunning = true;

		if (cellNumber == 7) // locoNumber
		{
			stringNumber = 1;
			if ( eeprom.saut.property.locoNumberBigEndian.updateUnblock(
					(uint16_t)Complex<uint16_t>{ data[1], data[0] },
					SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 15 || cellNumber == 16) // diameter0, diameter1
		{
			uint8_t num = (cellNumber == 16); // Номер вводимого бандажа
			eepromOpRunning = false;

			uint8_t sreg = reg.status;
			cli ();
			if (	eeprom.saut.property.diameterCorrection[!num].isReady() &&
					eeprom.saut.property.diameterAverageDiv10.isReady() 	)
			{
				diamCor[!num] =  eeprom.saut.property.diameterCorrection[!num];
				diamAvrDiv10  =  eeprom.saut.property.diameterAverageDiv10;
				reg.status = sreg;

				uint16_t diam[2];
				diam[!num] = uint16_t(diamAvrDiv10) * 10 +
								(diamCor[!num] & 0x7F) * (diamCor[!num] >= 0 ? 1 : -1); // отрицательное в прямом коде
				diam[num] = (uint16_t)Complex<uint16_t>{ data[0], data[1] };

				if ( abs(diam[num] - diam[!num]) > 246 )  // другое значение отстоит слишком далеко
				{
					// тогда, что делать, подвинем его поближе
					if (diam[num] < diam[!num])
						diam[!num] = diam[num] + 245;
					else
						diam[!num] = diam[num] - 245;
				}

				 // На этот момент различие в диаметрах не более 10, поэтому корректировки не более 127
				diamAvrDiv10 = (diam[num] + diam[!num]) / 20;

				for (uint8_t i = 0; i <= 1; i ++)
				{
					diamCor[i] = diam[i] - diamAvrDiv10 * 10;
					if ( diamCor[i] < 0 )
						diamCor[i] = ((~diamCor[i]) | 128) + 1; // знак в прямом коде
				}

				dispatcher.add( SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep1>(this), 0 );
				return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 18) // configuration
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.property.configuration.isReady() )
			{
				Bitfield<EepromData::Saut::Property::Configuration> sautConf = eeprom.saut.property.configuration;
				reg.status = sreg;

				Bitfield<EepromData::Club::Property::Configuration> clubConf = (uint32_t)Complex<uint32_t>{ data[0], data[1], data[2], data[3] };

				sautConf.dps0Position = clubConf.dps0Position;
				sautConf.dps1Position = clubConf.dps1Position;

				stringNumber = 0;
				if ( eeprom.saut.property.configuration.updateUnblock( sautConf, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
					return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 119) // Saut configuration
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.property.configuration.isReady() )
			{
				Bitfield<EepromData::Saut::Property::Configuration> sautConf = eeprom.saut.property.configuration;
				reg.status = sreg;

				Bitfield<EepromData::Club::Property::SautConfiguration> conf = (uint32_t)Complex<uint32_t>{ data[0], data[1], data[2], data[3] };

				sautConf.eks = conf.eks;
				sautConf.tapKM130 = conf.tapKM130;
				sautConf.club = conf.club;
				sautConf.velocityGauge = conf.velocityGauge;
				sautConf.ifSignal = conf.ifSignal;
				sautConf.alarmSystem = conf.alarmSystem;

				stringNumber = 0;
				if ( eeprom.saut.property.configuration.updateUnblock( sautConf, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
					return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 100 || cellNumber == 101) // vMaxPassenger || vRedYellowPassenger
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.property.vPassenger.isReady() )
			{
				Bitfield<EepromData::Saut::Property::SpeedLimitation> vPas = eeprom.saut.property.vPassenger;
				reg.status = sreg;

				if (cellNumber == 100)
					vPas.maxDiv10Minus1 = data[0]/10 - 1;
				if (cellNumber == 101)
					vPas.redYellowDiv10Minus1 = data[0]/10 - 1;

				stringNumber = 0;
				if ( eeprom.saut.property.vPassenger.updateUnblock( vPas, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
					return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 102 || cellNumber == 103) // vMaxElectricCargo || vRedYellowCargo
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.property.vCargo.isReady() )
			{
				Bitfield<EepromData::Saut::Property::SpeedLimitation> vCargo = eeprom.saut.property.vCargo;
				reg.status = sreg;

				if (cellNumber == 102)
					vCargo.maxDiv10Minus1 = data[0]/10 - 1;
				if (cellNumber == 103)
					vCargo.redYellowDiv10Minus1 = data[0]/10 - 1;

				stringNumber = 0;
				if ( eeprom.saut.property.vCargo.updateUnblock( vCargo, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
					return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 104 || cellNumber == 105) // vMaxElectricElectricTrain || vRedYellowElectricTrain
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.property.vElecticTrain.isReady() )
			{
				Bitfield<EepromData::Saut::Property::SpeedLimitation> vElecticTrain = eeprom.saut.property.vElecticTrain;
				reg.status = sreg;

				if (cellNumber == 104)
					vElecticTrain.maxDiv10Minus1 = data[0]/10 - 1;
				if (cellNumber == 105)
					vElecticTrain.redYellowDiv10Minus1 = data[0]/10 - 1;

				stringNumber = 0;
				if ( eeprom.saut.property.vElecticTrain.updateUnblock( vElecticTrain, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
					return;
			}
			else
				reg.status = sreg;
		}
		else if (cellNumber == 106) // vConstruct
		{
			stringNumber = 1;
			if ( eeprom.saut.property.vConstruct.updateUnblock( data[0], SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 107) // locoType
		{
			stringNumber = 0;
			if ( eeprom.saut.property.locoType.updateUnblock( data[0], SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 108) // locoTip
		{
			stringNumber = 1;
			if ( eeprom.saut.property.locoTip.updateUnblock( data[0], SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 109) // locoName1
		{
			stringNumber = 1;
			if ( eeprom.saut.property.locoName14.updateUnblock(
					(uint32_t)Complex<uint32_t>{ data[3], data[2], data[1], data[0] }, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 110) // locoName2
		{
			stringNumber = 1;
			if ( eeprom.saut.property.locoName56.updateUnblock(
					(uint16_t)Complex<uint16_t>{ data[3], data[2] }, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else if (cellNumber == 111) // section
		{
			stringNumber = 1;
			if ( eeprom.saut.property.sectionNumber.updateUnblock( data[0], SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
				return;
		}
		else
		{
			eepromOpRunning = false;
			dispatcher.add( runAfter, 0 );
			return;
		}

		// Если в своём месте успешно не завершился return'ом
		eepromOpRunning = false;
		dispatcher.add( Command{SoftIntHandler::from_method<SautConvert, &SautConvert::dataUpdate>(this), 0} );
	}
	else if (resetRequest == ResetRequest::SelfWaitCycle)
	{
		runAfterReset(0);
	}
}

void SautConvert::diametersWriteStep1 (uint16_t)
{
	eepromOpRunning = false;
	if (resetRequest == ResetRequest::No)
	{
		if ( eeprom.saut.property.diameterAverageDiv10.updateUnblock(
				diamAvrDiv10,
				SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep2>(this) )
				)
			eepromOpRunning = true;
		else
			dispatcher.add( SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep1>(this), 0 );
	}
	else if (resetRequest == ResetRequest::AfterEepromOp)
	{
		runAfterReset(0);
	}
}

void SautConvert::diametersWriteStep2 (uint16_t)
{
	eepromOpRunning = false;
	if (resetRequest == ResetRequest::No)
	{
		if ( eeprom.saut.property.diameterCorrection[0].updateUnblock(
				diamCor[0],
				SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep3>(this) )
				)
			eepromOpRunning = true;
		else
			dispatcher.add( SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep2>(this), 0 );
	}
	else if (resetRequest == ResetRequest::AfterEepromOp)
	{
		runAfterReset(0);
	}
}

void SautConvert::diametersWriteStep3 (uint16_t)
{
	eepromOpRunning = false;
	if (resetRequest == ResetRequest::No)
	{
		stringNumber = 0;
		if ( eeprom.saut.property.diameterCorrection[1].updateUnblock(
				diamCor[1],
				SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) )
				)
			eepromOpRunning = true;
		else
			dispatcher.add( SoftIntHandler::from_method<SautConvert, &SautConvert::diametersWriteStep3>(this), 0 );
	}
	else if (resetRequest == ResetRequest::AfterEepromOp)
	{
		runAfterReset(0);
	}
}

void SautConvert::updateStringCrc (uint16_t pointer)
{
	eepromOpRunning = false; // Эта функция вызывается по завершению операции eeprom
	uint8_t str = stringNumber;
	crc = 0;
	readNextStringByte( Complex<uint16_t>{str,0} );
}

void SautConvert::readNextStringByte (uint16_t byteStringNumber)
{
	if (resetRequest == ResetRequest::No)
	{
		uint8_t& string = Complex<uint16_t> (byteStringNumber)[0];
		uint8_t& byte = Complex<uint16_t> (byteStringNumber)[1];

		if (byte == 14)
		{
			if ( eeprom.saut.string[string].crc.updateUnblock( swap(crc), runAfter ) )
			{
				plainMap[string*16+14] = uint8_t(crc/256);
				plainMap[string*16+15] = uint8_t(crc);

				return;
			}
		}
		else
		{
			uint8_t sreg = reg.status;
			cli ();
			if ( eeprom.saut.string[string].data[byte].isReady() )
			{
				uint8_t data = eeprom.saut.string[string].data[byte];
				reg.status = sreg;


				// Восстановление "номера строки" в случае отсутсвия
				eepromOpRunning = true; // приготовимся к записи
				if (byte == 0 && data != 0x06)
				{
					if ( eeprom.saut.string[string].data[0].updateUnblock( 0x06, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
						return;
				}
				else if (byte == 1 && string == 0 && data != 0x00)
				{
					if ( eeprom.saut.string[0].data[1].updateUnblock( 0x00, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
						return;
				}
				else if (byte == 1 && string == 1 && data != 0x01)
				{
					if ( eeprom.saut.string[1].data[1].updateUnblock( 0x01, SoftIntHandler::from_method<SautConvert, &SautConvert::updateStringCrc>(this) ) )
						return;
				}
				else // "Номера строк" прописаны корректно.
				{
					eepromOpRunning = false; // запись не понадобилась

					// Обновляем plainMap
					plainMap[string*16+byte] = data;

					// Выполняем подсчёт crc..
					crc = crcUpdate<0xEBA9> (crc, data);
					readNextStringByte( Complex<uint16_t>{string, byte+1} );
					return;
				}
			}
			else
				reg.status = sreg;
		}

		dispatcher.add( Command{SoftIntHandler::from_method<SautConvert, &SautConvert::readNextStringByte>(this), byteStringNumber} );
	}
	else if (resetRequest == ResetRequest::AfterEepromOp)
	{
		runAfterReset(0);
	}
}

template<uint16_t poly>
uint16_t SautConvert::crcUpdate (uint16_t crc, const uint8_t& data)
{
	crc ^= (uint16_t(data) << 8);
    for (uint8_t i = 0; i < 8; i++)
        crc = crc & 0x8000 ? (crc << 1) ^ poly : crc << 1;
    return crc;
}

void SautConvert::runAfterReset (uint16_t)
{
	resetRequest = ResetRequest::No;
	dispatcher.add( afterReset, 0 );
}

// --------------------------------------- Связь с CAN ------------------------------------------►

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
class ConstValModule
{
public:
	ConstValModule ();

	void getWriteMessage (uint16_t getDataPointer);
	void getLeftDataMessage (uint16_t getDataPointer);
	void getQueryMessage (uint16_t getDataPointer);

	SautConvert sautConvert;

private:
	void isWritten (uint16_t res);
	void isGoodWhenWrite (uint16_t res);
	void isGoodWhenRead (uint16_t res);
	void confirm (uint16_t tryNumber);
	void write (uint16_t tryNumber);
	void read (uint16_t written);
	void sautWrite (uint16_t status);
	void successEnd (uint16_t );

	void sendState (uint16_t );
	void checkNext (uint16_t res);
	void checkWrite (uint16_t written);

	void resetAllOps (uint16_t);
	void resetSautConvert (uint16_t);
	void initAfterReset (uint16_t);

	enum Status
	{
		OK				= 0,
		ErrBusy			= 1,
		ErrUnwritten	= 2,
		ErrWrongCrc		= 3,
		ErrUnknownNum	= 4,
		ErrUnknown		= 5
	};
	void endOperation (const Status& status);


	struct Packet
	{
		uint8_t number;
		Complex<uint32_t> data;
	};
	Packet activePacket;

	struct
	{
		Complex<uint32_t>	train; 			// 3
		uint8_t				category;		// 4
		uint8_t				lengthWagon;	// 6
		Complex<uint16_t>	type;			// 11
		uint8_t				vWhite;			// 12
		uint8_t				vRedYellow;		// 13
		uint16_t			blockLength;	// 14
		Complex<uint16_t>	configuration;	// 18
		Complex<uint16_t>	vGreen;			// 19
		uint8_t				trackMPH;		// 23
		struct WrittenFlags
		{
			uint16_t	train			:1;
			uint16_t	category		:1;
			uint16_t	lengthWagon		:1;
			uint16_t	type			:1;
			uint16_t    vWhite			:1;
			uint16_t	vRegYellow		:1;
			uint16_t	blockLength		:1;
			uint16_t	configuration	:1;
			uint16_t	vGreen			:1;
			uint16_t	trackMPH		:1;
		};
		Bitfield<WrittenFlags> written;
	}
	monitoredData;
	uint8_t interrogateCell;
	uint8_t wrongCell;


	uint8_t killerId;
	bool reset;
	bool resetMonitor;
};



template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
ConstValModule<CanDatType, canDat, Scheduler, scheduler>::ConstValModule ()
	: sautConvert (), interrogateCell (128), wrongCell (0), activePacket({0,0}), reset(false)
{
	scheduler.runIn(
			Command {SoftIntHandler::from_method<ConstValModule,&ConstValModule::sendState> (this), 0},
			500 );
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::getWriteMessage (uint16_t getDataPointer)
{
	Packet& packet = *((Packet *) getDataPointer);

	if ( packet.number > 0 && packet.number < 128 )
	{
		// �?звращение КЛУБа
		if ( packet.number == 9 )
			return;
		if ( packet.number == 1 )
			packet.number = 23;

		if (activePacket.number == 0) // свободны
		{
			activePacket.number = packet.number;
			activePacket.data[0] = packet.data[3];
			activePacket.data[1] = packet.data[2];
			activePacket.data[2] = packet.data[1];
			activePacket.data[3] = packet.data[0];

			killerId = scheduler.runIn(
								Command {SoftIntHandler::from_method<ConstValModule, &ConstValModule::resetAllOps>(this), 0},
								400 );
			eeprom.club.cell[packet.number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isWritten>(this) );
		}
		else
		{
			if (reg.portB.pin7 == 0) // первый полукомплект
				canDat.template send<CanTx::SYS_DATA_A> ({uint8_t(packet.number|0x80), uint8_t(Status::ErrBusy), 0, 0, 0});
			else
				canDat.template send<CanTx::SYS_DATA_B> ({uint8_t(packet.number|0x80), uint8_t(Status::ErrBusy), 0, 0, 0});
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::getLeftDataMessage (uint16_t getDataPointer)
{
	Packet& packet = *((Packet *) getDataPointer);

	// �?звращение КЛУБа
	if ( packet.number == 1 ) // Только по адресу 1 принимать левые данные
	{
		if (activePacket.number == 0) // свободны
		{
			activePacket.number = packet.number;
			activePacket.data[0] = packet.data[3];
			activePacket.data[1] = packet.data[2];
			activePacket.data[2] = packet.data[1];
			activePacket.data[3] = packet.data[0];

			killerId = scheduler.runIn(
								Command {SoftIntHandler::from_method<ConstValModule, &ConstValModule::resetAllOps>(this), 0},
								400 );
			eeprom.club.cell[packet.number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isWritten>(this) );
		}
		else
		{
			if (reg.portB.pin7 == 0) // первый полукомплект
				canDat.template send<CanTx::SYS_DATA_A> ({uint8_t(packet.number|0x80), uint8_t(Status::ErrBusy), 0, 0, 0});
			else
				canDat.template send<CanTx::SYS_DATA_B> ({uint8_t(packet.number|0x80), uint8_t(Status::ErrBusy), 0, 0, 0});
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::getQueryMessage (uint16_t getDataPointer)
{
	uint8_t& number = *((uint8_t *) getDataPointer);

	if (number != 1) // По 1 я не работаю
	{
		if (activePacket.number == 0) // свободны
		{
			activePacket.number = number;

			killerId = scheduler.runIn(
								Command {SoftIntHandler::from_method<ConstValModule, &ConstValModule::resetAllOps>(this), 0},
								400 );
			eeprom.club.cell[number].isWritten( SoftIntHandler::from_method<ConstValModule, &ConstValModule::read> (this) );
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::isWritten (uint16_t res)
{
	if (!reset)
	{
		if (res)
			confirm (0);
		else
			write (0);
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::isGoodWhenWrite (uint16_t res)
{
	if (!reset)
	{
		if (res)
			confirm (1);
		else
			write (0);
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::isGoodWhenRead (uint16_t res)
{
	if (!reset)
	{
		if (res)
			read (2);
		else
		{
			scheduler.deleteId (killerId);
			endOperation(Status::ErrWrongCrc);
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::confirm (uint16_t tryNumber)
{
	if (!reset)
	{
		uint32_t savedData;
		if ( eeprom.club.cell[activePacket.number].read (savedData) )
		{
			if (savedData == activePacket.data)
			{
				sautWrite (true);
			}
			else
				write (0);
		}
		else
		{
			if (tryNumber > 0)
				dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule::confirm> (this), tryNumber );
			else
				eeprom.club.cell[activePacket.number].isGood( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isGoodWhenWrite>(this) );
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::write (uint16_t)
{
	if (!reset)
	{
		if ( !eeprom.club.cell[activePacket.number].write( activePacket.data,
															SoftIntHandler::from_method <ConstValModule, &ConstValModule::sautWrite> (this) ) )
			dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule::write> (this), 0);
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::read (uint16_t written)
{
	if (!reset)
	{
		if (written)
		{
			if ( eeprom.club.cell[activePacket.number].read(activePacket.data) )
			{
				scheduler.deleteId (killerId);
				endOperation (Status::OK);
			}
			else
			{
				if (written == 1) // первая попытка
					eeprom.club.cell[activePacket.number].isGood( SoftIntHandler::from_method<ConstValModule, &ConstValModule::isGoodWhenRead>(this) );
				else
					dispatcher.add( SoftIntHandler::from_method <ConstValModule, &ConstValModule::read> (this), 1 );
			}
		}
		else
		{
			scheduler.deleteId (killerId);
			endOperation (Status::ErrUnwritten);
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::sautWrite (uint16_t status)
{
	if (!reset)
	{
		if (status == true)
		{
			sautConvert.updateCell (activePacket.number, activePacket.data,
					SoftIntHandler::from_method<ConstValModule, &ConstValModule::successEnd>(this) );
		}
		else
		{
			scheduler.deleteId (killerId);
			endOperation(Status::ErrUnknown);
		}
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::successEnd (uint16_t )
{
	scheduler.deleteId (killerId);
	endOperation(Status::OK);
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::sendState (uint16_t )
{
	if (!resetMonitor) // Нормальный режим работы
	{
		if (interrogateCell != 128) // Предыдущий опрос не завершён
		{
			resetMonitor = true;
			return;
		}

		// Вывод

		// Сообщение sysDataState выводится даже если не все, содержащиеся в нём данные записаны,
		// потому что оно содержит результат самодиагностики
		uint8_t sysDataState[7] = {
				(wrongCell == 0) ? uint8_t(0) : uint8_t( (wrongCell << 1) | 1 ), // Результаты выполнения тестов...
				monitoredData.configuration[0],
				monitoredData.category,
				monitoredData.vRedYellow,
				monitoredData.vGreen[0],
				uint8_t( (monitoredData.vGreen[1] << 6) | (monitoredData.blockLength/100) ),
				monitoredData.lengthWagon
								};
		if (reg.portB.pin7 == 0) // первый полукомплект
			canDat.template send<CanTx::SYS_DATA_STATE_A> (sysDataState);
		else
			canDat.template send<CanTx::SYS_DATA_STATE_B> (sysDataState);


		if ( monitoredData.written.configuration &&
			 monitoredData.written.type &&
			 monitoredData.written.vWhite )
		{
			uint8_t sysDataState2[8] = {
					0, // Результаты выполнения тестов... здесь не выводим
					monitoredData.configuration[1],
					monitoredData.configuration[0],
					monitoredData.type[1],
					monitoredData.type[0],
					monitoredData.vWhite,
					0,
					0
									};
			if (reg.portB.pin7 == 0) // первый полукомплект
				canDat.template send<CanTx::SYS_DATA_STATE2_A> (sysDataState2);
			else
				canDat.template send<CanTx::SYS_DATA_STATE2_B> (sysDataState2);
		}

		if ( monitoredData.written.trackMPH &&
			 monitoredData.written.train )
		{
			uint8_t ipdParam[8] = {
					monitoredData.trackMPH,
					monitoredData.train[0],
					monitoredData.train[1],
					0,
					0,
					0,
					0,
					0
								};
			if (reg.portB.pin7 == 0) // первый полукомплект
				canDat.template send<CanTx::IPD_PARAM_A> (ipdParam);
			else
				canDat.template send<CanTx::IPD_PARAM_B> (ipdParam);
		}

		if ( monitoredData.written.train &&
			 monitoredData.written.category )
		{
			uint8_t mphState[4] = {
					monitoredData.train[2],
					monitoredData.train[1],
					monitoredData.train[0],
					monitoredData.category
								};
			if (reg.portB.pin7 == 0) // первый полукомплект
				canDat.template send<CanTx::MPH_STATE_A> (mphState);
			else
				canDat.template send<CanTx::MPH_STATE_B> (mphState);
		}
	}
	else // Вызов функции с включенным resetMonitor означает конец сброса
	{
		resetMonitor = false;
	}

	// �?нициализация
	monitoredData.written = 0xFFFF; // Флаги скидываются, если ячейки не записаны
	interrogateCell = 1;
	wrongCell = 0;
	eeprom.club.cell[interrogateCell].isWritten(
			SoftIntHandler::from_method<ConstValModule, &ConstValModule::checkWrite> (this) );

	scheduler.runIn(
			Command {SoftIntHandler::from_method<ConstValModule,&ConstValModule::sendState> (this), 0},
			500 );
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::checkWrite (uint16_t written)
{
	if (!resetMonitor)
	{
		if (written)
			eeprom.club.cell[interrogateCell].isGood(
				 SoftIntHandler::from_method<ConstValModule, &ConstValModule::checkNext>(this) );
		else
			checkNext (2);
	}
	else
	{
		sendState (0);
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::checkNext (uint16_t resPrev)
{
	if (!resetMonitor)
	{
		if (resPrev == 1) // записан, и без ошибок
		{
			bool readSuccess = true;
			uint32_t tmp = 0;
			if (interrogateCell == 3)
			{
				readSuccess = eeprom.club.property.train.read (tmp);
				monitoredData.train = tmp;
			}
			else if (interrogateCell == 4)
			{
				readSuccess = eeprom.club.property.category.read (tmp);
				monitoredData.category = tmp;
			}
			else if (interrogateCell == 6)
			{
				readSuccess = eeprom.club.property.lengthWagon.read (tmp);
				monitoredData.lengthWagon = uint8_t(tmp);
			}
			else if (interrogateCell == 11)
			{
				readSuccess = eeprom.club.property.typeLoco.read (tmp);
				monitoredData.type = tmp;
			}
			else if (interrogateCell == 12)
			{
				readSuccess = eeprom.club.property.vWhite.read (tmp);
				monitoredData.vWhite = tmp;
			}
			else if (interrogateCell == 13)
			{
				readSuccess = eeprom.club.property.vRedYellow.read (tmp);
				monitoredData.vRedYellow = uint8_t(tmp);
			}
			else if (interrogateCell == 14)
			{
				readSuccess = eeprom.club.property.blockLength.read (tmp);
				monitoredData.blockLength = uint16_t(tmp);
			}
			else if (interrogateCell == 18)
			{
				readSuccess = eeprom.club.property.configuration.read (tmp);
				monitoredData.configuration = uint16_t(tmp);
			}
			else if (interrogateCell == 19)
			{
				readSuccess = eeprom.club.property.vGreen.read (tmp);
				monitoredData.vGreen = uint16_t(tmp);
			}
			else if (interrogateCell == 23)
			{
				readSuccess = eeprom.club.property.trackMPH.read (tmp);
				monitoredData.trackMPH = uint8_t(tmp);
			}

			if (!readSuccess)
			{
				dispatcher.add( SoftIntHandler::from_method<ConstValModule, &ConstValModule::checkNext>(this), 1 );
				return;
			}
		}
		else // не записан или записан с ошибками
		{
			if (interrogateCell == 3)
				monitoredData.written.train = 0;
			else if (interrogateCell == 4)
				monitoredData.written.category = 0;
			else if (interrogateCell == 6)
				monitoredData.written.lengthWagon = 0;
			else if (interrogateCell == 11)
				monitoredData.written.type = 0;
			else if (interrogateCell == 12)
				monitoredData.written.vWhite = 0;
			else if (interrogateCell == 13)
				monitoredData.written.vRegYellow = 0;
			else if (interrogateCell == 14)
				monitoredData.written.blockLength = 0;
			else if (interrogateCell == 18)
				monitoredData.written.configuration = 0;
			else if (interrogateCell == 19)
				monitoredData.written.vGreen = 0;
			else if (interrogateCell == 23)
				monitoredData.written.trackMPH = 0;

			if (resPrev == 0) // записан, но ошибка в crc
				wrongCell = interrogateCell;
		}

		if (++interrogateCell < 128)
			eeprom.club.cell[interrogateCell].isWritten(
					 SoftIntHandler::from_method<ConstValModule, &ConstValModule::checkWrite>(this) );
	}
	else
	{
		sendState (0);
	}
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::endOperation (const Status& status)
{
	if (status == Status::OK)
	{
		if (reg.portB.pin7 == 0) // первый полукомплект
			canDat.template send<CanTx::SYS_DATA_A> ({activePacket.number, activePacket.data[3], activePacket.data[2], activePacket.data[1], activePacket.data[0]});
		else
			canDat.template send<CanTx::SYS_DATA_B> ({activePacket.number, activePacket.data[3], activePacket.data[2], activePacket.data[1], activePacket.data[0]});
	}
	else
	{
		if (reg.portB.pin7 == 0) // первый полукомплект
			canDat.template send<CanTx::SYS_DATA_A> ({uint8_t(activePacket.number|0x80), uint8_t(status), 0, 0, 0});
		else
			canDat.template send<CanTx::SYS_DATA_B> ({uint8_t(activePacket.number|0x80), uint8_t(status), 0, 0, 0});
	}

	activePacket.number = 0;
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::resetAllOps (uint16_t)
{
	reset = true;
	eeprom.club.cell[activePacket.number].reset( SoftIntHandler::from_method<ConstValModule, &ConstValModule::resetSautConvert>(this) );
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::resetSautConvert (uint16_t)
{
	sautConvert.reset( SoftIntHandler::from_method<ConstValModule, &ConstValModule::initAfterReset>(this) );
}

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler >
void ConstValModule<CanDatType, canDat, Scheduler, scheduler>::initAfterReset (uint16_t)
{
	reset = false;
	endOperation (Status::ErrBusy);
}



#endif /* MPH_H_ */
