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

#include "cpp/can-dat.h"
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
class Dimetior {
public:
	Dimetior (Delegate<void (bool)> lanterna0Set, Delegate<void (bool)> lanterna1Set, bool lanternaOperor) :
			lanterna0Set (lanterna0Set), lanterna1Set (lanterna1Set), lanternaOperor (lanternaOperor), tractus (false), versusInversio (
					false), celeritas (0), acceleratio (0), acceleratioColum (0), impulsio (
			{ 0, 0 }), impulsioLanterna (
			{ 0, 0 }), lanternaState (
			{ true, true }), tempusPunctum (
			{ 0, 0 }), affectus (0), versusRotatio (
			{ !positio, !positio }), versusDynamic (0), causarius (false), versusCausarius (false), commoratio (true)
	{
		constituoDiametros (1250);
		constituoCogs (42);
		positio = EepromData::DpsPosition::Left;
	}

	// Функция должна вызываться с периодом period (мкс)
	// принимает состояние каналов
	// возвращает пройденный путь
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
			for (uint8_t canalis = 0; canalis < 2; canalis++)
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
					if (vr != versusDynamic)
						if (impulsio[canalis] != 1)
							versusCausarius = true;
					versusDynamic = vr;

					if (tempusPunctum[canalis] >= minTempusPunctum && // Прошло достаточно времени для точного определения скорости
							(tractus || impulsio[canalis] >= 4)) // В режиме выбега повышаем порог чувствительности
					{

						causarius = (abs(impulsio[canalis] - impulsio[!canalis]) > 1) // Не было нормального чередования
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
							lanternaState[canalis] = !lanternaState[canalis];
							if (canalis)
								lanterna1Set (lanternaState[canalis]);
							else
								lanterna0Set (lanternaState[canalis]);
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
			longitudoImpulsio = (uint32_t (diametros) * 205887) / (cogs * 100);
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
			longitudoImpulsio = (uint32_t (diametros) * 205887) / (cogs * 100);
		}
	}

	EepromData::DpsPosition positio;

	// --- ДЛЯ ОТЛАДКИ ---
//	uint8_t retroCan; // последний канал, по которому производился расчёт
//	uint8_t vicisNum; // кол-во переключений между каналами
	// --- КОНЕЦ ---

private:
	Delegate<void (bool)> lanterna0Set, lanterna1Set;
	const bool lanternaOperor;

	enum {
		// Максимальная скорость в единицах (км/ч / 128)
		maxCeleritas = 65535,
		// Период стробирования (опоса состояния датчика) в мкс
		period = 100,
		// После прошествия этого времени по первому спаду произойдёт подсчёт
		minTempusPunctum = 4096,
		// 1/minTempusPunctum - относительная погрешность определения скорости.
		// Время обновления показания на больших скоростях: minTempusPunctum * animadversor.period
		maxCeleritasError = maxCeleritas / minTempusPunctum,
		// Если в течении maxTempusPunctum отсутсвуют импульсы, то считаем, что скорость = 0.
		// В секундах: maxTempusPunctum * animadversor.period
		maxTempusPunctum = 16384,
	};

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
	bool lanternaState[2]; // Текущее состояние, для мигания
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

// Выбирает из двух датчиков один
class DimetiorChooser {
public:
	DimetiorChooser () : traction (false), previous (0), wasSwitch (false), speedDeviationCritical (false), _causariusInTraction({false, false})
	{}

	// Устанавливает признак наличия тяги (для алгоритма прошлифовка-блокировка)
	void setTraction (bool traction)
	{
		this->traction = traction;
	}

