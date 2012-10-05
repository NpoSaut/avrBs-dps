/*
 * kpt.h
 *
 *  Created on: 13.10.2011
 *      Author: nazemnyh
 */

#ifndef KPT_H_
#define KPT_H_

#include <cpp/universal.h>
#include "hw_defines.h"
#include "CanDesriptors.h"

template < typename Scheduler, Scheduler& scheduler >
class KptCorrectnessSmooth
{
public:
	bool isCorrect ();

	void catchCorrect ();
	void catchUnCorrect ();

private:
	static constexpr uint16_t switchingTimeToCorrectMs = 4000;
	static constexpr uint16_t switchingTimeToUnCorrectMs = 2000;

	enum class State : uint8_t
	{
		Correct = 0,
		SwitchingToUnCorrect = 1,
		UnCorrect = 2,
		SwitchingToCorrect = 3
	};
	State state;

	uint8_t activeTimerId;
	void endSwitch (uint16_t);
};

template < typename Scheduler, Scheduler& scheduler >
bool KptCorrectnessSmooth<Scheduler, scheduler>::isCorrect()
{
	switch (state)
	{
		case State::Correct:
		case State::SwitchingToUnCorrect:
			return true;
			break;

		case State::UnCorrect:
		case State::SwitchingToCorrect:
		default:
			return false;
			break;
	}
}

template < typename Scheduler, Scheduler& scheduler >
void KptCorrectnessSmooth<Scheduler, scheduler>::catchCorrect()
{
	switch (state)
	{
		case State::SwitchingToUnCorrect:
			state = State::Correct;
			scheduler.deletId (activeTimerId);
			break;

		case State::UnCorrect:
			state = State::SwitchingToCorrect;
			scheduler.runIn (
					Command{SoftIntHandler::from_method<KptCorrectnessSmooth, &KptCorrectnessSmooth::endSwitch>(this), 0},
					switchingTimeToCorrectMs );
			break;

		default:
			break;
	}
}

template < typename Scheduler, Scheduler& scheduler >
void KptCorrectnessSmooth<Scheduler, scheduler>::catchUnCorrect()
{
	switch (state)
	{
		case State::SwitchingToCorrect:
			state = State::UnCorrect;
			scheduler.deletId (activeTimerId);
			break;

		case State::Correct:
			state = State::SwitchingToUnCorrect;
			scheduler.runIn (
					Command{SoftIntHandler::from_method<KptCorrectnessSmooth, &KptCorrectnessSmooth::endSwitch>(this), 0},
					switchingTimeToUnCorrectMs );
			break;

		default:
			break;
	}
}

template < typename Scheduler, Scheduler& scheduler >
void KptCorrectnessSmooth<Scheduler, scheduler>::endSwitch (uint16_t)
{
	switch (state)
	{
		case State::SwitchingToCorrect:
			state = State::Correct;
			break;

		case State::SwitchingToUnCorrect:
			state = State::UnCorrect;
			break;

		default:
			break;
	}
}

namespace KptInternal
{
	struct State
	{
		enum Color
		{
			Unknown = 0,
			Green = 1,
			Yellow = 2,
			RedYellow = 4,
			Red	= 8,
			White = 16
		};
		enum Type
		{
			Kpt5 = 0,
			Kpt7 = 1,
		};
		Color	color	:5;
		uint8_t kptImp	:1;
		uint8_t correct	:1;
		Type	type	:1;
	};
}

int8_t difference (const int8_t (&a) [3][2], const int8_t (&b) [3][2]) [3][2]
{
	uint8_t diff [3][2];
	for (uint8_t i = 0; i < 3; ++i)
		for (uint8_t j = 0; j < 2; ++j)
			if ( b[i][j] == 0xFF )
				diff[i][j] = 0;
			else
				diff[i][j] = a[i][j] - b[i][j];
	return diff;
}

// Сравнивает массивы data[3][2] и pattern[3][2], циклически переставляя data
//  - при нахождения такого циклического положение,
//    в ктором для любых i,j выполняет |data[i][j] - pattern[i][j]| <= tolerance,
//    возвращает true и дополнительно разность data - pattern по ссылке difference
//  - в противном случае возвращает false и неопределённые значения в difference
bool isApproximatelyCyclicEqual (const int8_t (&data) [3][2], const int8_t (&pattern) [3][2], const uint8_t tolerance, int8_t (&difference) [3][2])
{
	for (uint8_t offset = 0; offset < 3; ++offset) // цикличный поворот
	{
		uint8_t i, j;
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 2; ++j)
			{
				if ( pattern[i][j] == 0xFF ) // флаг, означаеющий "любой"
					difference[i][j] = 0;
				else
					difference[i][j] = data[(i+offset)%3][j] - pattern[i][j];

				if ( abs (difference[i][j]) > tolerance )
				{
					i = 0xFF; // Выходи из цикла сравнения и флаг о неудаче
					j = 0xFF;
				}
			}

		if ( i!= 0xFF && j != 0xFF)
			return true;
	}
	return false;
}

