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
	UNDEFINED = 0xFF
};

// Callback'�, ������� ���������� ����������������
extern Delegate<void (uint8_t)> diagnostic_storeDelegate;
extern Delegate<uint8_t ()> diagnostic_restoreDelegate;
typedef Delegate<bool (const uint8_t (&)[5])> AuxResourceMessage;
extern AuxResourceMessage diagnostic_sendMessageDelegate;

// ����� � ������������ � ��������������� ������� ��������� AuxResource � �������� �����������
//  ������ ������� ��������� ������� ����������� � ������� diagnostic_sendMessageDelegate � ������� ~10 ����
//  ���� �� ������, �� ��������� ������� � ������� diagnostic_storeDelegate
void diagnostic_restart (RestartReason reason, uint16_t detail = 0);

// ������ ��������� � ���������� �������� ����������� �����������
//  ������ ����������� �������� � ������� diagnostic_restoreDelegate
//  ���������� ��������� � ������� diagnostic_sendMessageDelegate
void diagnostic_sendReasonOfPreviousRestart ();

// ����� ��������� � ��������������� (�� ���������������)
//  �������� ������� ��� � ������� ~10 ����
//  �� ��������� � ������ �������
void diagnostic_sendWarninReason (RestartReason reason, uint16_t detail = 0);

#endif /* DIAGNOSTIC_H_ */