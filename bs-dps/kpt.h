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

namespace KptInternal
{
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

	typedef KptInternal::Status Status;

	Status& status; // Ссылка на внешнюю переменную, в которую выводится состояние

	bool active; // Разрешение выводить в линию связи

	bool impulseWatchDog;
	uint8_t periodTime;
	uint8_t impulseStartTime;
	uint8_t shortImpNumber;
	uint8_t lisZeroingPermission;

	Status statusPrevious;
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
//		if (reg.portB.pin7 == 0) // первый полукомплект
//			canDat.template send<AUX_RESOURCE_IPD_A> (packet);
//		else
//			canDat.template send<AUX_RESOURCE_IPD_B> (packet);
	}

	void watchDog (uint16_t)
	{
		if ( impulseWatchDog == false ) // не было изменений
		{
			if (status.color == Status::Color::RedYellow)
				status = { 	Status::Color::Red,
							status.kptImp,
							false,
							(Status::Type) ~((uint8_t) status.type)
						};
			else if (status.color != Status::Color::Red)
				status = { 	Status::Color::White,
							status.kptImp,
							false,
							status.type
						};
//			canSend ();
		}
		else
			impulseWatchDog = false;

		scheduler.runIn( Command{ SoftIntHandler::from_method <Kpt, &Kpt::watchDog>(this), 0 }, 2000000/Scheduler::discreetMks );
//		scheduler.runIn( Command{SoftIntHandler (this, &Kpt::watchDog), 0}, 2000000/Scheduler::discreetMks );
	}


public:
	Kpt (uint8_t& status, uint8_t& lis)
		: status ( _cast(Status, status) ),
		  impulseWatchDog (false), periodTime (0), impulseStartTime (0),
		  shortImpNumber (0), lisZeroingPermission (true), statusPrevious (Kpt::status),
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
		if ( status.kptImp == 1 )
		{
			status.kptImp = 0;
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

		if ( status.kptImp == 0 )
		{
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

						status = statusNew;
					}
				}
				else
					status.correct = false;
				statusPrevious = statusNew;

				periodTime = 0; // Начало нового периода
	//			canSend ();
			}
			else // короткий импульс
				if (++shortImpNumber == 3) // Трёх коротких имульсов подряд быть не может
					status.correct = false;
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
