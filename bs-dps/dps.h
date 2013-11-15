/*
 * dps.h
 *
 *	Работа с датчиком пути и скорости
 *	*********************************
 *
 *	~~~ Задача: ~~~
 *	Датчик пути и скорости (ДПС) передаёт
 *
 *
 *
 *
 */

#ifndef DPS_H_
#define DPS_H_

#include <avr/eeprom.h>
#include <stdint.h>

#include <cpp/timers.h>
#include <cpp/universal.h>
#include <cpp/io.h>
#include <cpp/dispatcher.h>
#include <cpp/interrupt-dynamic.h>

#include "hw_defines.h"

#include "CanDat.h"
#include "CanDesriptors.h"
#include "mph.h"
#include "ec-adjust.h"

// Датчик пути и скорости
// ----------------------
//	Обеспечивает подсчёт скорости и ускорения,
//	основываясь на регулярно поступающих данных о состоянии каналов ДПС
//		ДПС за оборот колеса диаметром diametros (в мм)
//		равномерно выдаёт 42 имульса в каждый канал.
//		Импульсы в каналах смещены на пол-импульса, что даёт возможность определять направление вращения
//	ВХОД:
//	- раз в period мкс должна вызываться функция punctum (affectus),
//		которая в качестве аргумента принимает состояния двух каналов ДПС: 2 младших бита
//	ВЫХОД:
//	- скорость const uint16_t& celeritas () - в единицах: км/ч/128
//	- ускорение uint8_t acceleratio () - в единицах: м/c/128, старший бит - знаковый (прямой код)
//	- направление вращения versus
//	- информацию о исправности каналов:
//		- ипульсы по каналам должны чередоваться; если это не так, то выставляется неисправность isValidus()
//		- есть возможность выставить или снять неисправность всему ДПСу через setCausarius() и setValidus()
//	- На пины-светодиоды lanterna0 и lanterna1, если выставлен флаг lanternaOperor выводится:
//		в процессе движения: информация о состоянии каналов с частостой 1/16
//		в режиме остановке: исправность каналов ДПСа
template<Port Register::*lanternaPortus, uint8_t lanterna0, uint8_t lanterna1, uint16_t minTempusPunctum,
		uint16_t maxTempusPunctum, uint16_t maxCeleritas, uint32_t period>
class Dimetior {
public:
	Dimetior (bool lanternaOperor) :
			lanternaOperor (lanternaOperor), tractus (false), versusInversio (false),
			celeritas (0), acceleratio (0), acceleratioColum (0),
			impulsio ({ 0, 0 }), impulsioLanterna ({ 0, 0 }), tempusPunctum ({ 0, 0 }),
			affectus (0), versusRotatio ({ !positio, !positio }), versusDynamic(0),
			causarius (false), versusCausarius (false), commoratio (true)
	{
//		if (lanternaOperor)
//		{
		(reg.*lanternaPortus).pin<lanterna0> ().out ();
		(reg.*lanternaPortus).pin<lanterna1> ().out ();
//		}

		constituoDiametros (1250);
		constituoCogs (42);
		positio = EepromData::DpsPosition::Left;
	}