class TypeKeeper
{
public:
	void newType (const uint8_t (&pattern) [3][2]);
	bool isCorrespond (const uint8_t (&data) [3][2]);

private:
	static constexpr uint8_t toleranceInit = 5;
	static constexpr uint8_t taleranceMin  = 5;

	int8_t pattern [3][2];
	int8_t patternX10 [3][2];
	uint8_t tolerance;
};

void TypeKeeper::newType (const int8_t (&pattern) [3][2])
{
	for (uint8_t i = 0; i < 3; ++i)
		for (uint8_t j = 0; j < 2; ++j)
			TypeKeeper::pattern[i][j] = pattern[i][j];

	tolerance = toleranceMin;
}

bool TypeKeeper::isCorrespond (const int8_t (&data) [3][2])
{
	uint8_t difference [3][2];
	if ( isApproximatelyCyclicEqual (data, pattern, tolerance, difference) )
	{
		for (uint8_t i = 0; i < 3; ++i)
			for (uint8_t j = 0; j < 2; ++j)
				if ( difference[i][j] < 1 )
					patternX10[i][j] -= 1;
				else if ( difference[i][j] > 1 )
					patternX10[i][j] += 1;

		return true;
	}
	else
		return false;
}

class KptNew
{
public:
	void fall (uint8_t upTime);
	void rise (uint8_t downTime);

private:
	KptInternal::State getState ();
	bool isApproximatelyCyclicEqual (const uint8_t (&data) [3][2], const uint8_t (&pattern) [3][2], const uint8_t error);

	uint8_t time[3][2];
	uint8_t timePosition;


	uint8_t timePattern5Green[3][2];
	uint8_t toleranceError;
};

void KptNew::fall (uint8_t upTime)
{
	if ( timePosition % 2 != 0 ) // Пропустили rise
		rise (0);

	time[timePosition/2][0] = upTime;
	timePosition ++;
}

void KptNew::rise (uint8_t downTime)
{
	if ( timePosition % 2 == 0 ) // Пропустили fall
		fall (0);

	time[timePosition/2][1] = downTime;
	timePosition ++;
}

KptInternal::State KptNew::getState ()
{
	using namespace KptInternal;
	if ( isApproximatelyCyclicEqual (time, timePattern5Green, toleranceError) )
		return State{ State::Color::Green, 0, 1, State::Type::Kpt5 };
	else
		return State{ State::Color::Unknown, 0, 0, State::Type::Kpt5 };
}

bool KptNew::isApproximatelyCyclicEqual (const uint8_t (&data) [3][2], const uint8_t (&pattern) [3][2], const uint8_t tolerance)
{
	for (uint8_t offset = 0; offset < 3; ++offset) // цикличный поворот
	{
		bool equal = true;
		for (uint8_t i = 0; i < 3; ++i)
			for (uint8_t j = 0; j < 2; ++j)
				if ( pattern[i][j] != 0xFF ) // флаг, означаеющий "любой"
					if ( data[(i+offset)%3][j] < pattern[i][j] - tolerance ||
						 data[(i+offset)%3][j] > pattern[i][j] + tolerance )
						equal = false;
		if (equal)
			return true;
	}
	return false;
}



// Расшифровка КПТ
template <  typename Clock, Clock& clock,
			typename Scheduler, Scheduler& scheduler,
			typename CanDat, CanDat& canDat  >
class Kpt
{
private:
	static constexpr uint16_t timePrescale = 10000 / Clock::discreetMks;
	static constexpr uint16_t timeMod = 10000 % Clock::discreetMks;

	typedef KptInternal::State State;

	State& state; // Ссылка на внешнюю переменную, в которую выводится состояние

	bool active; // Разрешение выводить в линию связи

	bool impulseWatchDog;
	uint8_t periodTime;
	uint8_t impulseStartTime;
	uint8_t shortImpNumber;
	uint8_t lisZeroingPermission;

	State statePrevious;
	uint8_t repeateCounter;

	uint8_t getImpulseTime ()
	{
		uint8_t time;
		uint8_t now = clock.getTime() / timePrescale;
		time = now - impulseStartTime;
		impulseStartTime = now;
		return time;
	}

	void canSend (uint8_t n)
	{
		uint8_t packet[5] = {
				2, // WARNING
				1, // KPT
				n,
				0,
				0
							};
		if (reg.portB.pin7 == 0) // первый полукомплект
			canDat.template send<AUX_RESOURCE_IPD_A> (packet);
		else
			canDat.template send<AUX_RESOURCE_IPD_B> (packet);
	}