	// Обрабатывает новое состояние датчиков
	void processNewDimetiorsState (Dimetior * (&dimetiors) [2])
	{
		recordValidity (dimetiors);
		recordSpeedDeviation (dimetiors);

		// Выбирает датчик по набору критериев с приоритетом критериев.
		//  - Каждый критерий принимает номер выбранного до этого датчика и выполняет свои проверки,
		//    В результаты возвращает номер датчика, который выбрал сам.
		//  - Критерии проверяются по очереди. Каждый следующий критерий получается более приоритетным.

		// По-умолчанию отдаётся предпочтение предыдущему
		uint8_t preferred = previous;
		//preferred = checkSlipBlock (dimetiors, preferred);
		//preferred = checkProlongedDeviation (dimetiors, preferred);
		preferred = checkConfidentValidity (dimetiors, preferred);
		preferred = checkValidity (dimetiors, preferred);
		preferred = 0;
		
		wasSwitch = (previous != preferred);
		previous = preferred;
	}

	// Возвращает номер более подходящего датчика
	uint8_t getBestDimetiorNumber () const
	{
		return previous;
	}
	
	// Было ли переключене датчика при последнем processNewDimetiorsState ()
	bool wasSwitchUntilLastProcessState () const
	{
		return wasSwitch;
	}
	
	bool getCausariusInTraction (uint8_t i) const
	{
		return _causariusInTraction[i];
	}
	
	bool wasCausariusChange () const
	{
		return causariusChanged;
	}
	
	// Уверенная исправность
	class ConfidentValidity {
		public:
		ConfidentValidity (bool initValid = true) :
		validCycles (initValid ? validCyclesEnough : 0)
		{ }

		// Принимает новое состояние исправности.
		void recordNextState (bool valid)
		{
			if (valid)
			{
				if (validCycles < validCyclesEnough)
				validCycles++;
			}
			else
			{
				validCycles = 0;
			}
		}

		// Возвращает true в случае, когда исправность "установилась" (была достаточно давно)
		bool isConfidentValid () const
		{
			return validCycles == validCyclesEnough;
		}

		private:
		uint8_t validCycles; // Счётчик "машинных циклов", прошедших после появления исправности
		static constexpr uint8_t validCyclesEnough = 128; // Достаточное для "уверенной исправности" число циклов с исправностью
	};
	ConfidentValidity confidentValidity[2];

	// Интегральная характеристика, показвыающая разянтся ли показания скоростей
	class DeviationSupervisor
	{
		public:
		DeviationSupervisor ()
		: measure (0)
		{}

		// Записывает "отсчёт" с показаниями скоростей
		void recordNextSpeedSet (uint16_t (&speeds) [2])
		{
			if ( abs( int16_t(speeds[0]) - int16_t(speeds[1]) ) > speedLimit )
			{
				if (measure != 255)
				measure ++;
			}
			else
			{
				if (measure != 0)
				measure --;
			}

		}

		// Возвращает true в случае "длительного существенного расхождения"
		bool isDeviationCritical () const
		{
			return measure > boundary;
		}

		private:
		// Каждый "отсчёт" сравниваются скорости, если они расходятся больше, чем на speedLimit, то к measure +1, в противном случае -1
		uint8_t measure; // Мера расхождения
		static constexpr uint8_t boundary = 128; // Граница, выше которой расхождение считается критическим
		static constexpr uint16_t speedLimit = 5*128; // Предел в км/ч * 128. Выше - считается как расхождения, ниже - считается как совпадение
	};
	DeviationSupervisor deviationSupervisor;

private:
	bool traction;
	uint8_t previous;
	bool wasSwitch;
	bool speedDeviationCritical;
	bool causariusChanged;
	bool _causariusInTraction[2];
	
	void setCausariusInTraction (uint8_t dimetior, bool causarius)
	{
		if ( _causariusInTraction[dimetior] != causarius )
		{
			_causariusInTraction[dimetior] = causarius;
			causariusChanged = true;
		}
	}

	// Ведёт статистику недостоверности для получения "уверенной достоверности"
	void recordValidity (Dimetior * (&dimetiors) [2])
	{
		causariusChanged = false;
		
		for (uint8_t i = 0; i < 2; i ++)
			if ( dimetiors[previous]->accipioCeleritas() > 128 ) // Достоверность учитывается только в движении
			{
				setCausariusInTraction(i, dimetiors[i]->sicinCausarius());
				confidentValidity[i].recordNextState( !getCausariusInTraction(i) );
			}				
	}