	// Функция должна вызываться с периодом period (мкс)
	// принимает состояние каналов
	const uint32_t punctum (uint8_t affectusNovus) __attribute__ ((noinline))
	{
		tempusPunctum[0]++;
		tempusPunctum[1]++;

		// Получение состояния порта и нахождение фронта по каждому каналу
		affectusNovus &= 0b11; // Обрезать лишнее
		uint8_t affectusCommutatio = (affectus ^ affectusNovus) & affectusNovus; // 1 - если состояние изменилось с 0 на 1 т.е. фронт
		affectus = affectusNovus;

		if (affectusCommutatio) // если случился фронт
		{
			uint32_t longitudo = 0;

			// каналы, по которым произошёл подъём
			for ( uint8_t canalis = 0; canalis < 2; canalis ++  )
			{
				if (affectusCommutatio & (1 << canalis))
				{
					impulsio[canalis]++;
					impulsioLanterna[canalis]++;

					if (impulsio[canalis] == 1)
						tempusPunctum[canalis] = 0; // Начинаем считать время с 1-го импульса
													// После остановки (оба счётчика = 0) необходимо 2 импульса по одному и 1 по другому,
													// для того, чтобы вывести скорость


					if (impulsio[canalis] > impulsio[!canalis]) // Метры идут только по одному каналу. По большему.
						longitudo = longitudoImpulsio; // при переключении будет небольшая погрешность в большую сторону.

					// Определение направления движения
					uint8_t vr = ((affectus + canalis) / 2) & 1;
					if ( vr != versusDynamic )
						if ( impulsio[canalis] != 1 )
							versusCausarius = true;
					versusDynamic = vr;


					if (tempusPunctum[canalis] >= minTempusPunctum && // Прошло достаточно времени для точного определения скорости
							(tractus || impulsio[canalis] >= 4)) // В режиме выбега повышаем порог чувствительности
					{

						causarius = (abs(impulsio[canalis] - impulsio[!canalis]) > 1)  // Не было нормального чередования
									|| versusCausarius; // Направление менялось в течении измерения

						if (versusDynamic == versusRotatio.retro) // Направление "применяется" только после подтверждения
							versusRotatio.modo = versusRotatio.retro; //  чтобы исключить 1-импульсные дёрганья в момент трогания/остановки
						versusRotatio.retro = versusDynamic;

						debugImpulsio[0] = impulsio[0];
						debugImpulsio[1] = impulsio[1];

						computo (canalis);

						commoratio = false;

						// Для нового расчёта
						impulsio[canalis] = 1; // Сам начинаю считать время от текущего импульса
						tempusPunctum[canalis] = 0;
						impulsio[!canalis] = 0; // А сосед пусть сначала дождётся импульса и тогда начнёт считать время
						tempusPunctum[!canalis] = 0;
						versusCausarius = false;
					}

					// Мигание светодиодами
					if (lanternaOperor)
						if (impulsioLanterna[canalis] % 16 == 0) // мигать с периодом 1/16
						{
							if (canalis)
								(reg.*lanternaPortus).pin<lanterna1> ().toggle ();
							else
								(reg.*lanternaPortus).pin<lanterna0> ().toggle ();
						}
				}
			}

			return longitudo;
		}
		else // Фронта не было
		{
			for (uint8_t can = 0; can < 2; ++can)
				if (tempusPunctum[can] > maxTempusPunctum) // Фронта не было долго
					commoratioCanalis (can);
			return 0;
		}
	}

	void constituoTractus ()
	{
		tractus = true;
	}
	void constituoNonTractus ()
	{
		tractus = false;
	}
	void constituoVersusInversio (bool inversio)
	{
		versusInversio = inversio;
	}

	// Скрость в км/ч/128
	const uint16_t& accipioCeleritas () const
	{
		return celeritas;
	}
	// Ускорение выбранного канала в м/c/128, знак в прямом коде
	uint8_t accipioAcceleratio () const
	{
		int8_t a;
		if (acceleratio > 127)
			a = 127;
		else if (acceleratio < -127)
			a = -127;
		else
			a = acceleratio;

		if (a < 0)
			a = ((~a) | 128) + 1;
		return a;
	}

	// Направление движения. 0 - вперёд
	bool accipioVersus () const
	{
		return (versusInversio ^ !(versusRotatio.modo ^ positio));
	}
	// Остановка
	bool sicinCommoratio () const
	{
		return commoratio;
	}
	// Неисправность (недостоверность)
	bool sicinCausarius () const
	{
		return causarius;
	}

	// Диаметр
	uint16_t accipioDiametros () const
	{
		return diametros;
	}
	void constituoDiametros (const uint16_t& diametros)
	{
		if (Dimetior::diametros != diametros)
		{
			Dimetior::diametros = diametros;
			// Расчитываем длину имульса (в единицах: дм/65536)
			// L = Pi * d(мм) / cogs / 100 * 65536
			// d: 2966 - 200  => d*65536*Pi помещается в uint32_t
			// Pi * 65536 = 205887,416172544
			longitudoImpulsio = (uint32_t (diametros) * 205887) / ( cogs * 100);
		}
	}

