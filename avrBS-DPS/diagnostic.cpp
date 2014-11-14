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
Delegate<void ()> diagnostic_watchdogResetDelegate;

bool diagnostic_internal_trySend (const uint8_t (&message)[5])
{
	volatile bool successSend = false;
	for (uint8_t i = 0; i++ < 80 && !successSend; _delay_us(100))
		successSend = diagnostic_sendMessageDelegate(message);
	return successSend;
}

enum diagnostic_internal_Situation
{
	RESTART = 1,
	LOAD = 3,
	INFO = 4
};
	
bool diagnostic_internal_sendAuxResource (diagnostic_internal_Situation situation, RestartReason reason,  uint8_t detail1 = 0, uint8_t detail2 = 0, uint8_t detail3 = 0)
{
	uint8_t data [5] = {
		uint8_t (situation),
		uint8_t (reason),
		detail1,
		detail2,
		detail3
	};
	
	return diagnostic_internal_trySend(data);
}

void diagnostic_internal_reboot ()
{
	cli ();
	do
	{
		wdt_enable(WDTO_15MS);
		for (;;) { asm volatile ("nop"); }
	}
	while (0);
}

void diagnostic_restart (RestartReason reason, uint8_t detail1, uint8_t detail2, uint8_t detail3)
{	
	diagnostic_watchdogResetDelegate ();
	
	if (!diagnostic_internal_sendAuxResource(RESTART, reason, detail1, detail2, detail3))
	{
		diagnostic_watchdogResetDelegate ();
		diagnostic_storeDelegate (uint8_t(reason));
	}
	
	diagnostic_internal_reboot();
}

bool diagnostic_sendInfo (RestartReason reason, uint8_t detail1, uint8_t detail2, uint8_t detail3)
{
	diagnostic_watchdogResetDelegate ();
	return diagnostic_internal_sendAuxResource(INFO, reason, detail1, detail2, detail3);
}

void diagnostic_sendReasonOfPreviousRestart ()
{
	RestartReason reason = RestartReason (diagnostic_restoreDelegate());
	
	if (reason != POWER_OFF)
	{
		if (diagnostic_internal_sendAuxResource(LOAD, reason, 0, 0, 0))
		{
			diagnostic_watchdogResetDelegate ();
			diagnostic_storeDelegate (RestartReason::POWER_OFF);
		}
	}
}