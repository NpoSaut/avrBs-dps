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
	bool lisCountPermission;

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


public:
	Kpt (uint8_t& status, uint8_t& lis)
		: status ( _cast(Status, status) ),
		  impulseWatchDog (false), periodTime (0), impulseStartTime (0),
		  shortImpNumber (0), lisCountPermission (false), statusPrevious (Kpt::status),
		  repeateCounter (0), lis (lis), correctKptDistance (0)
	{
//		static_assert (timeMod < timePrescale/256, "Выбранный таймер не даёт необходимой точности");
		setPassive ();
//		scheduler.runIn( Command{ SoftIntHandler (this, &Kpt::watchDog), 0}, 2000000/Scheduler::discreetMks );
	}

	void getCleverInfo (uint16_t correctionDecemeters, bool kptSwitch, uint8_t kptType)
	{
		if (kptType == 7)
		{
			status.type = Status::Kpt7;
			status.correct = 1;
		}
		else if (kptType == 5)
		{
			status.type = Status::Kpt5;
			status.correct = 1;
		}
		else
		{
			status.correct = 0;
		}

		if (active)
		{
			if ( kptSwitch == true )
				lisCountPermission = true;

			if ( lisCountPermission )
				lis = correctionDecemeters/12;
		}
	}

	void lisPlusPlus ()
	{
		if ( active && lisCountPermission )
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
		lisCountPermission = false;
		lis = 0;
	}

	uint8_t& lis; // Коррекция изостыка
	uint8_t correctKptDistance; // Расстояние от последнего правильно распознанного периода КПТ
};



#endif /* KPT_H_ */