	// Число импульсов на оборот колеса
	uint16_t accipioCogs () const
	{
		return cogs;
	}
	void constituoCogs (const uint16_t& cogs)
	{
		if (Dimetior::cogs != cogs)
		{
			Dimetior::cogs = cogs;
			// Расчитываем длину имульса (в единицах: дм/65536)
			// L = Pi * d(мм) / cogs / 100 * 65536
			// d: 2966 - 200  => d*65536*Pi помещается в uint32_t
			// Pi * 65536 = 205887,416172544
			longitudoImpulsio = (uint32_t (diametros) * 205887) / ( cogs * 100);
		}
	}

	EepromData::DpsPosition positio;

	// --- ДЛЯ ОТЛАДКИ ---
//	uint8_t retroCan; // последний канал, по которому производился расчёт
//	uint8_t vicisNum; // кол-во переключений между каналами
	// --- КОНЕЦ ---

private:
	enum {
		maxCeleritasError = maxCeleritas / minTempusPunctum
	};
	const bool lanternaOperor;
	bool tractus; // 0 - выбег или торможение, 1 - тяга
	bool versusInversio;

	uint32_t longitudoImpulsio; // Длина, которую колесо проходит за один импульс (в единицах: дм/65536)
	uint16_t diametros; // Диаметр
	uint16_t cogs; // Число импульсов на оборот колеса
	uint16_t celeritas; // Скорость по показаниям канала
	int16_t acceleratio; // Ускорение по показаниям канала
	int16_t acceleratioColum; // Промежуточные коэф-ты в фильтре ускорения
	uint16_t impulsio[2]; // Кол-во импульсов, пришедших по каналу
	uint8_t impulsioLanterna[2]; // Это кол-во не обнуляется, чтобы корректно моргать лампочками
	uint16_t tempusPunctum[2];
	uint8_t affectus; // состояние порта
	struct VersusRotatio // Напрвление вращения (true - туда (по часовой стрелке), false - обратно)
	{
		uint8_t modo :1; // Сейчас
		uint8_t retro :1; // В прошлый раз (для контроля)
	};
	Bitfield<VersusRotatio> versusRotatio;
	uint8_t versusDynamic;
	bool causarius; // Испорченность (недостоверность данных)
	bool versusCausarius; // При изменении направления вращения
	bool commoratio; // Остановка

	uint16_t debugImpulsio[2];

	// Выставляет режим остановки
	void commoratioCanalis (const uint8_t& can) __attribute__ ((noinline))
	{
		tempusPunctum[can] = 0;
		impulsio[can] = 0;
		celeritas = 0;
		commoratio = true;
		acceleratio = 0;
		acceleratioColum = 0;
		causarius = false;
		versusCausarius = false;
	}

	void computo (const uint8_t& can) __attribute__ ((noinline))
	{
//		// Для отладки --- УБРАТЬ
//		if (retroCan != can)
//			vicisNum++;
//		retroCan = can;
//		// конец --- для отладки -- УБРАТЬ

		// Считаем скорость
		uint16_t celeritasNovus = (((uint32_t (diametros) * 1447656) / period / cogs) * (impulsio[can] - 1))
				/ tempusPunctum[can];

		// Считаем ускорение
		// Предотвращаем перегрузку (A не более 4м/c^2)
		int16_t celeritasCommutatio = int16_t (celeritasNovus) - int16_t (celeritas);
		if (celeritasCommutatio > 749)
			celeritasCommutatio = 749;
		else if (celeritasCommutatio < -749)
			celeritasCommutatio = -749;

		int16_t acceleratioNovusX8 = (int32_t (8 * 277778 / int32_t (period)) * celeritasCommutatio)
				/ tempusPunctum[can];

		// Для минимизации пульсаций ускорения применяется рекурсивный цифровой эллиптический НЧ фильтр 3-го порядка
		// С частотой среза 0,3 Гц (при дискрете 4Гц) и подавлением 40 дб в полосе непропускания
		// Время релаксации порядка 4 сек.
		// На 512 км/ч при ускорении 0,01 м/c фильтр даёт пульсации +-0,01 м/c
		// Коэффициенты разностного уравнения: b0 = 1, b1 = 1, a0 = 1, a1 = -3/4, gain = 1/8
		int16_t colum = acceleratioNovusX8 + (acceleratioColum / 4) * 3;
		acceleratio = (colum + acceleratioColum) / (8 * 8);
		acceleratioColum = colum;

		celeritas = celeritasNovus;

	}
};

// Cкоростемер
// -----------