	void watchDog (uint16_t)
	{
		if ( impulseWatchDog == false ) // не было изменений
		{
			if (state.color == State::Color::RedYellow)
				state = { 	State::Color::Red,
							state.kptImp,
							false,
							(State::Type) ~((uint8_t) state.type)
						};
			else if (state.color != State::Color::Red)
				state = { 	State::Color::White,
							state.kptImp,
							false,
							state.type
						};
//			canSend ();
		}
		else
			impulseWatchDog = false;

		scheduler.runIn( Command{ SoftIntHandler::from_method <Kpt, &Kpt::watchDog>(this), 0 }, 2000000/Scheduler::discreetMks );
//		scheduler.runIn( Command{SoftIntHandler (this, &Kpt::watchDog), 0}, 2000000/Scheduler::discreetMks );
	}


public:
	Kpt (uint8_t& state, uint8_t& lis)
		: state ( _cast(State, state) ),
		  impulseWatchDog (false), periodTime (0), impulseStartTime (0),
		  shortImpNumber (0), lisZeroingPermission (true), statePrevious (Kpt::state),
		  repeateCounter (0), lis (lis), correctKptDistance (0)
	{
//		static_assert (timeMod < timePrescale/256, "Выбранный таймер не даёт необходимой точности");
		setPassive ();
		scheduler.runIn( Command{ SoftIntHandler::from_method <Kpt, &Kpt::watchDog>(this), 0}, 2000000/Scheduler::discreetMks );
//		scheduler.runIn( Command{ SoftIntHandler (this, &Kpt::watchDog), 0}, 2000000/Scheduler::discreetMks );
	}

	void fall ()
	{
		fall( getImpulseTime() );
	}

	void fall (uint8_t upTime)
	{
		// Начало отрицательного импульса
		impulseWatchDog = true;
		if ( state.kptImp == 1 )
		{
			state.kptImp = 0;
			periodTime += upTime;
		}
		else
		{
			canSend (0);
		}

	}

	void rise ()
	{
		rise( getImpulseTime() );
	}

	void rise (uint8_t downTime)
	{
		// Конец отрицательного импульса
		impulseWatchDog = true;

		if ( state.kptImp == 0 )
		{
			state.kptImp = 1;
			periodTime += downTime;


			if ( downTime > 35) // длинный импульс -- конец периода
			{
				if (shortImpNumber == 0) // При отсутствии коротких импульсов (КЖ)
					periodTime *= 2; // реальный период в 2 раза меньше

				State stateNew = {State::Color::White, 1, true, state.type};

				// Цвет светофора
				stateNew.color = State::Color (1 << (2-shortImpNumber));
				if (shortImpNumber >= 3) // Если были ошибки определения цвета
					stateNew.correct = false;
				shortImpNumber = 0;

				// Тип КПТ
				if ( 147 < periodTime && periodTime < 173 ) // 1.6 +- 0.13  -- КПТ-5
					stateNew.type = State::Type::Kpt5;
				else if ( 173 <= periodTime && periodTime < 199 ) // 1.86 +- 0.13  -- КПТ-7
					stateNew.type = State::Type::Kpt7;
				else
					stateNew.correct = false;

				// Необходимо 2 одинаковых корректно расшифрованных периода
				if ( _cast(uint8_t, stateNew) == _cast(uint8_t, statePrevious) )
				{
					if ( stateNew.correct )
					{
						if ( stateNew.type != state.type ) // Проехали изостык
						{
							if ( lisZeroingPermission )
								lisZeroingPermission = false;
							else
								lis = correctKptDistance;
						}

						correctKptDistance = 0;
						if ( lisZeroingPermission )
							lis = 0;

						state = stateNew;
					}
				}
				else
					state.correct = false;
				statePrevious = stateNew;

				periodTime = 0; // Начало нового периода
	//			canSend ();
			}
			else // короткий импульс
				if (++shortImpNumber == 3) // Трёх коротких имульсов подряд быть не может
					state.correct = false;
		}
		else
		{
			canSend(1);
		}
	}

	void lisPlusPlus ()
	{
		if ( active )
			if (lis < 255)
				lis ++;
	}

	void correctKptDistancePlusPlus ()
	{
		if (correctKptDistance < 255)
			correctKptDistance ++;
	}

	void setActive ()
	{
		active = true;
	}

	void setPassive ()
	{
		active = false;
		lisZeroingPermission = true;
	}

	uint8_t& lis; // Коррекция изостыка
	uint8_t correctKptDistance; // Расстояние от последнего правильно распознанного периода КПТ
};



#endif /* KPT_H_ */
