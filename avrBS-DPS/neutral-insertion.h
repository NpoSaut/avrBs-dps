/*
 * neutral-insertion.h
 *
 * Нейтральная вставка
 * *******************
 *
 *  Created on: 13.10.2011
 *      Author: nazemnyh
 */

#ifndef NEUTRAL_INSERTION_H_
#define NEUTRAL_INSERTION_H_

#include <cpp/dispatcher.h>
#include <cpp/universal.h>
#include <cpp/scheduler.h>
#include <cpp/delegate/delegate.hpp>
#include "cpp/can-dat.h"
#include "CanDesriptors.h"
#include "eeprom.h"

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler,
			typename DpsType, DpsType& dps >
class NeutralInsertion
{
public:
	NeutralInsertion ()
		: active (false), type (Type::NoTarget), coord (0)
	{}

	void getEcData (uint16_t pointerToData)
	{
		typedef const uint8_t Data[8];
		Data& data = *((Data*) pointerToData);

		uint16_t getCoord = uint16_t (data[3]) + uint16_t (data[2]) * 256;
		int16_t distance = getCoord - uint16_t(dps.spatiumMeters);
		if ( distance > 0 && distance < 1500 )
		{
			coord = getCoord;
			
			uint16_t trainLength;
			if ( !trainLengthCalc(trainLength) && !active ) // если не удалось прочитать в первый раз
				trainLength = 300;
			length = trainLength + data[5] + uint16_t(data[4] & 0x1F) * 256; // длина поезда + длина вставки
			
			uint8_t inputType = data[1] & 0b11;
			if ( inputType == 0 )
				type = Type::NeutralInsertion;
			else if ( inputType == 1 )
				type = Type::SystemChange;
			else
				return;
				
			if (!active)
			{
				active = true;
				sendData(0);
			}
		}
	}
private:
	void sendData (uint16_t)
	{
		Complex<uint16_t> outDistance = 0;
		if ( type != Type::NoTarget )
		{
			int16_t distance = coord - uint16_t(dps.spatiumMeters);
			if ( distance + length <= 0 )
			{
				type = Type::NoTarget;
				active = false;
			}

			if ( distance > 0 && distance < 1500 )
				outDistance = distance;
		}

		if (active)
		{
			uint8_t data[3] = { (uint8_t)type, outDistance[0], outDistance[1] };
			if ( canDat.template send<CanTx::IPD_NEUTRAL> (data) )
			{
				scheduler.runIn(
					Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
					500	);
			}
			else
			{
				sendAttempts ++;
				dispatcher.add( SoftIntHandler::from_method<NeutralInsertion, &NeutralInsertion::sendData>(this), 0 );
			}
		}
	}
	bool trainLengthCalc (uint16_t& length)
	{
		uint32_t conventionalWagonNumber;
//		uint32_t category, conventionalWagonNumber;
//		if ( eeprom.club.property.category.read (category) &&
		if ( eeprom.club.property.lengthWagon.read (conventionalWagonNumber) )
		{
//			uint8_t wagonLength =  uint8_t(category) > 5 ? 16 : 25;
			uint8_t wagonLength = 25;
			length = uint16_t(conventionalWagonNumber) * wagonLength + 50;
			return true;
		}
		else
			return false;
	}

	union
	{
		bool active;
		uint8_t sendAttempts;
	};
	enum class Type : uint8_t
	{
		NoTarget,
		NeutralInsertion,
		SystemChange
	};
	Type type;
	uint16_t length;
	uint16_t coord;
};


#endif /* NEUTRAL_INSERTION_H_ */
