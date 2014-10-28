/*
 * diagnostic.h
 *
 * Created: 21.10.2014 11:45:56
 *  Author: nazemnyh
 */ 


#ifndef DIAGNOSTIC_H_
#define DIAGNOSTIC_H_

#include "stdint.h"
#include "cpp/delegate/delegate.hpp"

enum RestartReason
{
	POWER_OFF = 0,
	PROGRAM_MODE = 1,
	BUTTON_PRESS = 2,
	CO_LOST = 3,
	IPD_OUT = 4,
	DPS_OUT = 5,
	MPH_OUT = 6,
	CAN_BUSOFF = 7,
	DISPATCHER_OVER = 8,
	SCHEDULER_FULL = 9,
	WATCHDOG = 10,
	UNDEFINED = 0xFF
};

// Callback'и, которые необходимо инициализировать
extern Delegate<void (uint8_t)> diagnostic_storeDelegate;
extern Delegate<uint8_t ()> diagnostic_restoreDelegate;
typedef Delegate<bool (const uint8_t (&)[5])> AuxResourceMessage;
extern AuxResourceMessage diagnostic_sendMessageDelegate;

// Уходе в перезагрузку с предварительной выдачей сообщения AuxResource с причиной перезапуска
//  Делает попытку отправить причину перезапуска с помощью diagnostic_sendMessageDelegate в течении ~10 мсек
//  Если не удаётся, то сохраняет причину с помощью diagnostic_storeDelegate
void diagnostic_restart (RestartReason reason, uint16_t detail = 0);

// Выдает сообщение с сохранённой причиной предыдущего перезапуска
//  Читает сохраненное значение с помощью diagnostic_restoreDelegate
//  Отправляет сообщение с помощью diagnostic_sendMessageDelegate
void diagnostic_sendReasonOfPreviousRestart ();

// Выдаёт сообщение с предупреждением (не перезагружается)
//  Пытается сделать это в течении ~10 мсек
//  Не сохраняет в случае неудачи
void diagnostic_sendWarninReason (RestartReason reason, uint16_t detail = 0);

#endif /* DIAGNOSTIC_H_ */