	// Ведёт статистику по разности скоростей датчиков
	void recordSpeedDeviation (Dimetior * (&dimetiors) [2])
	{
		if ( confidentValidity[0].isConfidentValid() && confidentValidity[1].isConfidentValid() )
		{
			uint16_t speeds [2] = {dimetiors[0]->accipioCeleritas(), dimetiors[1]->accipioCeleritas()};
			deviationSupervisor.recordNextSpeedSet(speeds);
		}
	}

	// При тяге выбираем более медленный (если это существенно - больлше чем на 1 км/ч), при торможении - более быстрый
	uint8_t checkSlipBlock (Dimetior * (&dimetiors) [2], uint8_t preferred)
	{
		if (traction)
		{
			if ( dimetiors[preferred]->accipioCeleritas () > dimetiors[!preferred]->accipioCeleritas () + 128 )
				return !preferred;
			else
				return preferred;
		}
		else // торможение
		{
			if ( dimetiors[preferred]->accipioCeleritas () + 128 < dimetiors[!preferred]->accipioCeleritas () )
				return !preferred;
			else
				return preferred;
		}
	}

	// В состоянии "затянувшегося отклонения" выбирает датчик с максимальной скоростью
	uint8_t checkProlongedDeviation (Dimetior * (&dimetiors) [2], uint8_t preferred)
	{
		if ( deviationSupervisor.isDeviationCritical() )
			return dimetiors[0]->accipioCeleritas() > dimetiors[1]->accipioCeleritas() ? 0 : 1; // max
		else
			return preferred;
	}

	// Датчик должен быть уверенно достоверным в течении последнего времени.
	bool checkConfidentValidity (Dimetior * (&dimetiors) [2], uint8_t preferred)
	{
		if ( !confidentValidity[preferred].isConfidentValid() && confidentValidity[!preferred].isConfidentValid() )
			return !preferred;
		else
			return preferred;
	}

	// Датчик должен быть исправным (на случай если оба не являются "достоверно исправными")
	bool checkValidity (Dimetior * (&dimetiors) [2], uint8_t preferred)
	{
		if ( getCausariusInTraction(preferred) && !getCausariusInTraction(!preferred) )
			return !preferred;
		else
			return preferred;
	}

};

// Контролирует "обрыв двух датчиков"
template <typename CanType, CanType& canDat>
class BothBreak
{
public:
	BothBreak (bool isSelfComplectA)
		: isSelfComplectA (isSelfComplectA), tractionWithoutSpeedCycles (0), bothBreak (false)
	{}

	void processStopAndTractionState (bool stop, bool traction)
	{
		if ( traction && !isDoubleTraction() && stop )// Стоим при тяге
		{
			if ( tractionWithoutSpeedCycles >= 76*2 ) // В течении времени 76 сек.
				bothBreak = true;
			else
				tractionWithoutSpeedCycles ++;
		}
		else
			tractionWithoutSpeedCycles = 0;
	}

	bool isBreak () const { return bothBreak; }

private:
	const bool isSelfComplectA;
	uint8_t tractionWithoutSpeedCycles;
	bool bothBreak;

	// признак двойной тяги
	bool isDoubleTraction () const
	{
		if ( isSelfComplectA )
			return ( (canDat.template get<CanRx::MCO_LIMITS_A> ()[7] & 0b11) == 0b11 );
		else
			return ( (canDat.template get<CanRx::MCO_LIMITS_B> ()[7] & 0b11) == 0b11 );
	}
};

// Cкоростемер
// -----------

template<typename CanType, CanType& canDat, typename ClockType, ClockType& clock, typename Scheduler,
		Scheduler& scheduler>
