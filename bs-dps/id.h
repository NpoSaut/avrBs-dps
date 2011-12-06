/*
 * id.h
 *
 *	Храним ID во флеше
 *	******************
 *
 */

#ifndef ID_H_
#define ID_H_

#include <avr/pgmspace.h>

#define VerProg_Pr 108


struct IdentifyDoc
{
	uint16_t	size;												// [100] 0-1 - байт размер программы в параграфах
	uint16_t	crc;												// [102]
	uint8_t		temp;          										// [104] 4  - ?????? ??????? ? ??????????
	uint8_t		version;											// [105]
	uint8_t		year;   											// [106] Год 2007-1980
	uint8_t		modif;        										// [107] Вариант блока + Модернизация
	uint8_t		manth;         										// [108] Месяц
	uint16_t	number;        										// [109] Номер блока
	uint16_t	ks;													// [10B]
	uint8_t		fuseLow;											// [10D]
	uint8_t		fuseHigh;											// [10E]
	uint8_t		fuseExt;											// [10F]
};

//uint16_t ID  __attribute__ ((section (".id"))) = 0xab;

IdentifyDoc ID  __attribute__ ((section (".id"))) = {0,0,0,VerProg_Pr,25,17,1,1,((25+17+1+1+VerProg_Pr)<<8),0xDF,0xD9,0xFD};
//IdentifyDoc ID  __attribute__ ((section (".id"))) = {0xaaaa,0xaaaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaaaa,0xaaaa,0xaa,0xaa,0xaa};
//IdentifyDoc ID  __attribute__ ((section (".id"))) = {0,0,0,0,0,0,0,0,0,0,0,0};
//IdentiyDoc ID  __attribute__ ((section (".id"))) = {0xffff,0xffff,0xff,0xff,0xff,0xff,0xff,0xffff,0xffff,0xff,0xff,0xff};
//IdentiyDoc ID  __attribute__ ((section (".id"))) = {0x1234,0x5678,255,255,255,255,255,0xffff,0xffff,255,255,255};

//uint16_t testID __attribute__ ((section (".id"))) = 0xaaaa;

// ----------------------------------ATTENTION--------------------------------------
// .id Section must be placed by linker to address 0x0100.
// We modify avr/lib/ldscripts/avr5.x to allow place .id between .vectors and .init0
// It normally use --script-file option to linker instead it, but it work bad
// New file store at name "ld-script.x"
// ---------------------------------------------------------------------------------

#endif /* ID_H_ */
