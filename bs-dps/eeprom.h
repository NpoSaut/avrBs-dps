/*
 * eeprom.h
 *
 *  Created on: 18.07.2011
 *      Author: nazemnyh
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/eeprom.h>
#include <cpp/io.h>

struct Eeprom
{
	struct Saut
	{
		uint16_t N_Str_1;   // номер строки		--?
		uint8_t DiameterAvarage;  // Фактический диаметр бандажа в мм (максимальный из двух: ДПС1 или ДПС2, деленный на 10, плюс 1).
		uint8_t V_Max_P;  // Vmax пасс. + Vкж пасс.		--?
		uint8_t V_Max_G;  // Vmax груз. + Vкж груз.		--?
		uint8_t V_Max_E;  // Vmax эл.поезд. + Vкж эл.поезд.		--?
		uint8_t S_min;    // Минимальная длина блок-участка (деленная на 10).В БЭК2 не используется, в БЭК не используется начиная с версии 33.			--? тоже ли самое
		uint8_t Tip_Loc;  // Тип локомотива.		-- ? тоже ли самое
		uint8_t D_Ban_Sr; // Средний диаметр бандажа данного типа локомотива (деленный на 10).		-- ?
		struct Configuration
		{
			enum DpsPosition { Left = 0, Rigth = 1 };
			DpsPosition	dps0Position	:1;
			DpsPosition	dps1Position	:1;
			uint8_t		eks				:1;									// --?
			uint8_t		tap				:1;	// Кран машиниста КМ-130 (1)		--?
			uint8_t		club			:1;									// --?
			enum { CL = 0, KPD = 1 } velocityGauge	:1;						// --?
			enum { ALS = 0, CKR = 1 } ifSignal		:1;						// --?
			enum { ALSN = 0, CLUB = 1 } alarmSystem	:1;						// --?
		};
		Bitfield<Configuration> configuration;
		uint16_t Rez_1;    // Резерв
		uint8_t DiameterCorrection[2];  // Уточнение диаметров бандажей соответственно для ДПС1 и ДПС2. Вычисляются как разность между точным значением
										// соответствующего диаметра бандажа в миллиметрах и значением байта номер 2, умноженным на 10.
										// Седьмой бит - знаковый. -- ВНИМАНИЕ -- это не дополнительный код
		uint16_t KS_1;     // Контрольная сумма первой строки					--?
		uint16_t N_Str_2;  // Номер второй строки (601h или 401h).				--?
		uint8_t Tip_el1;  // Название локомотива (DOS - кодировка), дополненное пробелами (символами с кодом 20h).
		uint8_t Tip_el2;
		uint8_t Tip_el3;
		uint8_t Tip_el4;
		uint8_t Tip_el5;
		uint8_t Tip_el6;
		uint16_t N_El;    // Номер локомотива (сначала старший байт, затем младший).
		uint8_t Kod_Os;  // Код особенностей локомотива: 1 - ЧС2 (сигнал "Тяга" - от БС-ЦКР);			--?
					//  2 - ЧС4, ЧС4Т, ЧС7, ЭП1, ВЛ65, ВЛ85, ВЛ11 (инверсный сигнал "Тяга");
					//  Остальные локомотивы - 0.
		uint8_t Sekc_lok;// Секция.
		uint8_t V_Konst; // Конструктивная скорость локомотива (км/ч).
		uint8_t Tip;     // Тип (код) подвижной единицы (см. файл "Типы локомотивов").
		uint16_t KS_2;    // Контрольная сумма второй строки.
	} saut; // [0x00 -- 0x1F] : 32 байта

	struct
	{
		uint32_t dummy;			//  0 - для совместимости (счёт идёт с 1)
		uint32_t track; 		//  1 - Номер пути для хранения в МПХ ЭК
		uint32_t driver; 		//  2 - Номер машиниста
		uint32_t train; 		//  3 - Номер поезда
		uint32_t category; 		//  4 - Категория поезда
		uint32_t lengthWheel;	//  5 - Длина состава в осях
		uint32_t lengthWagon; 	//  6 - Длина состава в условных вагонах
		uint32_t number; 		//  7 - Номер локомотива или ведущей секции многосекционного локомотива
		uint32_t weigth; 		//  8 - масса поезда
		uint32_t coordStart;	//  9 - Начальная координата
		uint32_t time;			// 10 - Время
		uint32_t typeLoco; 		// 11 - Тип локомотива
		uint32_t vMax; 			// 12 - Допустимая скорость (на белый)
		uint32_t vRedYellow; 	// 13 - Скорость движения на КЖ
		uint32_t blockLength;	// 14 - Приведённая длина блок-участка «Дозор»
		uint32_t diameter1; 	// 15 - Диаметр бандажа колеса 1, мм
		uint32_t diameter2; 	// 16 - Диаметр бандажа колеса 2, мм
		uint32_t dpsDentos;		// 17 - Число зубьев ДС
		uint32_t configuration;	// 18 - Конфигурация
		uint32_t vGreen; 		// 19 - Допустимая скорость на Зелёный
		uint32_t dirCoord; 		// 20 - Направление изменения координаты
		uint32_t milage; 		// 21 - Пробег локомотива
		uint32_t clsdVersion;	// 22 - Информация о версии КЛУБ
		uint32_t trackVpd; 		// 23 - Номер пути для хранения в МПХ ВПД
		uint32_t vpdPrivate; 	// 24 - Параментр используемый только внутри программы ВПД-М
		uint32_t bilBrightnes;  // 25 - Параметр яркости для модуля БИЛ
		uint32_t snsPosition1;	// 26 - Расположение СНС первой кабины
		uint32_t sndPosition2;	// 27 - Расположение СНС второй кабины
		//-----------------------Запись по MCO_DATA---------------------------------------------------------
		uint32_t ufirOutConf;	// 28 - УФИР выходил из конфигурации
		uint32_t tskbmOutConf;	// 29 - ТСКБМ выходил из конфигурации
		uint32_t sautOutConf;	// 30 - САУТ выходил из конфигурации
		uint32_t bilOutConf;	// 31 - БИЛ выходил из конфигурации
		uint32_t ipdOutConf;	// 32 - ИПД выходил из конфигурации
		uint32_t bvuOutConf;	// 33 - БВУ выходил из конфигурации
		uint32_t mmOutConf;		// 34 - ММ выходил из конфигурации
		uint32_t ecOutConf;		// 35 - ЭК выходил из конфигурации
		//-----------------------Запись по BKSI_DATA--------------------------------------------------------
		uint32_t krError;		// 36 - Наличие и исправность КР
		uint32_t dps1Error;		// 37 - Обрыв ДПС1
		uint32_t dps2Error;		// 38 - Обрыв ДПС2
		uint32_t snsError;		// 39 - Нет данных СНС
		uint32_t epkError;		// 40 - Обрыв ЭПК
		uint32_t konError;		// 41 - Обрыв КОН
		uint32_t epvError;		// 42 - Обрыв ЭПВ
		uint32_t eptError;		// 43 - Неисправен ЭПТ
		uint32_t pkm1TvError;	// 44 - Обрыв ПКМ1 ТВ
		uint32_t pkm1OvError;	// 45 - Обрыв ПКМ1 ОВ
		uint32_t pkm2TvError;	// 46 - Обрыв ПКМ2 ТВ
		uint32_t pkm2OvError;	// 47 - Обрыв ПКМ2 ОВ
	} club; // [0x20 -- 0xE0] : 192 байта

	struct // отступ в памяти (резерв)
	{
		uint8_t empty[128];
	} dummy; // [0xE0 -- 0xFF] : 128 байт

	//uint8_t	oldStyleClubEeprom; // 0x100...

	// Исправности датчиков
	uint8_t dps0Good;
	uint8_t dps1Good;

} eeprom EEMEM;


#endif /* EEPROM_H_ */
