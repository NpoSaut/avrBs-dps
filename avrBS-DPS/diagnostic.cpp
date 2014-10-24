/*
 * diagnostic.c
 *
 * Created: 21.10.2014 15:04:20
 *  Author: nazemnyh
 */ 

#include "diagnostic.h"
#include "avr/delay.h"
#include "avr/wdt.h"
#include "avr/interrupt.h"

Delegate<void (uint8_t)> diagnostic_storeDelegate;
Delegate<uint8_t (void)> diagnostic_restoreDelegate;
AuxResourceMessage diagnostic_sendMessageDelegate;

bool diagnostic_trySendAuxResource (const uint8_t (&message)[5])
{
	volatile bool successSend = false;
	for (uint8_t i = 0; i++ < 100 && !successSend; _delay_us(100))
		successSend = diagnostic_sendMessageDelegate(message);
	return successSend;
}

bool diagnostic_sendRestartReason (RestartReason reason, bool previousWrited = false, uint16_t detail = 0)
{
	uint8_t data [5] = {
		1,
		previousWrited ? 2 : 1,
		uint8_t (reason),
		uint8_t (detail >> 8),
		uint8_t (detail)
	};
	
	return diagnostic_trySendAuxResource(data);
}

void diagnostic_sendWarninReason (RestartReason reason, uint16_t detail)
{
	uint8_t data [5] = {
		2,
		3,
		uint8_t (reason),
		uint8_t (detail >> 8),
		uint8_t (detail)
	};
		
	diagnostic_trySendAuxResource(data);
}

void diagnostic_reboot ()
{
	cli ();
	do
	{
		wdt_enable(WDTO_15MS);
		for (;;) { asm volatile ("nop"); }
	}
	while (0);
}

void diagnostic_restart (RestartReason reason, uint16_t detail)
{
	if (!diagnostic_sendRestartReason(reason, false))
		diagnostic_storeDelegate (uint8_t(reason));
	
	diagnostic_reboot();
}

void diagnostic_sendReasonOfPreviousRestart ()
{
	RestartReason reason = RestartReason (diagnostic_restoreDelegate());
	
	if (!reason == POWER_OFF)
	{
		if (diagnostic_sendRestartReason (reason, true))
		diagnostic_storeDelegate (RestartReason::POWER_OFF);
	}
}