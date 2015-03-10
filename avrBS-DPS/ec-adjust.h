/*
 * ec-adjust.h
 *
 * Подсктройка под электронную карту
 * *********************************
 *
 *  Created on: 13.10.2011
 *      Author: nazemnyh
 */

#ifndef EC_ADJUST_H_
#define EC_ADJUST_H_

#include <cpp/universal.h>
#include <cpp/delegate/delegate.hpp>
#include "cpp/can-dat.h"
#include "CanDesriptors.h"

template < typename CanDatType, CanDatType& canDat >
class EcAdjust
{
public:
	EcAdjust ( Delegate<uint16_t ()> getVelocity, Delegate<int32_t ()> getSpatium )
		: getVelocity( getVelocity ), getSpatium( getSpatium ),
		  correctionRemainder( 0 ), correctionStep ( 2 ),
		  criticalMismatch( false ),
		  firstTime( true ),
		  firstValue( 0 )
	{ }

	void takeEcData (int16_t pointerToMessage)
	{
		typedef const uint8_t Message[8];
		Message& message = *( (Message *)(pointerToMessage) );

		Bitfield<CardState>& cardState = _cast(Bitfield<CardState>, message[0]);
		if ( (cardState.map && cardState.mapVerify && !cardState.error) ||
			 firstTime ) // первый раз после загрузки берём значение счётчика от ЭК невзирая на флаги
		{
			// Получение данных от ЭК
			Complex<int32_t> ec = {
				message [3], // Данные в обратном порядке
				message [4],
				message [5],
				(message [5] & (1 << 7) ? 0xFF : 0x00) // корректная работа с отрицательными
								};

			// В первый раз после загрузки взять данные от ЭК
			if ( firstTime )
				firstValue = ec;
			else
			{
				// Масштабирование
				uint8_t scaleY = getVelocity()/256 * 10 / 18; // скорость в м/с
				uint8_t scaleX;
				if ( scaleY < 4 )
					scaleX = 4;
				else if ( scaleY > 16 )
					scaleX = 16;
				else
					scaleX = scaleY;

				// Проверка на критическое расхождение
				int32_t mismatch = ec - getSpatium();
				if ( abs(mismatch) >= 256 )
					criticalMismatch = true;
				else
				{
					uint8_t mismatchRestricted = abs(mismatch);
					criticalMismatch = ( mismatchRestricted * 16/scaleX >= 256 );
				}

				// Рассчитываем корректировку
				correctionRemainder = int16_t (correction( mismatch * 16/scaleX )) * scaleY/64;
				correctionStep = 0;
			}
		}
	}

	void adjust (int32_t& spatium)
	{
		// В первый раз после загрузки задать значение пройденного пути, полученное от ЭК
		if ( firstTime )
		{
			firstTime = false;
			spatium = firstValue;
		}
		else
		{
			// Чтобы сделать коррекцию за 2 посылки
			if ( correctionStep == 0 ) // После расчёта
			{
				spatium += correctionRemainder/2;
				correctionRemainder = correctionRemainder - correctionRemainder/2;
				correctionStep ++;
			}
			else if ( correctionStep == 1 ) // Половина коррекции уже добавлена
			{
				spatium += correctionRemainder;
				correctionStep ++;
			}
		}
	}

	bool isMismatchCritical () const { return criticalMismatch;	}

private:
	int8_t correction (int32_t mismatch)
	{
		uint32_t mismatch1 = abs(mismatch);
		if ( mismatch1 >= 370 )
			return 0;

		uint32_t mismatch2 = mismatch1*mismatch1;
		uint32_t mismatch3 = mismatch2*mismatch1;

		int8_t cor = ( uint32_t(4) * (uint32_t(10000)*mismatch2 - uint32_t(27)*mismatch3) ) /
					 ( uint32_t(1600000) - uint32_t(26000)*mismatch1 + uint32_t(900)*mismatch2 - uint32_t(2)*mismatch3 );

		return mismatch >= 0 ? cor : -cor;
	}

	struct CardState
	{
		uint8_t extrapolation	:1;
		uint8_t station			:1;
		uint8_t mapVerify		:1; // этот бит (дублирующий 6-ой) я использую как флаг обновления данных
		uint8_t zeroing			:1;
		uint8_t snsOk			:1;
		uint8_t error			:1;
		uint8_t map				:1;
		uint8_t tks				:1;
	};

	Delegate<uint16_t ()> getVelocity;
	Delegate<int32_t ()> getSpatium;

	int8_t correctionRemainder; // Коррекция, не сделанная в прошедшие разы.
	uint8_t correctionStep; // Шаг коррекции: 0 - расчитана, 1 - первая половина, 2 - вторая половина.
	bool criticalMismatch; // Критическое расхождение

	bool firstTime; // Признак инициализации
	int32_t firstValue; // Первое пришедшее значение пройденного пути
};


#endif /* EC_ADJUST_H_ */
