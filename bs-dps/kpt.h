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

class Clock10ms
{
public:
	Clock10ms ()
		: alarm10ms ( InterruptHandler::from_method< Clock10ms, &Clock10ms::timePlusPlus > (this) ) {}

	const Safe<uint8_t>& time () const
	{
		return time_;
	}

	SoftIntHandler overflowHandler;

private:
	Alarm<Alarm3A, 10000> alarm10ms;
	Safe<uint8_t> time_;

	void timePlusPlus ()
	{
		if (++time_ == 255)
			dispatcher.add (overflowHandler, 0);
	}
};

// Расшифровка КПТ
//template <const Safe<uint16_t>& time>
class Kpt
{
private:
	struct Status
	{
		enum Color
		{
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

	Status& status; // Ссылка на внешнюю переменную, в которую выводится состояние

	Clock10ms clock;
	uint8_t impulseWatchDogCounter;
	uint8_t periodTime;
	uint8_t impulseStartTime;
	uint8_t shortImpNumber;
	uint8_t lisZeroingPermission;

	Status statusPrevious;
	uint8_t repeateCounter;

	uint8_t getImpulseTime ()
	{
		uint8_t time;
		ATOMIC
		{
			time = clock.time() - impulseStartTime;
			impulseStartTime = clock.time();
		}
		return time;
	}

	void watchDog (uint16_t)
	{
		if ( impulseWatchDogCounter == 0 ) // не было изменений
		{
			if (status.color == Status::Color::RedYellow)
				status = { 	Status::Color::Red,
							0,
							false,
							(Status::Type) ~((uint8_t) status.type)
						};
			else if (status.color != Status::Color::Red)
				status = { 	Status::Color::White,
							0,
							false,
							status.type
						};
		}
		else
			impulseWatchDogCounter = 0;
	}


public:
	Kpt (uint8_t& status, uint8_t& lis)
		: status ( _cast(Status, status) ), clock (), lis (lis)
	{
		clock.overflowHandler = SoftIntHandler::from_method <Kpt, &Kpt::watchDog> (this);
		lisZeroingPermission = true;
	}

	void fall ()
	{
		fall( getImpulseTime() );
	}

	void fall (uint8_t upTime)
	{
		// Начало отрицательного импульса
		impulseWatchDogCounter ++;
		status.kptImp = 0;
		periodTime += upTime;

	}

	void rise ()
	{
		rise( getImpulseTime() );
	}

	void rise (uint8_t downTime)
	{
		// Конец отрицательного импульса
		impulseWatchDogCounter ++;
		status.kptImp = 1;
		periodTime += downTime;


		if ( downTime > 35) // длинный импульс -- конец периода
		{
			if (shortImpNumber == 0) // При отсутствии коротких импульсов (КЖ)
				periodTime *= 2; // реальный период в 2 раза меньше

			Status statusNew = {Status::Color::White, 1, true, status.type};

			// Цвет светофора
			statusNew.color = Status::Color (1 << (2-shortImpNumber));
			if (shortImpNumber >= 3) // Если были ошибки определения цвета
				statusNew.correct = false;
			shortImpNumber = 0;

			// Тип КПТ
			if ( 147 < periodTime && periodTime < 173 ) // 1.6 +- 0.13  -- КПТ-5
				statusNew.type = Status::Type::Kpt5;
			else if ( 173 <= periodTime && periodTime < 199 ) // 1.86 +- 0.13  -- КПТ-7
				statusNew.type = Status::Type::Kpt7;
			else
				statusNew.correct = false;

			// Необходимо 2 одинаковых корректно расшифрованных периода
			if ( _cast(uint8_t, statusNew) == _cast(uint8_t, statusPrevious) )
			{
				if ( statusNew.correct )
				{
					if ( statusNew.type != status.type ) // Проехали изостык
					{
						if ( lisZeroingPermission )
							lisZeroingPermission = false;
						else
							lis = correctKptDistance;
					}

					correctKptDistance = 0;
					if ( lisZeroingPermission )
						lis = 0;
				}
				status = statusNew;
			}
			else
				status.correct = false;
			statusPrevious = statusNew;

			periodTime = 0; // Начало нового периода
		}
		else // короткий импульс
			if (++shortImpNumber == 3) // Трёх коротких имульсов подряд быть не может
				status.correct = false;
	}

	void lisReadConfirm ()
	{
		lisZeroingPermission = true;
	}

	void lisCountBan ()
	{
		lisZeroingPermission = true;
	}

	uint8_t& lis; // Коррекция изостыка
	uint8_t correctKptDistance; // Расстояние от последнего правильно распознанного периода КПТ
};



#endif /* KPT_H_ */
