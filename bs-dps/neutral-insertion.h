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

#include <cpp/universal.h>
#include <cpp/scheduler.h>
#include <cpp/delegate/delegate.hpp>
#include "CanDat.h"
#include "CanDesriptors.h"
#include "eeprom.h"

template <  typename CanDatType, CanDatType& canDat,
			typename Scheduler, Scheduler& scheduler,
			typename DpsType, DpsType& dps >
class NeutralInsertion
{
public:
	NeutralInsertion (bool active)
		: type (Type::NoTarget), coord (0), numberFaultSendTrys (0)
	{
		length = trainLengthCalc();
		if (active)
			scheduler.runIn(
					Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
					500	);
	}

	void getEcData (uint16_t)
	{
		const uint8_t (&data) [8] = canDat.template get <CanRx::MM_NEUTRAL> ();

		uint8_t inputType = data[1] & 0b11;
		if ( inputType == 0 )
			type = Type::NeutralInsertion;
		else if ( inputType == 1 )
			type = Type::SystemChange;
		else
			return;

		length = trainLengthCalc() + data[5] + uint16_t(data[4] & 0x1F) * 256; // длина поезда + длина вставки
		coord = uint16_t (data[3]) + uint16_t (data[2]) * 256;
	}
private:
	void sendData (uint16_t)
	{
		Complex<uint16_t> outDistance = 0;
		if ( type != Type::NoTarget )
		{
			int16_t distance = coord - uint16_t(dps.spatiumMeters);
			if ( distance <= -length )
				type = Type::NoTarget;

			if ( distance > 0 && distance < 1500 )
				outDistance = distance;

			uint8_t data[3] = { (uint8_t)type, outDistance[0], outDistance[1] };
			if ( canDat.template send<CanTx::IPD_NEUTRAL> (data) )
			{
				numberFaultSendTrys = 0;
				scheduler.runIn(
						Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
						500	);
			}
			else if ( numberFaultSendTrys < 4 )
			{
				numberFaultSendTrys ++;
				scheduler.runIn(
						Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
						100	);
			}
			else
			{
				numberFaultSendTrys = 0;
				scheduler.runIn(
						Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
						500	);
			}
		}
		else
		{
			numberFaultSendTrys = 0;
			scheduler.runIn(
					Command { SoftIntHandler::from_method <NeutralInsertion, &NeutralInsertion::sendData> (this), 0 },
					500	);
		}
	}
	const uint16_t trainLengthCalc () const
	{
		//uint8_t wagonLength =  eeprom_read_dword (&eeprom.club.category) > 5 ? 16 : 25;
		uint8_t wagonLength = 25;
		uint32_t conventionalWagonNumber = eeprom_read_dword (&eeprom.club.lengthWagon);
		return conventionalWagonNumber * wagonLength + 50;
	}

	enum class Type : uint8_t
	{
		NoTarget,
		NeutralInsertion,
		SystemChange
	};
	Type type;
	int16_t length;
	uint16_t coord;
	uint8_t numberFaultSendTrys;
};


#endif /* NEUTRAL_INSERTION_H_ */
