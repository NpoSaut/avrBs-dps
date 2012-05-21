/*
 * eeprom.h
 *
 *  Created on: 18.07.2011
 *      Author: nazemnyh
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <cpp/eeprom.h>
#include <cpp/io.h>

struct Eeprom
{
	struct Saut
	{
		Eeprom<uint16_t>	string1Number1;   			// номер строки		--?
		Eeprom<>uint8_t>	diameterAvarage; 			// Фактический диаметр бандажа в мм (максимальный из двух: ДПС1 или ДПС2, деленный на 10, плюс 1).
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

	Eeprom<uint32_t>		dummy;						//  0 - для совместимости (счёт идёт с 1)
	Eeprom<uint32_t>		track; 						//  1 - Номер пути для хранения в МПХ ЭК
	Eeprom<uint32_t>		driver; 					//  2 - Номер машиниста
	Eeprom<uint32_t>		train; 						//  3 - Номер поезда
	Eeprom<uint32_t>		category; 					//  4 - Категория поезда
	Eeprom<uint32_t>		lengthWheel;				//  5 - Длина состава в осях
	Eeprom<uint32_t>		lengthWagon; 				//  6 - Длина состава в условных вагонах
	Eeprom<uint32_t>		number; 					//  7 - Номер локомотива или ведущей секции многосекционного локомотива
	Eeprom<uint32_t>		weigth; 					//  8 - масса поезда
	Eeprom<uint32_t>		coordStart;					//  9 - Начальная координата
	Eeprom<uint32_t>		time;						// 10 - Время
	Eeprom<uint32_t>		typeLoco; 					// 11 - Тип локомотива
	Eeprom<uint32_t>		vMax; 						// 12 - Допустимая скорость (на белый)
	Eeprom<uint32_t>		vRedYellow; 				// 13 - Скорость движения на КЖ
	Eeprom<uint32_t>		blockLength;				// 14 - Приведённая длина блок-участка «Дозор»
	Eeprom<uint32_t>		diameter1; 					// 15 - Диаметр бандажа колеса 1, мм
	Eeprom<uint32_t>		diameter2; 					// 16 - Диаметр бандажа колеса 2, мм
	Eeprom<uint32_t>		dpsDentos;					// 17 - Число зубьев ДС
	Eeprom<uint32_t>		configuration;				// 18 - Конфигурация
	Eeprom<uint32_t>		vGreen; 					// 19 - Допустимая скорость на Зелёный
	Eeprom<uint32_t>		dirCoord; 					// 20 - Направление изменения координаты
	Eeprom<uint32_t>		milage; 					// 21 - Пробег локомотива
	Eeprom<uint32_t>		clsdVersion;				// 22 - Информация о версии КЛУБ
	Eeprom<uint32_t>		trackMPH; 					// 23 - Номер пути для хранения в МПХ ВПД
	Eeprom<uint32_t>		vpdPrivate; 				// 24 - Параментр используемый только внутри программы ВПД-М
	Eeprom<uint32_t>		bilBrightnes;  				// 25 - Параметр яркости для модуля БИЛ
	Eeprom<uint32_t>		snsPosition1;				// 26 - Расположение СНС первой кабины
	Eeprom<uint32_t>		sndPosition2;				// 27 - Расположение СНС второй кабины
	//-----------------------Запись по MCO_DATA---------------------------------------------------------
	Eeprom<uint32_t>		ufirOutConf;				// 28 - УФИР выходил из конфигурации
	Eeprom<uint32_t>		tskbmOutConf;				// 29 - ТСКБМ выходил из конфигурации
	Eeprom<uint32_t>		sautOutConf;				// 30 - САУТ выходил из конфигурации
	Eeprom<uint32_t>		bilOutConf;					// 31 - БИЛ выходил из конфигурации
	Eeprom<uint32_t>		ipdOutConf;					// 32 - ИПД выходил из конфигурации
	Eeprom<uint32_t>		bvuOutConf;					// 33 - БВУ выходил из конфигурации
	Eeprom<uint32_t>		mmOutConf;					// 34 - ММ выходил из конфигурации
	Eeprom<uint32_t>		ecOutConf;					// 35 - ЭК выходил из конфигурации
	//-----------------------Запись по BKSI_DATA--------------------------------------------------------
	Eeprom<uint32_t>		krError;					// 36 - Наличие и исправность КР
	Eeprom<uint32_t>		dps1Error;					// 37 - Обрыв ДПС1
	Eeprom<uint32_t>		dps2Error;					// 38 - Обрыв ДПС2
	Eeprom<uint32_t>		snsError;					// 39 - Нет данных СНС
	Eeprom<uint32_t>		epkError;					// 40 - Обрыв ЭПК
	Eeprom<uint32_t>		konError;					// 41 - Обрыв КОН
	Eeprom<uint32_t>		epvError;					// 42 - Обрыв ЭПВ
	Eeprom<uint32_t>		eptError;					// 43 - Неисправен ЭПТ
	Eeprom<uint32_t>		pkm1TvError;				// 44 - Обрыв ПКМ1 ТВ
	Eeprom<uint32_t>		pkm1OvError;				// 45 - Обрыв ПКМ1 ОВ
	Eeprom<uint32_t>		pkm2TvError;				// 46 - Обрыв ПКМ2 ТВ
	Eeprom<uint32_t>		pkm2OvError;				// 47 - Обрыв ПКМ2 ОВ
	// [0x20 -- 0xE0] : 192 байта

	Eeprom<uint8_t>			 empty[128];				// [0xE0 -- 0xFF] : 128 байт - отступ в памяти (резерв)

	// Исправности датчиков
	Eeprom<uint8_t> 		dps0Good;
	Eeprom<uint8_t>			dps1Good;

} eeprom EEMEM;


#endif /* EEPROM_H_ */