template < Port Register::*lanternaPortus, uint8_t lanterna0, uint8_t lanterna1, Port Register::*semiSynthesisPortus, uint8_t semiSynthesisPes,
typename CanType, CanType& canDat,
typename ClockType, ClockType& clock,
typename Scheduler, Scheduler& scheduler >
class CeleritasSpatiumDimetior {
public:
	CeleritasSpatiumDimetior (Port Register::* accessusPortus, uint8_t& spatium, Safe<uint16_t>& celeritas,
			Safe<uint16_t>& acceleratioEtAffectus, InterruptHandler odometer16dm0PlusPlus,
			InterruptHandler odometer16dm1PlusPlus) :
			accessusPortus (accessusPortus), spatiumMeters (0), odometer16dmPlusPlus (
			{ odometer16dm0PlusPlus, odometer16dm1PlusPlus }), tractus (false), repeto (true), // после перезагрузки -- флаг перезагрузки
			ecAdjust (
					Delegate<uint16_t ()>::from_method<CeleritasSpatiumDimetior,
							&CeleritasSpatiumDimetior::accipioCeleritas> (this)
					,
					Delegate<int32_t ()>::from_method<CeleritasSpatiumDimetior,
							&CeleritasSpatiumDimetior::accipioSpatiumMeters> (this)), animadversor (
					InterruptHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::animadversio> (
							this)), spatium (spatium), celeritasProdo (celeritas), acceleratioEtAffectus (
					acceleratioEtAffectus), spatiumDecimeters65536 (0), spatiumDecimetersMultiple10 (10), spatiumDecimetersMulitple16 (
					0), retroRotundatioCeleritas (0), nCapio (0), tempusRestitutioValidus (0), tempusDifferens (0), tempusTractusCommoratio (
					0), activus (0)
	{
		causarius[0] =
		{	0,0,0};
		causarius[1] =
		{	0,0,0};

//		dimetior[0] = new DimetiorType( 0 );
//		dimetior[1] = new DimetiorType( 0 );
		dimetior[0] = new DimetiorType( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 );
		dimetior[1] = new DimetiorType( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 1 );
		accipioConstans (0);

		(reg.*accessusPortus).in ();
		(reg.*lanternaPortus).pin<lanterna0>().out ();
		(reg.*lanternaPortus).pin<lanterna1>().out ();

		// Инициализация линии связи
		acceleratioEtAffectus <<= 0x74;// оба вперёд и исправны. Флаг перезагрузки
		celeritasProdo <<= 0;

		scheduler.runIn(
				Command
				{	SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::produco>(this), 0},
				500 );
	}
	~CeleritasSpatiumDimetior ()
	{
		delete dimetior[0];
		delete dimetior[1];
	}

	void constituoActivus ()
	{
		if (!activus)
		{
			activus = true;
			animadversor.start ();
		}
	}
	void constituoPassivus ()
	{
		if (activus)
		{
			activus = false;
			animadversor.disable ();
		}
	}
	bool sicinActivus () const
	{	return activus;}

	void constituoTractus ()
	{
		tractus = true;
		dimetior[0]->constituoTractus();
		dimetior[1]->constituoTractus();
	}
	void constituoNonTractus ()
	{
		tractus = false;
		dimetior[0]->constituoNonTractus();
		dimetior[1]->constituoNonTractus();
	}

	void constituoVersusInversio ( bool inversio )
	{	dimetior[0]->constituoVersusInversio (inversio); dimetior[1]->constituoVersusInversio (inversio);}

	bool sicinCausarius () const
	{	return causarius[0] && causarius[1];}

	// Скрость в км/ч/256 + старший бит в младшем бите
	const uint16_t celeritas () const
	{
		return signCeleritas( dimetior[nCapio]->accipioCeleritas() );
	}

	// Напрвление движения. 0 - вперёд
	const uint8_t versus () const
	{
		return dimetior[nCapio]->accipioVersus();
	}

	// диаметр бандажа
	const uint16_t diametros (const uint8_t& n) const
	{
		return dimetior[n]->accipioDiametros();
	}

	void takeEcDataForAdjust(uint16_t pointerToData)
	{
		ecAdjust.takeEcData(pointerToData);
	}

