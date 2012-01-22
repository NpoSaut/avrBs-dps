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

#include <cpp/timers.h>
#include <cpp/universal.h>
#include <cpp/io.h>
#include <cpp/dispatcher.h>

#include "hw_defines.h"

#include "SautDat.h"
using namespace Saut;
#include "CanDesriptors.h"
#include "eeprom.h"
#include <stdint.h>


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
template <	Port Register::*lanternaPortus, uint8_t lanterna0, uint8_t lanterna1,
			uint16_t minTempusPunctum, uint16_t maxTempusPunctum, uint16_t maxCeleritas,
			uint32_t period  >
class Dimetior
{
public:
	Dimetior( uint16_t diametros_, Eeprom::Saut::Configuration::DpsPosition	positio, bool lanternaOperor_ )
		: diametros (diametros_),
			// Расчитываем длину имульса (в единицах: дм/65536)
			// L = Pi * d(мм) /42 / 100 * 65536
			// d: 1600 - 800  => d*65536*Pi помещается в uint32_t
			// Pi * 65536 = 205887,416172544
		  longitudoImpulsio ( (uint32_t(diametros) * 205887) / 4200 ),
		  positio (positio), lanternaOperor (lanternaOperor_),
		  celeritas (0), acceleratio (0), acceleratioColum (0),
		  impulsio ({0,0}), impulsioLanterna ({0,0}), tempusPunctum ({0,0}),
		  affectus (0), versusRotatio ({!positio, !positio}), causarius (false), commoratio (true),
		  retroCan (0), vicisNum (0)
	{
//		if (lanternaOperor)
//		{
			(reg.*lanternaPortus).pin<lanterna0>().out ();
			(reg.*lanternaPortus).pin<lanterna1>().out ();
//		}
	}

