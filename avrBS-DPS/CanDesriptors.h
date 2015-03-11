/*
 * CanDesriptors.h
 *
 */

#ifndef CANDESRIPTORS_H_
#define CANDESRIPTORS_H_

enum CanTx : uint16_t
{
	 SYS_KEY =				(0x60	<< 5) + 1, // 0C01
	 IPD_STATE_A =			(0xC4	<< 5) + 8, // 1888
	 IPD_STATE_B =			(0xD4	<< 5) + 8, // 1A88
	 IPD_DPS_FAULT =		(0xD6	<< 5) + 2, // 1AC2
	 IPD_PARAM_A =			(0xC6	<< 5) + 8, // 18C8
	 IPD_PARAM_B =			(0xC7	<< 5) + 8, // 18E8
	 IPD_NEUTRAL =			(0xC8	<< 5) + 3, // 1903
	 SAUT_INFO_A =          (0x233	<< 5) + 8, // 4668
	 SAUT_INFO_B =          (0x234	<< 5) + 8, // 4688
	 VDS_STATE_A = 			(0x2E0	<< 5) + 2, // 5C02
	 VDS_STATE_B = 			(0x2E1	<< 5) + 2, // 5C22
	 SYS_DATA_A =			(0x313	<< 5) + 5, // 6265
	 SYS_DATA_B = 			(0x314	<< 5) + 5, // 6285
	 SYS_DATA_STATE_A =		(0x31F	<< 5) + 7, // 63E7
	 SYS_DATA_STATE_B =		(0x319	<< 5) + 7, // 6327
	 SYS_DATA_STATE2_A =	(0x31E	<< 5) + 8, // 63C8
	 SYS_DATA_STATE2_B =	(0x318	<< 5) + 8, // 6308
	 MPH_STATE_A =			(0x31C	<< 5) + 4, // 6384
	 MPH_STATE_B =			(0x31D	<< 5) + 4, // 63A4
	 FU_DEV = 				((uint16_t)0x337	<< 5) + 8, // 66E8
	 AUX_RESOURCE_IPD_A = 	((uint16_t)0x4C0	<< 5) + 5, // 9805
	 AUX_RESOURCE_IPD_B = 	((uint16_t)0x4D0	<< 5) + 5, // 9A05
	 AUX_RESOURCE_BS_A = 	((uint16_t)0x633	<< 5) + 5, // C665
	 AUX_RESOURCE_BS_B = 	((uint16_t)0x634	<< 5) + 5, // C685
	 AUX_RESOURCE_VDS_A = 	((uint16_t)0x6E1	<< 5) + 5, // DC25
	 AUX_RESOURCE_VDS_B = 	((uint16_t)0x6E2	<< 5) + 5, // DC45
	 MY_DEBUG_A =			((uint16_t)0x4CC	<< 5) + 4, // 9984
	 MY_DEBUG_B =			((uint16_t)0x4DC	<< 5) + 4, // 9B84
	 MY_KPT_A = 			((uint16_t)0x4CE	<< 5) + 1, // 99C1
	 MY_KPT_B = 			((uint16_t)0x4DE	<< 5) + 1, // 9BC1
};

enum CanRx : uint16_t
{
	 MCO_DATA =			  		(0x48	<< 5) + 5, // 905
	 MCO_STATE_A =				(0x50	<< 5) + 8, // A08
	 MCO_STATE_B =				(0x58	<< 5) + 8, // B08
	 MCO_LIMITS_A =  			(0x52	<< 5) + 8, // A48
	 MCO_LIMITS_B =  			(0x5A	<< 5) + 8, // B48
	 SYS_DATA_QUERY =			(0x70	<< 5) + 1, // E01
	 MP_ALS_ON_A =				(0x80 	<< 5) + 0, // 1000
	 MP_ALS_OFF_A =				(0x84	<< 5) + 0, // 1080
	 MP_ALS_ON_B =				(0x81 	<< 5) + 0, // 1020
	 MP_ALS_OFF_B =				(0x85	<< 5) + 0, // 10A0
	 MP_ALS_ON_TIME_A =			(0x80 	<< 5) + 1, // 1001
	 MP_ALS_OFF_TIME_A =		(0x84	<< 5) + 1, // 1081
	 MP_ALS_ON_TIME_B =			(0x81 	<< 5) + 1, // 1021
	 MP_ALS_OFF_TIME_B =		(0x85	<< 5) + 1, // 10A1
	 MM_DATA =					(0x211	<< 5) + 8, // 4228
	 MM_NEUTRAL =				(0x215	<< 5) + 8, // 42A8
	 BKSI_DATA =				(0x252	<< 5) + 5, // 4A45
	 INPUT_DATA =				(0x310	<< 5) + 5, // 6205
	 FU_INIT =					((uint16_t)0x335	<< 5) + 8, // 66A8
	 SYS_DIAGNOSTICS = 			((uint16_t)0x468	<< 5) + 7, // 8D07
	 AUX_RESOURCE_MCO_A =		((uint16_t)0x440	<< 5) + 5, // 8805
	 AUX_RESOURCE_MCO_B =		((uint16_t)0x450	<< 5) + 5, // 8A05
	 IPD_EMULATION =		    ((uint16_t)0x67F	<< 5) + 8,  // CFE8
};


#endif /* CANDESRIPTORS_H_ */