	Port Register::* accessusPortus; // Указатель на порт, на битах 0-3 отражается состояние каналов ДПС
	Complex<int32_t> spatiumMeters;// пройденный путь в метрах
	InterruptHandler odometer16dmPlusPlus[2];// Делагаты функций, делающийх ++ к одометрам

	bool repeto;// флаг перезагрузки в линию связи

private:
//	typedef CeleritasSpatiumDimetior< lanternaPortus, lanterna0, lanterna1, semiSynthesisPortus, semiSynthesisPes, CanType, canDat, Scheduler, scheduler > myType;
	// После прошествия этого времени по первому спаду произойдёт подсчёт
	// 1/minTempusPunctum - относительная погрешность определения скорости.
	// Время обновления показания на больших скоростях: minTempusPunctum * animadversor.period
	static constexpr uint16_t minTempusPunctum = 4096;
	// Если в течении maxTempusPunctum отсутсвуют импульсы, то считаем, что скорость = 0.
	// В секундах: maxTempusPunctum * animadversor.period
	static constexpr uint16_t maxTempusPunctum = 16384;
	// Максимальная скорость в единицах (км/ч / 128)
	static constexpr uint16_t maxCeleritas = 65535;
	static constexpr uint8_t maxCeleritasError = maxCeleritas / minTempusPunctum;
	// Максимальное время в единицах productor.period, в течении которого расхождение по датчикам на 25% считается допустимым
	static constexpr uint8_t maxTempusDifferens = 127;
	// Максимальное время, в течении которого допускается отсутсвие скорости по обоим ДПС в режиме Тяга
	static constexpr uint8_t maxTempusTractusCommoratio = 127;
	// Время в единицах productor.period, по истечении которого происходит восстановление испорченности, если не было расхождения более 25%
	static constexpr uint8_t maxTempusRestitutioValidus = 63;

	Alarm<Alarm0, 100> animadversor;
	typedef Dimetior< lanternaPortus, lanterna0, lanterna1, minTempusPunctum, maxTempusPunctum, maxCeleritas, 100 > DimetiorType;
	DimetiorType* dimetior[2];

	typedef EcAdjust < CanType, canDat > EcAdjustType;
	EcAdjustType ecAdjust;

	bool tractus;// 0 - выбег или торможение, 1 - тяга

	uint8_t& spatium;
	Safe<uint16_t>& celeritasProdo;
	Safe<uint16_t>& acceleratioEtAffectus;

	uint32_t spatiumDecimeters65536;// пройденный путь в дм/65536
	uint8_t spatiumDecimetersMultiple10;// путь в дециметрах, кратный 10; для перевода в метры
	uint8_t spatiumDecimetersMulitple16;// путь в 1,6 м. Используется для ++ одометров

	uint16_t retroRotundatioCeleritas;// прошлое округлённое значение скорости. Для нужд округления с гистерезисом.

	bool nCapio;
	uint8_t nMax; // Датчик с максимальной скоростью
	uint8_t tempusRestitutioValidus;// время после последнего сброса показаний исправности
	uint8_t tempusDifferens;// время, в течении которого сохраняется разность показаний ДПС более 25%
	uint8_t tempusTractusCommoratio;// время, в течении которого стоят оба ДПС в режиме Тяга
	uint8_t activus;// 0 - пассивен, 1 - активен

	struct Causarius
	{
		uint8_t vicis :1;
		uint8_t conjuctio :1; // Эти критерии исправности могут быть выставлены только извне
		uint8_t celeritas :1;//
	};
	Bitfield<Causarius> causarius[2];

	// Вызывается c периодом animadversor.period (100 мкс)
	void animadversio ()
	{
		dispatcher.add (
				SoftIntHandler::from_method <CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::corpusVicissim> (this),
//				SoftIntHandler (this, &myType::corpusVicissim),
				(reg.*accessusPortus)
		);
		sei ();// Если прерывание ждёт, то пустить его
				  // Это может быть принципиально.
				  // Т.к. в коммуникаторе после завершения отправки нужно выставить флаг за 18 мкс,
				  // это делается в прерывании
				  // Вход и add здесь - 8,3 мкс, вход туда - 4 мкс. Выход отсюда - 4 мкс.
	}