	// Функция должна вызываться с периодом period (мкс)
	// принимает состояние каналов
	const uint32_t punctum (uint8_t affectusNovus) __attribute__ ((noinline))
	{
		tempusPunctum[0] ++;
		tempusPunctum[1] ++;

		// Получение состояния порта и нахождение фронта по каждому каналу
		affectusNovus &= 0b11; // Обрезать лишнее
		uint8_t affectusCommutatio = (affectus ^ affectusNovus) & affectusNovus; // 1 - если состояние изменилось с 0 на 1 т.е. фронт
		affectus = affectusNovus;

		if (affectusCommutatio) // Если случился фронт
		{
			// номер канала, по которому произошёл подъём
			uint8_t canalis = affectusCommutatio / 2;

			impulsio		 [canalis] ++;
			impulsioLanterna [canalis] ++;

			if ( impulsio[canalis] == 1 )
				tempusPunctum[canalis] = 0; // Начинаем считать время с 1-го импульса
											// После остановки (оба счётчика = 0) необходимо 2 импульса по одному и 1 по другому,
											// для того, чтобы вывести скорость

			uint32_t longitudo = 0;
			if ( impulsio[canalis] > impulsio[!canalis] )	// Метры идут только по одному каналу. По большему.
				longitudo = longitudoImpulsio;				// при переключении будет небольшая погрешность в большую сторону.


			if ( tempusPunctum[canalis] >= minTempusPunctum )  // Прошло достаточно времени для точного определения скорости
			{
				causarius = ( abs(impulsio[canalis] - impulsio[!canalis]) > 1 ); // Не было нормального чередования

				// Определение направления движения
				uint8_t vr = ((affectus + canalis) / 2) & 1;
				if ( vr == versusRotatio->retro )				 // Направление "применяется" только после подтверждения
					versusRotatio->modo = versusRotatio->retro;	//  чтобы исключить 1-импульсные дёрганья в момент трогания/остановки
				versusRotatio->retro = vr;


				debugImpulsio[0] = impulsio[0];
				debugImpulsio[1] = impulsio[1];

				computo(canalis);

				commoratio = false;

				// Для нового расчёта
				impulsio [canalis] = 1; // Сам начинаю считать время от текущего импульса
				tempusPunctum [canalis] = 0;
				impulsio [!canalis] = 0; // А сосед пусть сначала дождётся импульса и тогда начнёт считать время
				tempusPunctum [!canalis] = 0;
			}

			// Мигание светодиодами
			if (lanternaOperor)
				if ( impulsioLanterna[canalis] % 16 == 0 ) // мигать с периодом 1/16
				{
					if (canalis)
						(reg.*lanternaPortus).pin<lanterna1>().toggle ();
					else
						(reg.*lanternaPortus).pin<lanterna0>().toggle ();
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
		  a = ( (~a)|128 ) + 1;
		return a;
	}

	// Направление движения. 0 - вперёд
	bool accipioVersus () const
	{
		return !(versusRotatio->modo ^ positio);
	}
	// Остановка
	bool sicinCommoratio () const
	{
		return commoratio;
	}
	// Неисправность (недостоверность)
	bool sicinCausarius() const
	{
		return causarius;
	}

	const uint16_t diametros;	// Диаметр
	const uint32_t longitudoImpulsio; // Длина, которую колесо проходит за один импульс (в единицах: дм/65536)
	const Eeprom::Saut::Configuration::DpsPosition	positio;

	// --- ДЛЯ ОТЛАДКИ ---
	uint8_t retroCan; // последний канал, по которому производился расчёт
	uint8_t vicisNum; // кол-во переключений между каналами
	// --- КОНЕЦ ---

private:
	enum { maxCeleritasError = maxCeleritas / minTempusPunctum };
	const bool lanternaOperor;

	uint16_t celeritas; // Скорость по показаниям канала
	int16_t acceleratio; // Ускорение по показаниям канала
	int16_t acceleratioColum; // Промежуточные коэф-ты в фильтре ускорения
	uint16_t impulsio[2]; // Кол-во импульсов, пришедших по каналу
	uint8_t impulsioLanterna[2]; // Это кол-во не обнуляется, чтобы корректно моргать лампочками
	uint16_t tempusPunctum[2];
	uint8_t affectus; // состояние порта
	struct VersusRotatio // Напрвление вращения (true - туда, false - обратно)
	{
		uint8_t modo	:1;	// Сейчас
		uint8_t retro	:1; // В прошлый раз (для контроля)
	};
	Bitfield<VersusRotatio> versusRotatio;
	bool causarius; // Испорченность (недостоверность данных)
	bool commoratio;	// Остановка

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
		if (lanternaOperor)
		{
			(reg.*lanternaPortus).pin<lanterna0>() = sicinCausarius();
			(reg.*lanternaPortus).pin<lanterna1>() = sicinCausarius();
		}
	}

	void computo (const uint8_t& can) __attribute__ ((noinline))
	{
		// Для отладки --- УБРАТЬ
		if ( retroCan != can )
			vicisNum ++;
		retroCan = can;
		// конец --- для отладки -- УБРАТЬ

		// Считаем скорость
		uint16_t celeritasNovus =
					(((34468 * uint32_t (diametros)) / period) * (impulsio[can] - 1)) / tempusPunctum[can];

		// Считаем ускорение
		// Предотвращаем перегрузку (A не более 4м/c^2)
		int16_t celeritasCommutatio = int16_t(celeritasNovus) - int16_t(celeritas);
		if (celeritasCommutatio > 749)
			celeritasCommutatio = 749;
		else if (celeritasCommutatio < -749)
			celeritasCommutatio = -749;

		int16_t acceleratioNovusX8 =
				( int32_t( 8*277778 / int32_t (period) ) * celeritasCommutatio ) / tempusPunctum[can];

		// Для минимизации пульсаций ускорения применяется рекурсивный цифровой элептический НЧ фильтр 3-го порядка
		// С частойтой среза 0,3 Гц (при дискрете 4Гц) и подавлением 40 дб в полосе непропускания
		// Время релаксации порядка 4 сек.
		// На 512 км/ч при ускорении 0,01 м/c фильтр даёт пульсации +-0,01 м/c
		// Коэффициенты разностного уравнения: b0 = 1, b1 = 1, a0 = 1, a1 = -3/4, gain = 1/8
		int16_t colum = acceleratioNovusX8 + (acceleratioColum / 4)*3 ;
		acceleratio = (colum + acceleratioColum) / (8 * 8);
		acceleratioColum = colum;

		celeritas = celeritasNovus;

	}

};



// Cкоростемер
// -----------

template <	Port Register::*lanternaPortus, uint8_t lanterna0, uint8_t lanterna1, Port Register::*semiSynthesisPortus, uint8_t semiSynthesisPes,
			typename CanType, CanType& canDat  >
class CeleritasSpatiumDimetior
{
public:
	// диаметр бандажа
	const uint16_t& diametros (const uint8_t& n) const
	{
		return dimetior[n]->diametros;
	}

