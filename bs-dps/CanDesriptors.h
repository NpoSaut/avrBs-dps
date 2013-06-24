/*
 * CanDesriptors.h
 *
 */

#ifndef CANDESRIPTORS_H_
#define CANDESRIPTORS_H_

enum CanTx : uint16_t
{
	 SAUT_INFO_A =          ((uint16_t)0x148	<< 5) + 8, // 2908
	 SAUT_INFO_B =          ((uint16_t)0x168	<< 5) + 8, // 2D08
};

enum CanRx : uint16_t
{
	EKS_KLUB_MCO_STATE_A =    ((uint16_t)0x050	<< 5) + 8, // 0A08
	EKS_KLUB_MCO_STATE_B =    ((uint16_t)0x058	<< 5) + 8, // 0B08
	MLSB_SAUT_KPT_A =         ((uint16_t)0x175	<< 5) + 3, // 2EA3
	MLSB_SAUT_KPT_B =         ((uint16_t)0x176	<< 5) + 3, // 2EC3
	VDS_STATE_A =         	  ((uint16_t)0x2E0	<< 5) + 8, // 5C08
	VDS_STATE_B =         	  ((uint16_t)0x2E1	<< 5) + 8, // 5C28
};


#endif /* CANDESRIPTORS_H_ */