	void corpusVicissim (uint16_t affectus)
	{
		uint32_t spatium0 = dimetior[0]->punctum (affectus & 0b11);
		uint32_t spatium1 = dimetior[1]->punctum (affectus / 4);
		// накапливать пройденный путь по выбранному датчику
		uint32_t appendicula = nCapio ? spatium1 : spatium0;

		if (appendicula != 0)
		{
			// Выводим в линию связи пройденный путь ...
			spatiumDecimeters65536 += appendicula;
			spatium = uint8_t(spatiumDecimeters65536 >> 16);
			// Обновляем показания одометров
			uint8_t spatiumDecimetersMulitple16New = uint8_t(spatiumDecimeters65536 >> 20);
			if ( spatiumDecimetersMulitple16 != spatiumDecimetersMulitple16New )
			{
				(odometer16dmPlusPlus[0]) ();
				(odometer16dmPlusPlus[1]) ();
			}
			spatiumDecimetersMulitple16 = spatiumDecimetersMulitple16New;

			if ( uint8_t(spatiumDecimeters65536 >> 16) == spatiumDecimetersMultiple10 ) // посчитать в метрах
			{
				spatiumDecimetersMultiple10 += 10;
				if ( versus() == 0 )
				spatiumMeters ++;
				else
				spatiumMeters --;
			}
		}
	}