class CeleritasSpatiumDimetior {
public:
	CeleritasSpatiumDimetior (Delegate<void (bool)> lanterna0Set, Delegate<void (bool)> lanterna1Set,
							bool isSelfComplectA, Port Register::* accessusPortus, uint8_t& spatium, Safe<uint16_t>& celeritas,
							Safe<uint16_t>& acceleratioEtAffectus, InterruptHandler odometer16dm0PlusPlus,
							InterruptHandler odometer16dm1PlusPlus)
			: lanterna0Set (lanterna0Set), lanterna1Set (lanterna1Set), isSelfComplectA (isSelfComplectA),
			  accessusPortus (accessusPortus), spatiumMeters (0), spatiumAdjustedMeters (0),
			  odometer16dmPlusPlus ({ odometer16dm0PlusPlus, odometer16dm1PlusPlus }),
			  tractus (false), manualVersus (manualVersus), repeto (true), // после перезагрузки -- флаг перезагрузки
			  ecAdjust ( Delegate<uint16_t ()>::from_method<CeleritasSpatiumDimetior,
					  	&CeleritasSpatiumDimetior::accipioCeleritas> (this),
					  	 Delegate<int32_t ()>::from_method<CeleritasSpatiumDimetior,
						&CeleritasSpatiumDimetior::accipioSpatiumAdjustedMeters> (this)),
			  animadversor ( InterruptHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::animadversio> (this) ),
			  mittoSaut (false), spatium (spatium), celeritasProdo (celeritas), acceleratioEtAffectus (acceleratioEtAffectus),
			  spatiumDecimeters65536 (0), spatiumDecimetersMultiple10 (10), spatiumDecimetersMulitple16 (0),
			  rotundCeleritas (0), dimetiorChooser(), bothBreak(isSelfComplectA), activus (0),
			  dimetiorObj ({Dimetior(lanterna0Set, lanterna1Set, isSelfComplectA), Dimetior(lanterna0Set, lanterna1Set, !isSelfComplectA)})

