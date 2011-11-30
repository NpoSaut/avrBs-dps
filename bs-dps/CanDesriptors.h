/*
 * CanDesriptors.h
 *
 */

#ifndef CANDESRIPTORS_H_
#define CANDESRIPTORS_H_

enum CanTx : uint16_t
{
	 IPD_STATE_A =			(0xC4	<< 5) + 8, // 1888
	 IPD_STATE_B =			(0xD4	<< 5) + 8, // 1A88
	 SAUT_INFO_A =          (0x233	<< 5) + 8, // 4668
	 SAUT_INFO_B =          (0x234	<< 5) + 8, // 4688
	 SYS_DATA =				(0x313	<< 5) + 5, // 6265
	 SYS_DATA_STATE =		(0x31F	<< 5) + 7, // 63E7
	 SYS_DATA_STATE2 =		(0x31E	<< 5) + 8, // 63C8
	 AUX_RESOURCE_IPD_A = 	((uint16_t)0x4C0	<< 5) + 5, // 9805
	 AUX_RESOURCE_IPD_B = 	((uint16_t)0x4D0	<< 5) + 5, // 9A05
	 AUX_RESOURCE_BS_A = 	((uint16_t)0x633	<< 5) + 5, // C665
	 AUX_RESOURCE_BS_B = 	((uint16_t)0x634	<< 5) + 5, // C685
	 MY_DEBUG_A =			((uint16_t)0x4CC	<< 5) + 8, // 9988
	 MY_DEBUG_B =			((uint16_t)0x4CD	<< 5) + 8  // 9988
};

enum CanRx : uint16_t
{
	 MCO_DATA =			  	(0x48	<< 5) + 5,
	 MCO_STATE_A =			(0x50	<< 5) + 8,
	 MCO_STATE_B =			(0x58	<< 5) + 8,
	 MCO_LIMITS_A =  		(0x52	<< 5) + 8,
	 MCO_LIMITS_B =  		(0x5A	<< 5) + 8,
	 SYS_KEY = 				(0x60	<< 5) + 1,
	 SYS_DATA_QUERY =		(0x70	<< 5) + 1,
	 MP_ALS_ON_A =			(0x80 	<< 5) + 0,
	 MP_ALS_OFF_A =			(0x84	<< 5) + 0,
	 MP_ALS_ON_B =			(0x81 	<< 5) + 0,
	 MP_ALS_OFF_B =			(0x85	<< 5) + 0,
	 MP_ALS_ON_TIME_A =		(0x80 	<< 5) + 1,
	 MP_ALS_OFF_TIME_A =	(0x84	<< 5) + 1,
	 MP_ALS_ON_TIME_B =		(0x81 	<< 5) + 1,
	 MP_ALS_OFF_TIME_B =	(0x85	<< 5) + 1,
	 IPD_DATE =				(0xC7	<< 5) + 7,
	 MM_DATA =				(0x211	<< 5) + 8,
	 BKSI_DATA =			(0x252	<< 5) + 5,
	 INPUT_DATA =			(0x310	<< 5) + 5,
	 SYS_DIAGNOSTICS = 		((uint16_t)0x468	<< 5) + 7
};


#endif /* CANDESRIPTORS_H_ */