	// Выводит результат работы датчиков в линию связи и CAN
	void produco (uint16_t)
	{
		if (activus)
		{
			// Анализ показаний датчиков, выбор ДПС, установка неисправности
			nMax =  nMax == 0 ?
					 ( (dimetior[0]->accipioCeleritas() + 64) <  dimetior[1]->accipioCeleritas() )
					:(  dimetior[0]->accipioCeleritas() 	  < (dimetior[1]->accipioCeleritas() + 64) );
			// +64 чтобы предотвратить постоянное переключение

			causarius[0].vicis = dimetior[0]->sicinCausarius();
			causarius[1].vicis = dimetior[1]->sicinCausarius();

			if ( !dimetior[0]->sicinCausarius() && !dimetior[1]->sicinCausarius() )
			{
				if ( dimetior[nMax]->accipioCeleritas() > 1280 &&
						abs( dimetior[nMax]->accipioCeleritas() - dimetior[!nMax]->accipioCeleritas() )
						> dimetior[nMax]->accipioCeleritas()/4 ) // Если разброс большой (и больше 10км/ч)
				{
					if (tempusDifferens == maxTempusDifferens) // довольно давно
					causarius[!nMax].celeritas = true;
					else
					tempusDifferens ++;
				}
				else
				{
					tempusDifferens = 0;

					if ( !dimetior[nCapio]->sicinCommoratio() )
					tempusRestitutioValidus ++;
				}
			}

			if ( !causarius[0] && !causarius[1] )
			{
				bool potentiaCapio = tractus ^ nMax;
				if ( !dimetior[potentiaCapio]->sicinCommoratio() )
				nCapio = potentiaCapio;
				else
				nCapio = !potentiaCapio;

				tempusRestitutioValidus = 0; // Время начинать считать с момента возникновения неисправности
			}
			else
			{
				if ( causarius[0] && !causarius[1] ) // не работает один из каналов
				nCapio = 1;// выбираем рабочий
				else if ( !causarius[0] && causarius[1] )
				nCapio = 0;
				else if ( causarius[0] && causarius[1] )
				nCapio = nMax;

				if ( tempusRestitutioValidus == maxTempusRestitutioValidus )// раз в несколько секунд сбрасывать неисправность
				{
					causarius[0].celeritas = false;
					causarius[1].celeritas = false;
				}
			}

			// Контроль обрыва обоих ДПС
			bool duplarisTractus;
			if ( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 )
				duplarisTractus = ( (canDat.template get<CanRx::MCO_LIMITS_A> ()[7] & 0b11) == 0b11 );// признак двойной тяги
			else
				duplarisTractus = ( (canDat.template get<CanRx::MCO_LIMITS_B> ()[7] & 0b11) == 0b11 );// признак двойной тяги

			if ( tractus && !duplarisTractus )// При тяге
			{
				if ( dimetior[nCapio]->sicinCommoratio() ) // стоим
				{
					if ( tempusTractusCommoratio >= 70*2 ) // В течении времени 70 сек.
					{
						causarius[0].conjuctio = true;
						causarius[1].conjuctio = true;
					}
					else
					tempusTractusCommoratio ++;
				}
				else
				tempusTractusCommoratio = 0;
			}
			else
			{
				if ( tempusTractusCommoratio > 0 )
				tempusTractusCommoratio --;
			}

			// Выставление флагов
			struct Mappa
			{
				uint8_t versus0 :1;
				uint8_t versus1 :1;
				uint8_t commoratio :1;
				uint8_t dimetior :1;
				uint8_t validus0 :1;
				uint8_t validus1 :1;
				uint8_t repeto :1;
				uint8_t :1;
			};
			Bitfield<Mappa> mappa;

			mappa.repeto = repeto;
			mappa.versus0 = dimetior[0]->accipioVersus();
			mappa.versus1 = dimetior[1]->accipioVersus();
			mappa.commoratio = dimetior[nCapio]->sicinCommoratio();
			mappa.dimetior = nCapio;
			// Неисправность != недостоверность
			// Неисправность - это недостверность при достаточно большой скорости
			// Потому что при смене направления и дребезге на стоянке возникает недостоверность
			bool firmusCausarius[2] =
			{	( causarius[0].vicis
						&& dimetior[0]->accipioCeleritas() > 128*4
						&& dimetior[1]->accipioCeleritas() > 128*4
				),
				( causarius[1].vicis
						&& dimetior[0]->accipioCeleritas() > 128*4
						&& dimetior[1]->accipioCeleritas() > 128*4
				)
			};
			mappa.validus0 = !( firmusCausarius[0]
					|| causarius[0].celeritas
					|| causarius[0].conjuctio
			);
			mappa.validus1 = !( firmusCausarius[1]
					|| causarius[1].celeritas
					|| causarius[1].conjuctio
			);

			// Сохранение неисправности в eeprom
			if (!mappa.validus0)
				eeprom.dps0Good = 0;
			if (!mappa.validus1)
				eeprom.dps1Good = 0;

			// Индикация неисправности на стоянке
			if ( dimetior[nCapio]->sicinCommoratio() )
			{
				if ( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 ) // полукомплект A
				{
					(reg.*lanternaPortus).pin<lanterna0>() = !eeprom.dps0Good;
					(reg.*lanternaPortus).pin<lanterna1>() = !eeprom.dps0Good;
				}
				else
				{
					(reg.*lanternaPortus).pin<lanterna0>() = !eeprom.dps1Good;
					(reg.*lanternaPortus).pin<lanterna1>() = !eeprom.dps1Good;
				}
			}

			// Вывод данных в линию связи
			acceleratioEtAffectus <<= (uint16_t(dimetior[nCapio]->accipioAcceleratio()) * 256) | mappa;

			uint16_t sigCel = signCeleritas( dimetior[nCapio]->accipioCeleritas() );
			celeritasProdo <<= sigCel;

			// Вывод данных в CAN
			if ( clock.getTime() > 1500 )// Запустит вывод сообщений через 1,5 секунды. За это время я подхвачу пройденный путь от ЭК.
			{
				// SAUT_INFO ---
				Complex<uint16_t> diam0 = dimetior[0]->accipioDiametros();
				Complex<uint16_t> diam1 = dimetior[1]->accipioDiametros();
				uint8_t sautInfo[8] =
				{
					uint8_t(sigCel >> 8),
					uint8_t(sigCel),
					dimetior[nCapio]->accipioAcceleratio(),
					diam0[1],
					diam0[0],
					diam1[1],
					diam1[0],
					mappa
				};

				// IPD_STATE ---

				// Округление скорости с гистерезисом
				Complex<uint16_t> rotCel;
				rotCel = rotundatioCeleritas( dimetior[nCapio]->accipioCeleritas() );

				// Подстройка под ЭК
				ecAdjust.adjust (spatiumMeters);

				uint8_t ipdState[8] =
				{
					(mappa.validus0 == false && mappa.validus1 == false) ? (uint8_t)2 : (uint8_t)0,
					uint8_t( (versus() * 128)
							| ((dimetior[nCapio]->accipioAcceleratio() & 0x80) >> 2) // знак ускорения
							| (!dimetior[nCapio]->sicinCommoratio() << 2)
							| uint8_t( rotCel[1] & 0x1) ),// направление + наличие импульсов ДПС + старший бит скорости в км/ч
					uint8_t( rotCel[0] ),// скорость в км/ч
					uint8_t( spatiumMeters[1] ),
					uint8_t( spatiumMeters[0] ),
					uint8_t( spatiumMeters[2] ),
					uint8_t( (ecAdjust.isMismatchCritical() << 5)
							| (causarius[!nCapio].celeritas << 4)
							| (nCapio << 3)
							| (firmusCausarius[!nCapio] << 2)
							| (causarius[nCapio].celeritas << 1)
							| (firmusCausarius[nCapio] << 0) ),
					uint8_t( dimetior[nCapio]->accipioAcceleratio()*2 )
				};

				if ( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 )
				{
					canDat.template send<CanTx::SAUT_INFO_A> (sautInfo);
					canDat.template send<CanTx::IPD_STATE_A> (ipdState);

					// IPD_DPS_FAULT ---
					enum class DpsFault : uint8_t
					{
						AllValidus,
						DuplarisCausarius,
						DuplarisConjuctio,
						Causarius0,
						Causarius1,
						Conjuctio0,
						Conjuctio1,
						Celeritas0,
						Celeritas1
					};
					DpsFault dpsFault = DpsFault::AllValidus;
					if ( causarius[0].vicis )
					dpsFault = DpsFault::Causarius0;
					if ( causarius[1].vicis )
					dpsFault = DpsFault::Causarius1;
					if ( causarius[0].vicis && causarius[1].vicis )
					dpsFault = DpsFault::DuplarisCausarius;
					if ( causarius[0].conjuctio && causarius[1].conjuctio )
					dpsFault = DpsFault::DuplarisConjuctio;

					scheduler.runIn(
							Command
							{	SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::dpsFaultProduco>(this), (uint16_t) dpsFault},
							100
					); // Вывести через 0,1 сек, чтобы успели освободиться страницы отправки CAN
				}
				else
				{
					canDat.template send<CanTx::SAUT_INFO_B> (sautInfo);
					canDat.template send<CanTx::IPD_STATE_B> (ipdState);
				}
			}
		}

		scheduler.runIn(
				Command {SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::produco>(this), 0},
				500); // Выводить сообщения раз в 0,5 сек.
	}

