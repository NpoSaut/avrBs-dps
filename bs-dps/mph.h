/*
 * mph.h
 *
 * Модуль постоянных характеристик
 * *******************************
 *
 *  Created on: 13.10.2011
 *      Author: nazemnyh
 */

#ifndef MPH_H_
#define MPH_H_

#include <cpp/universal.h>
#include "CanDat.h"
#include "CanDesriptors.h"
#include "eeprom.h"

template < typename CanDatType, CanDatType& canDat,
		   typename Scheduler, Scheduler& scheduler,
		   typename SautCom, SautCom& sautCom >
class MPH
{
public:
	MPH ()
	{
		scheduler.runIn(
				Command {SoftIntHandler::from_method<MPH,&MPH::sendState> (this), 0},
				500 );
	}

	void writeConfirm (uint16_t packetAdr)
	{
		write (packetAdr);
		read (packetAdr);
	}

	void write (uint16_t packetAdr)
	{
		Packet& packet = *( (Packet *)(packetAdr) );

		if ( packet.parameter == 9 )
			return;

		if ( packet.parameter == 1 )
			packet.parameter = 23;

		write (packet);
	}

	void read (uint16_t parameterAdr)
	{
		const uint8_t& parameter = *((uint8_t *) parameterAdr);

		if ( parameter == 9 || parameter == 1 )
			return;

		uint8_t* adr = (uint8_t *) ( (uint16_t)(&eeprom.club) + parameter*4 );
		uint8_t packet[5] = {
					parameter,
					eeprom_read_byte (adr+3),
					eeprom_read_byte (adr+2),
					eeprom_read_byte (adr+1),
					eeprom_read_byte (adr)
						};
		canDat.template send<CanTx::SYS_DATA> (packet);
	}

private:
	struct Packet
	{
		uint8_t parameter;
		uint8_t data[4];
	};

	void write (const Packet& packet)
	{
		uint8_t* adr = (uint8_t *) ( (uint16_t)(&eeprom.club) + packet.parameter*4 );

		eeprom_update_byte (adr  , packet.data[3]);
		eeprom_update_byte (adr+1, packet.data[2]);
		eeprom_update_byte (adr+2, packet.data[1]);
		eeprom_update_byte (adr+3, packet.data[0]);
	}

	// Посылает в CAN дескрипоторый SYS_DATA_STATE и SYS_DATA_STATE_2
	void sendState (uint16_t)
	{
		bool success =
		scheduler.runIn(
				Command {SoftIntHandler::from_method<MPH,&MPH::sendState> (this), 0},
				500 );

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
				success,
				scheduler.fill,
				sautCom.termTime
								};
		canDat.template send<CanTx::SYS_DATA_STATE2> (sysDataState2);
		sautCom.termTime = 0;

		Complex<uint16_t> trainNumber = eeprom_read_word ((const uint16_t *) &eeprom.club.train);
		uint8_t ipdParam[8] = {
				(uint8_t) eeprom_read_byte ((const uint8_t *) &eeprom.club.trackMPH),
				trainNumber[0],
				trainNumber[1],
				0,
				0,
				0,
				0,
				0
							};
		canDat.template send<CanTx::IPD_PARAM> (ipdParam);
	}
};


#endif /* MPH_H_ */
