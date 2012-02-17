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
#include "CanDat.h"
#include "CanDesriptors.h"

template <typename CanDatType, CanDatType& canDat>
class EcAdjust
{
public:
	EcAdjust ()
		:  old( {0,0} ), ecOld( 0 ),
		  direction( 0 ), ecDirection( 0 ),
		  criticalMismatch( false ),
		  firstTime( true )
	{ }

	void takeEcData (int16_t pointerToMessage)
	{
		typedef const uint8_t Message[8];
		Message& message = *( (Message *)(pointerToMessage) );

		Bitfield<CardState>& cardState = _cast(Bitfield<CardState>, message[0]);
		if ( cardState->map && cardState->mapVerify && !cardState->error )
		{
			// Получение данных от ЭК
			Complex<int32_t> ec = {
				message [3], // Данные в обратном порядке
				message [4],
				message [5],
				(message [5] & (1 << 7) ? 0xFF : 0x00) // корректная работа с отрицательными
								};

			// Определение направления движения
			int8_t newEcDirection;
			if ( ec > ecOld )
				newEcDirection = ecDirection + 1;
			else if ( ec < ecOld )
				newEcDirection = ecDirection - 1;
			else
				newEcDirection = ecDirection;
			if ( abs(newEcDirection) <= 8 )
				ecDirection = newEcDirection;
			ecOld = ec;


		}
	}

	void adjust (int32_t& spatium)
	{
		if ( cardState->map && cardState->mapVerify && !cardState->error )
		{
			// Выбор масштаба
			uint32_t delta = abs(spatium - old[1]); // пройденный путь за "машинный цикл" ЭК - 1 сек (2 моих цикла)
			if ( delta > 255 )
				delta = 255;
//			uint8_t scale = delta < 8 ? 8 : delta; // сильное уменьшение масштаба приведёт к быстрому критическому расхождению
			uint8_t scale = delta;




			if ( firstTime )
			{
				firstTime = false;
				spatium = ec;
				old[0] = ec;
				old[1] = ec;
			}
			else
			{
				// Расчёт расхождения с ЭК
				int32_t mismatchOrig = ec - (old[0] + old[1])/2; // не возникнет переполнения, т.к. используются только 3 байта
				int32_t mismatchScaled = (mismatchOrig * 32) / scale;
				if ( scale >= 8 )
					criticalMismatch = ( abs(mismatchScaled) >= 256 );
				else
					criticalMismatch = ( abs(mismatchOrig) >= 64 ); // есть постоянная ошибка gps, не зависящая от скорости

				// Анализ направления ЭК
				if ( abs(ecDirection) == 8 && // достоверно определено по ЭК
						direction != (ecDirection<0) ) // не совпадает с ДПС
					criticalMismatch = true;

				// Корректировка
				int16_t cor = ( uint16_t (scale) * correction (abs(mismatchScaled)) ) / 256; // масштабирование
				correction = (mismatchScaled >= 0 ? cor : -cor); // антисимметричная
				spatium += correction/2;
				correction = correction - correction/2;
			}

			cardState->mapVerify = false; // Помечаю, что воспользовался этими данными
		}
		else
		{
			spatium += correction;
			correction = 0;
		}
		old[1] = old[0];
		old[0] = spatium;
	}

	void setDpsDirection (bool direction) { EcAdjust::direction = direction; }
	bool isMismatchCritical () const { return criticalMismatch;	}

private:
	uint8_t correction (uint32_t mismatch)
	{
		if ( mismatch > 1625 ) // Ограничение, чтобы не переполнится при вычислениях
			mismatch = 1625;

		uint32_t mismatch2 = mismatch*mismatch;
		uint32_t mismatch3 = mismatch2*mismatch;

		return ( uint32_t(5400) * mismatch2 ) / ( mismatch3 - (70*int32_t(mismatch2) - int32_t(mismatch) - 120000) );
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


	int32_t old[2]; // Показания, выданные в прошлый(0) и позапрошлый(1) раз
	int8_t correction; // Коррекция, не сделанная в прошлый раз.
	int32_t ecOld; // Показания от ЭК, полученные в прошлый раз. Для определения направления.
	int8_t ecDirection; // Направления движения по ЭК. + - вперёд, - - назад. 8 - достоверно.
	bool direction; // Направление по ДПС. Получаем извне.
	bool criticalMismatch; // Критическое расхождение
	bool firstTime; // Признак инициализации
};


#endif /* EC_ADJUST_H_ */