	void accipioConstans (uint16_t )
	{
		scheduler.runIn(
				Command{ SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::accipioConstans>(this), 0},
				500 );

			uint32_t tmp;
			if ( eeprom.club.property.configuration.read (tmp) )
			{
				Bitfield<EepromData::Club::Property::Configuration> conf (tmp);
				dimetior[0]->positio = conf.dps0Position;
				dimetior[1]->positio = conf.dps1Position;
			}

			if ( eeprom.club.property.diameter0.read (tmp) )
				dimetior[0]->constituoDiametros (tmp);

			if ( eeprom.club.property.diameter1.read (tmp) )
				dimetior[1]->constituoDiametros (tmp);

			if ( eeprom.club.property.dpsDentos.read (tmp) )
			{
				dimetior[0]->constituoCogs (tmp);
				dimetior[1]->constituoCogs (tmp);
			}
	}

	void dpsFaultProduco (uint16_t dpsFault)
	{
		uint8_t data[2] =
		{ (uint8_t) dpsFault, 0 };
		canDat.template send<CanTx::IPD_DPS_FAULT> (data);
	}

	// Выдаёт скорость в требуемом формате
	uint16_t signCeleritas (const uint16_t& cel) const
	{
		uint8_t superiorAliquam = cel >> 15;
		return (cel << 1) | superiorAliquam;
	}

	// Округление скорости до целых с гистерезисом
	const uint16_t& rotundatioCeleritas (const uint16_t& cel) const
	{
		if (cel / 128 < retroRotundatioCeleritas)
			((CeleritasSpatiumDimetior*) this)->retroRotundatioCeleritas = (cel + 96) / 128;
		else
			((CeleritasSpatiumDimetior*) this)->retroRotundatioCeleritas = (cel + 32) / 128;
		return retroRotundatioCeleritas;
	}

	// Выдаёт скрость выбранного датчика в км/ч/128
	uint16_t accipioCeleritas ()
	{
		return dimetior[nCapio]->accipioCeleritas ();
	}

	int32_t accipioSpatiumMeters ()
	{
		return _cast(int32_t, spatiumMeters);
	}
};

#endif /* DPS_H_ */