	CeleritasSpatiumDimetior (	Port Register::* accessusPortus,
								uint8_t& spatium, Safe<uint16_t>& celeritas, Safe<uint16_t>& acceleratioEtAffectus,
								uint8_t& odometer16dm0, uint8_t& odometer16dm1,
//								uint8_t (&spatiumClubPage)[3], uint8_t (&celeritasClubPage)[3],
								uint16_t diametros0, uint16_t diametros1  )
		: accessusPortus (accessusPortus),
		  spatiumMeters (0), spatiumMetersAdditioPermissus (true),
		  odometer16dm({ &odometer16dm0, &odometer16dm1 }),
		  ecDifferens (false), tractus (false), blockStatus (BlockStatus::InitState),
		  animadversor( InterruptHandler::from_method <myType, &myType::animadversio>(this) ),
		  productor( InterruptHandler::from_method <myType, &myType::produco>(this) ),
//		  animadversor( InterruptHandler (this, &myType::animadversio) ),
//		  productor( InterruptHandler (this, &myType::produco) ),
		  spatium (spatium), celeritasProdo (celeritas), acceleratioEtAffectus(acceleratioEtAffectus),
		  spatiumDecimeters65536 (0),
		  spatiumDecimetersMultiple10 (10),
		  spatiumDecimetersMulitple16 (0),
		  retroRotundatioCeleritas (0),
		  nCapio (0),
		  tempusRestitutioValidus (0),
		  tempusDifferens (0),
		  tempusTractusCommoratio (0),
		  tempusOpus (0),
		  activus (0)
	{
		causarius[0] = {0,0,0};
		causarius[1] = {0,0,0};
		Bitfield<Eeprom::Saut::Configuration> conf ( eeprom_read_byte( (uint8_t*) &eeprom.saut.configuration ) );
//		dimetior[0] = new DimetiorType( diametros0, conf->dps0Position, 0 );
//		dimetior[1] = new DimetiorType( diametros1, conf->dps1Position, 0 );
		dimetior[0] = new DimetiorType( diametros0, conf->dps0Position, (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 );
		dimetior[1] = new DimetiorType( diametros1, conf->dps1Position, (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 1 );

		(reg.*accessusPortus).in ();
		(reg.*lanternaPortus).pin<lanterna0>().out ();
		(reg.*lanternaPortus).pin<lanterna1>().out ();
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
			productor.start();
		}
	}
	void constituoPassivus ()
	{
		if (activus)
		{
			activus = false;
			animadversor.disable ();
			productor.disable ();
		}
	}
	bool sicinActivus () const { return activus; }

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

	Port Register::* accessusPortus; // Указатель на порт, на битах 0-3 отражается состояние каналов ДПС
	Complex<int32_t> spatiumMeters; // пройденный путь в метрах
	bool spatiumMetersAdditioPermissus;
	uint8_t* odometer16dm[2]; // cсылки на одометры

	bool ecDifferens; // Выставлять ли критическое расхождение с электронной картой
//	uint8_t parity; // чётность направления. Чётное направление (0) - увеличение ЖД-координаты. Нечётное (1) - уменьшение
	bool tractus; // 0 - торможение, 1...- тяга

	// О УЖАС!!!!!! -- !УБЕРИТЕ ЭТО ОТСЮДА!
	enum class BlockStatus: uint8_t
	{
		InitState			= 0,
		SystemOnLine		= 1,
		LoseSystemMessage	= 2
	};
	BlockStatus blockStatus;

private:
	typedef CeleritasSpatiumDimetior< lanternaPortus, lanterna0, lanterna1, semiSynthesisPortus, semiSynthesisPes, CanType, canDat > myType;
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
	// Время от старта програграммы в единицах productor.period, по истечению котого нужно снять флаг перезагрузки
	static constexpr uint8_t tempusAbInitioIndicium = 14;

	Alarm<Alarm0, 100> animadversor;
	Alarm<Alarm1A, 480000> productor;
	typedef Dimetior< lanternaPortus, lanterna0, lanterna1, minTempusPunctum, maxTempusPunctum, maxCeleritas, 100 > DimetiorType;
	DimetiorType* dimetior[2];

	uint8_t& spatium;
	Safe<uint16_t>& celeritasProdo;
	Safe<uint16_t>& acceleratioEtAffectus;

//	uint8_t (&spatiumClubPage) [3];
//	uint8_t (&celeritasClubPage) [3];

	uint32_t spatiumDecimeters65536; // пройденный путь в дм/65536
	uint8_t spatiumDecimetersMultiple10; // путь в дециметрах, кратный 10; для перевода в метры
	uint8_t spatiumDecimetersMulitple16; // путь в 1,6 м. Используется для ++ одометров

	uint16_t retroRotundatioCeleritas; // прошлое округлённое значение скорости. Для нужд округления с гистерезисом.

	bool nCapio;
	uint8_t tempusRestitutioValidus; // время после последнего сброса показаний исправности
	uint8_t tempusDifferens; // время, в течении которого сохраняется разность показаний ДПС более 25%
	uint8_t tempusTractusCommoratio; // время, в течении которого отсутсвует скорость по обоим ДПС в режиме Тяга
	uint8_t tempusOpus; // время работы в единицах productor.period (0,48 сек); для опред. перезагрузки
	uint8_t activus; // 0 - пассивен, 1 - активен

	struct Causarius
	{
		uint8_t vicis		:1;
		uint8_t conjuctio	:1;	// Эти критерии исправности могут быть выставлены только извне
		uint8_t celeritas	:1;	//
	};
	Bitfield<Causarius> causarius[2];

	// Вызывается c периодом animadversor.period (100 мкс)
	void animadversio ()
	{
		dispatcher.add (
				SoftIntHandler::from_method <myType, &myType::corpusVicissim> (this),
//				SoftIntHandler (this, &myType::corpusVicissim),
				(reg.*accessusPortus)
						);
		sei (); // Если прерывание ждёт, то пустить его
				// Это может быть принципиально.
				// Т.к. в коммуникаторе после завершения отправки нужно выставить флаг за 18 мкс,
				// это делается в прерывании
				// Вход и add здесь - 12,5 мкс, вход туда - 5,5 мкс. Выход отсюда - 4 мкс.
	}

	void corpusVicissim (uint16_t affectus)
	{
		uint32_t spatium0 = dimetior[0]->punctum (affectus & 0b11);
		uint32_t spatium1 = dimetior[1]->punctum (affectus / 4);
		// накапливать пройденный путь по выбранному датчику
		uint32_t appendicula = nCapio ? spatium1 : spatium0 ;

		if (appendicula != 0)
		{
			// Выводим в линию связи пройденный путь ...
			spatiumDecimeters65536 += appendicula;
			spatium = uint8_t(spatiumDecimeters65536 >> 16);
			// Обновляем показания одометров
			uint8_t spatiumDecimetersMulitple16New = uint8_t(spatiumDecimeters65536 >> 20);
			if ( spatiumDecimetersMulitple16 != spatiumDecimetersMulitple16New )
			{
				(*(odometer16dm[0])) ++;
				(*(odometer16dm[1])) ++;
			}
			spatiumDecimetersMulitple16 = spatiumDecimetersMulitple16New;

			if ( uint8_t(spatiumDecimeters65536 >> 16) == spatiumDecimetersMultiple10  ) // посчитать в метрах
			{
				spatiumDecimetersMultiple10 += 10;
//				if ( versus() ^ parity )
				if ( versus() )
					spatiumMeters --;
				else if ( spatiumMetersAdditioPermissus )
					spatiumMeters ++;
			}
		}
	}

	// Вызывается с периодом productor.period (0,48 сек)
	void produco ()
	{
		sei ();

		// Анализ показаний датчиков, выбор ДПС, установка неисправности
		uint8_t nMax = (dimetior[0]->accipioCeleritas() + 64) < dimetior[1]->accipioCeleritas(); // +64 чтобы предотвратить постоянное переключение

		causarius[0]->vicis = dimetior[0]->sicinCausarius();
		causarius[1]->vicis = dimetior[1]->sicinCausarius();

		if ( !dimetior[0]->sicinCausarius() && !dimetior[1]->sicinCausarius() )
		{
			if ( abs( dimetior[nMax]->accipioCeleritas() - dimetior[!nMax]->accipioCeleritas() )
					> dimetior[nMax]->accipioCeleritas()/4 + 1280  ) // Если разброс большой (и больше 10км/ч)
			{
				if (tempusDifferens == maxTempusDifferens) // довольно давно
					causarius[!nMax]->celeritas = true;
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
				nCapio = 1; // выбираем рабочий
			else if ( !causarius[0] && causarius[1] )
				nCapio = 0;
			else if ( causarius[0] && causarius[1] )
				nCapio = nMax;

			if ( tempusRestitutioValidus == maxTempusRestitutioValidus ) // раз в несколько секунд сбрасывать неисправность
			{
				causarius[0]->celeritas = false;
				causarius[1]->celeritas = false;
			}
		}

		// Контроль обрыва обоих ДПС
		bool duplarisTractus;
		if ( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 )
			duplarisTractus = ( canDat.template get<CanRx::MCO_LIMITS_A> ()[7] & 0b11 ); // признак двойной тяги
		else
			duplarisTractus = ( canDat.template get<CanRx::MCO_LIMITS_B> ()[7] & 0b11 ); // признак двойной тяги

		if ( tractus && dimetior[0]->sicinCommoratio() && dimetior[1]->sicinCommoratio() && !duplarisTractus ) // При тяге стоят оба ДПС
		{
			if (++tempusTractusCommoratio == 146)
			{
				causarius[0]->conjuctio = true;
				causarius[1]->conjuctio = true;
			}
		}
		else
			tempusTractusCommoratio = 0;


		// Выставление флагов
		struct Mappa
		{
			uint8_t versus0		:1;
			uint8_t versus1		:1;
			uint8_t commoratio 	:1;
			uint8_t dimetior	:1;
			uint8_t validus0	:1;
			uint8_t validus1	:1;
			uint8_t repeto		:1;
			uint8_t				:1;
		};
		Bitfield<Mappa> mappa;

		// Определение времени работы и выставление флага перезагрузки
		if (tempusOpus <= tempusAbInitioIndicium)
		{
			mappa->repeto = 1; // выставление флага перезагрузки
			tempusOpus ++;
		}
		else
			mappa->repeto = 0;

		// Перезагрузка, если выключили БЛОК
		if (blockStatus != BlockStatus::InitState)
			blockStatus = (BlockStatus) ( (uint8_t) blockStatus + 1 );

		if ( (uint8_t) blockStatus == 8 )
		{
    		cli ();
    		do															// Перезагружаем самих себя
    		{
    			wdt_enable(WDTO_15MS);
    			for (;;) { asm volatile ("nop"); }
    		}
    		while (0);
		}

		mappa->versus0 = dimetior[0]->accipioVersus();
		mappa->versus1 = dimetior[1]->accipioVersus();
		mappa->commoratio = dimetior[nCapio]->sicinCommoratio();
		mappa->dimetior = nCapio;
		// Неисправность != недостоверность
		// Неисправность - это недостверность при достаточно большой скорости
		// Потому что при смене направления и дребезге на стоянке возникает недостоверность
		mappa->validus0 = !(	( causarius[0]->vicis
									&& dimetior[0]->accipioCeleritas() > 128*4
									&& !dimetior[1]->sicinCommoratio()
								)
							|| causarius[0]->celeritas
							|| causarius[0]->conjuctio
							);
		mappa->validus1 = !(	( causarius[1]->vicis
									&& dimetior[1]->accipioCeleritas() > 128*4
									&& !dimetior[0]->sicinCommoratio()
								)
							|| causarius[1]->celeritas
							|| causarius[1]->conjuctio
							);

		// Вывод данных в линию связи
		acceleratioEtAffectus <<= (uint16_t(dimetior[nCapio]->accipioAcceleratio()) * 256) | mappa;

		uint16_t sigCel = signCeleritas( dimetior[nCapio]->accipioCeleritas() );
		celeritasProdo <<= sigCel;

		// ОТЛАДКА
		uint8_t myDebug[3] = {
					dimetior[0]->vicisNum,
					dimetior[1]->vicisNum,
					dispatcher.maxSize
							};

		// Вывод данных в CAN
		uint8_t sautInfo[8] = {
					uint8_t(sigCel >> 8),
					uint8_t(sigCel),
					dimetior[nCapio]->accipioAcceleratio(),
					uint8_t(dimetior[0]->diametros >> 8),
					uint8_t(dimetior[0]->diametros),
					uint8_t(dimetior[1]->diametros >> 8),
					uint8_t(dimetior[1]->diametros),
					mappa
							 };

		// Округление скорости с гистерезисом
		Complex<uint16_t> rotCel;
		rotCel = rotundatioCeleritas( dimetior[nCapio]->accipioCeleritas() );

		uint8_t ipdState[8] = {
					(mappa->validus0 == false && mappa->validus1 == false) ? (uint8_t)2 : (uint8_t)0,
					uint8_t(  (versus() * 128)
							| (!dimetior[nCapio]->sicinCommoratio() << 2)
							| uint8_t( rotCel[1] & 0x1) ), // направление + наличие импульсов ДПС + старший бит скорости в км/ч
					uint8_t( rotCel[0] ), // скорость в км/ч
					uint8_t( spatiumMeters[1] ),
					uint8_t( spatiumMeters[0] ),
					uint8_t( spatiumMeters[2] ),
					uint8_t( ecDifferens << 5 ),
					0
							 };
		ecDifferens = false;

		if ( (reg.*semiSynthesisPortus).pin<semiSynthesisPes>() == 0 )
		{
			canDat.template send<CanTx::SAUT_INFO_A> (sautInfo);
			canDat.template send<CanTx::IPD_STATE_A> (ipdState);
			canDat.template send<CanTx::MY_DEBUG_A> (myDebug);
		}
		else
		{
			canDat.template send<CanTx::SAUT_INFO_B> (sautInfo);
			canDat.template send<CanTx::IPD_STATE_B> (ipdState);
			canDat.template send<CanTx::MY_DEBUG_B> (myDebug);

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
					0,
					0,
					0
									};
			canDat.template send<CanTx::SYS_DATA_STATE2> (sysDataState2);
		}
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
		if ( cel/128 < retroRotundatioCeleritas )
			((CeleritasSpatiumDimetior*)this)->retroRotundatioCeleritas = (cel + 96) / 128;
		else
			((CeleritasSpatiumDimetior*)this)->retroRotundatioCeleritas = (cel + 32) / 128;
		return retroRotundatioCeleritas;
	}
};




#endif /* DPS_H_ */