	{
		dimetiors[0] = &(dimetiorObj[0]);
		dimetiors[1] = &(dimetiorObj[1]);
		accipioConstans (0);

		(reg.*accessusPortus).pin<0>().in ();
		(reg.*accessusPortus).pin<1>().in ();
		(reg.*accessusPortus).pin<2>().in ();
		(reg.*accessusPortus).pin<3>().in ();

		// Инициализация линии связи
		acceleratioEtAffectus <<= 0x74;// оба вперёд и исправны. Флаг перезагрузки
		celeritasProdo <<= 0;

		scheduler.runIn( Command{SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::produco>(this), 0}, 500 );
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
	{	
		return activus;
	}
	
	void constituoMittoSaut (bool mitto)
	{
		mittoSaut = mitto;
	}

	void constituoTractus ()
	{
		tractus = true;
		dimetiors[0]->constituoTractus();
		dimetiors[1]->constituoTractus();
	}
	void constituoNonTractus ()
	{
		tractus = false;
		dimetiors[0]->constituoNonTractus();
		dimetiors[1]->constituoNonTractus();
	}

	void constituoVersusInversio ( bool inversio )
	{	dimetiors[0]->constituoVersusInversio (inversio); dimetiors[1]->constituoVersusInversio (inversio);}
	
	// Напрвление движения. 0 - вперёд
	const uint8_t versus () const
	{
		return manualVersus;
	}
	void constituoVersus (uint8_t versus)
	{
		manualVersus = versus;
	}

	bool sicinBothBrake () const
	{	return bothBreak.isBreak();}

	// Скрость в км/ч/256 + старший бит в младшем бите
	const uint16_t celeritas () const
	{
		return signCeleritas( dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas() );
	}

	// диаметр бандажа
	const uint16_t diametros (const uint8_t& n) const
	{
		return dimetiors[n]->accipioDiametros();
	}

	void takeEcDataForAdjust(uint16_t pointerToData)
	{
		ecAdjust.takeEcData(pointerToData);
	}

	Port Register::* accessusPortus; // Указатель на порт, на битах 0-3 отражается состояние каналов ДПС
	Complex<int32_t> spatiumMeters;// пройденный путь в метрах
	Complex<int32_t> spatiumAdjustedMeters;// пройденный путь с подстройкой под ЭК
	InterruptHandler odometer16dmPlusPlus[2];// Делагаты функций, делающийх ++ к одометрам

	bool repeto;// флаг перезагрузки в линию связи

private:
	const Delegate<void (bool)> lanterna0Set, lanterna1Set;
	const bool isSelfComplectA;

	Alarm<Alarm0, 100> animadversor;
	Dimetior dimetiorObj[2];
	Dimetior* dimetiors[2];

	typedef EcAdjust < CanType, canDat > EcAdjustType;
	EcAdjustType ecAdjust;

	bool tractus;// 0 - выбег или торможение, 1 - тяга
	uint8_t manualVersus;

	bool mittoSaut; // Отправлять ли информацию в САУТ
	uint8_t& spatium;
	Safe<uint16_t>& celeritasProdo;
	Safe<uint16_t>& acceleratioEtAffectus;

	uint32_t spatiumDecimeters65536;// пройденный путь в дм/65536
	uint8_t spatiumDecimetersMultiple10;// путь в дециметрах, кратный 10; для перевода в метры
	uint8_t spatiumDecimetersMulitple16;// путь в 1,6 м. Используется для ++ одометров

	Complex<uint16_t> rotundCeleritas;// прошлое округлённое значение скорости. Для нужд округления с гистерезисом.

	DimetiorChooser dimetiorChooser;
	BothBreak<CanType, canDat> bothBreak;
	uint8_t activus;// 0 - пассивен, 1 - активен
	
	uint8_t auxResourceTimer;

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
		uint32_t spatium0 = dimetiors[0]->punctum ((affectus / 2) & 0b11);
		uint32_t spatium1 = 0;
		// накапливать пройденный путь по выбранному датчику
		uint32_t appendicula = dimetiorChooser.getBestDimetiorNumber() == 1 ? spatium1 : spatium0;

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
				{
					spatiumMeters ++;
					spatiumAdjustedMeters ++;
				}
				else
				{
					spatiumMeters --;
					spatiumAdjustedMeters --;
				}
			}
		}
	}

	// Выводит результат работы датчиков в линию связи и CAN
	void produco (uint16_t step)
	{
		if (activus)
		{
			switch (step)
			{
				case 0:
				dimetiorChooser.setTraction (tractus);
				dimetiorChooser.processNewDimetiorsState (dimetiors);

				bothBreak.processStopAndTractionState (dimetiors[0]->sicinCommoratio() && dimetiors[1]->sicinCommoratio(), tractus);
				break;
				
				case 1:

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
				mappa.versus0 = dimetiors[0]->accipioVersus();
				mappa.versus1 = dimetiors[1]->accipioVersus();
				mappa.commoratio = dimetiors[dimetiorChooser.getBestDimetiorNumber()]->sicinCommoratio();
				mappa.dimetior = dimetiorChooser.getBestDimetiorNumber();
				mappa.validus0 = !dimetiorChooser.getCausariusInTraction(0);
				mappa.validus1 = !dimetiorChooser.getCausariusInTraction(1);

				// Сохранение неисправности в eeprom
				if (!mappa.validus0)
					eeprom.dps0Good = 0;
				if (!mappa.validus1)
					eeprom.dps1Good = 0;

				// Индикация неисправности на стоянке
				if ( dimetiors[dimetiorChooser.getBestDimetiorNumber()]->sicinCommoratio() )
				{
					if ( isSelfComplectA ) // полукомплект A
					{
						lanterna0Set (eeprom.dps0Good);
						lanterna1Set (eeprom.dps0Good);
					}
					else
					{
						lanterna0Set (eeprom.dps1Good);
						lanterna1Set (eeprom.dps1Good);
					}
				}

				// Вывод данных в линию связи
				acceleratioEtAffectus <<= (uint16_t(dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioAcceleratio()) * 256) | mappa;

				uint16_t sigCel = signCeleritas( dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas() );
				celeritasProdo <<= sigCel;

				// Вывод данных в CAN
				if ( clock.getTime() > 1500 )// Запустит вывод сообщений через 1,5 секунды. За это время я подхвачу пройденный путь от ЭК.
				{
					// SAUT_INFO ---
					Complex<uint16_t> diam0 = dimetiors[0]->accipioDiametros();
					Complex<uint16_t> diam1 = dimetiors[1]->accipioDiametros();
					uint8_t sautInfo[8] =
					{
						uint8_t(sigCel >> 8),
						uint8_t(sigCel),
						dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioAcceleratio(),
						diam0[1],
						diam0[0],
						diam1[1],
						diam1[0],
						mappa
					};

					// IPD_STATE ---

					// Округление скорости с гистерезисом
					rotundCeleritas = rotundatioCeleritas( dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas(), rotundCeleritas );

					// Подстройка под ЭК
					ecAdjust.adjust (spatiumAdjustedMeters);

					uint8_t ipdState[8] =
					{
						(mappa.validus0 == false && mappa.validus1 == false) || bothBreak.isBreak() ? (uint8_t)2 : (uint8_t)0,
						uint8_t( (versus() * 128)
								| ((dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioAcceleratio() & 0x80) >> 2) // знак ускорения
								| (!dimetiors[dimetiorChooser.getBestDimetiorNumber()]->sicinCommoratio() << 2)
								| uint8_t( (rotundCeleritas >> 15) & 1 ) ),// направление + наличие импульсов ДПС + старший бит скорости в км/ч
						uint8_t( rotundCeleritas/128 ),// скорость в км/ч
						uint8_t( spatiumAdjustedMeters[1] ),
						uint8_t( spatiumAdjustedMeters[0] ),
						uint8_t( spatiumAdjustedMeters[2] ),
						uint8_t( (ecAdjust.isMismatchCritical() << 5)
								| (dimetiorChooser.deviationSupervisor.isDeviationCritical() << 4)
								| (dimetiorChooser.getBestDimetiorNumber() << 3)
								| (dimetiorChooser.getCausariusInTraction(!dimetiorChooser.getBestDimetiorNumber()) << 2)
								| (dimetiorChooser.deviationSupervisor.isDeviationCritical() << 1)
								| (dimetiorChooser.getCausariusInTraction(dimetiorChooser.getBestDimetiorNumber()) << 0) ),
						uint8_t( dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioAcceleratio()*2 )
					};

					uint8_t origDist[4] =
					{	spatiumMeters[0], spatiumMeters[1], spatiumMeters[2], spatiumMeters[3]};
					
					// AUX_RESORCE: RES_INTERNAL_WARNING ---
					uint8_t auxResource[5] = {2, 0, 0, 0, 0};
					if ( dimetiorChooser.wasSwitchUntilLastProcessState() )
					{
						auxResource[1] = 1;
					}
					else if ( auxResourceTimer++ >= 240 ) // Раз в 2 минуты
					{
						auxResourceTimer = 0;
						auxResource[1] = 2;
					}
					else if ( dimetiorChooser.wasCausariusChange() )
					{
						auxResource[1] = 3;
					}
					
					if ( auxResource[1] != 0 )
					{
						auxResource[2] = (dimetiorChooser.getCausariusInTraction(dimetiorChooser.getBestDimetiorNumber()) << 0)
								| (dimetiorChooser.deviationSupervisor.isDeviationCritical() << 1)
								| (dimetiorChooser.getCausariusInTraction(!dimetiorChooser.getBestDimetiorNumber()) << 2)
								| (dimetiorChooser.deviationSupervisor.isDeviationCritical() << 3)
								| (dimetiorChooser.getBestDimetiorNumber() << 4);
						auxResource[3] = rotundatioCeleritas (dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas(), rotundCeleritas) / 128;
						auxResource[4] = rotundatioCeleritas (dimetiors[!dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas(), rotundCeleritas) / 128;
					}

					if ( isSelfComplectA )
					{
						canDat.template send<CanTx::SAUT_INFO_A> (sautInfo);
						canDat.template send<CanTx::IPD_STATE_A> (ipdState);
						if ( auxResource[1] != 0 )
							canDat.template send<CanTx::AUX_RESOURCE_IPD_A> (auxResource);
						else
							canDat.template send<CanTx::MY_DEBUG_A> (origDist);

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
						if ( dimetiorChooser.getCausariusInTraction(0) )
							dpsFault = DpsFault::Causarius0;
						if ( dimetiorChooser.getCausariusInTraction(1) )
							dpsFault = DpsFault::Causarius1;
						if ( dimetiorChooser.getCausariusInTraction(0) && dimetiorChooser.getCausariusInTraction(1) )
							dpsFault = DpsFault::DuplarisCausarius;
						if ( bothBreak.isBreak() )
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
						if ( auxResource[1] != 0 )
							canDat.template send<CanTx::AUX_RESOURCE_IPD_B> (auxResource);
						else
							canDat.template send<CanTx::MY_DEBUG_B> (origDist);
					}
				}
				break;
			}
		}

		if (step == 0)
		{
			dispatcher.add(
				Command{SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::produco>(this), step+1});
		}
		else
		{
			scheduler.runIn(
				Command{SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::produco>(this), 0},
				500); // Выводить сообщения раз в 0,5 сек.
		}
	}

	void accipioConstans (uint16_t step)
	{
		uint32_t tmp;
		
		switch (step)
		{
			case 0:
			if ( eeprom.club.property.configuration.read (tmp) )
			{
				Bitfield<EepromData::Club::Property::Configuration> conf (tmp);
				dimetiors[0]->positio = conf.dps0Position;
				dimetiors[1]->positio = conf.dps1Position;
			}
			break;
			
			case 1:
			if ( eeprom.club.property.diameter0.read (tmp) )
				dimetiors[0]->constituoDiametros ( tmp );
			break;
			
			case 2:
			if ( eeprom.club.property.diameter1.read (tmp) )
				dimetiors[1]->constituoDiametros ( tmp );
			break;
			
			case 3:
			if ( eeprom.club.property.dpsDentos.read (tmp) )
			{
				dimetiors[0]->constituoCogs (tmp);
				dimetiors[1]->constituoCogs (tmp);
			}
			break;
		}
		if (step < 3)
		{
			dispatcher.add(
				Command{SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::accipioConstans>(this), step+1}
			);
		}
		else
		{
			scheduler.runIn(
				Command{SoftIntHandler::from_method<CeleritasSpatiumDimetior, &CeleritasSpatiumDimetior::accipioConstans>(this), 0},
				500 );
		}
	}

	void dpsFaultProduco (uint16_t dpsFault)
	{
		uint8_t data[2] =
		{	(uint8_t) dpsFault, 0};
		canDat.template send<CanTx::IPD_DPS_FAULT> (data);
	}

	// Выдаёт скорость в требуемом формате
	uint16_t signCeleritas (const uint16_t& cel) const
	{
		uint8_t superiorAliquam = cel >> 15;
		return (cel << 1) | superiorAliquam;
	}

	// Округление скорости до целых с гистерезисом
	uint16_t rotundatioCeleritas (const uint16_t& cel, const uint16_t& retroCel) const
	{
		uint16_t rotund;
		
		if (cel < retroCel)
			rotund = cel + 96;
		else
			rotund = cel + 32;

		return rotund;
	}

	// Выдаёт скрость выбранного датчика в км/ч/128
	uint16_t accipioCeleritas ()
	{
		return dimetiors[dimetiorChooser.getBestDimetiorNumber()]->accipioCeleritas ();
	}

	int32_t accipioSpatiumMeters ()
	{
		return _cast(int32_t, spatiumMeters);
	}

	int32_t accipioSpatiumAdjustedMeters ()
	{
		return _cast(int32_t, spatiumAdjustedMeters);
	}
};

#endif /* DPS_H_ */
