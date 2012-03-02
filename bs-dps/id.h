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

#define PROGRAM_VERSION 125

struct IdentifyDoc {
	uint16_t size; 			// [100] Размер программы, в параграфах
	uint16_t controlSumm;	// [102] Сумма всех word программы (дополнение до 0)
	uint8_t idSize;			// [104] Размер ID области в параграфах
	uint8_t version;		// [105] Номер версии
	uint8_t year;			// [106] Год 1980-2043
	uint8_t modif;			// [107] Вариант блока + Модернизация
	uint8_t manth;			// [108] Месяц
	uint16_t number;		// [109] Номер блока
	uint8_t parametersSummH;// [10B] Сумма байт [105]-[109], старший байт
	uint8_t parametersSummL;// [10C] младший байт
	uint8_t fuseLow;		// [10D]
	uint8_t fuseHigh;		// [10E]
	uint8_t fuseExt;		// [10F]
};

//uint16_t ID  __attribute__ ((section (".id"))) = 0xab;

IdentifyDoc id __attribute__ ((section (".id"))) =
{ 0, 0, 1, PROGRAM_VERSION, 25, 0x21, 1, 1, (PROGRAM_VERSION + 25 + 0x21 + 1 + 1), 0, 0xDF, 0xD9, 0xFD };


// ----------------------------------ATTENTION--------------------------------------
// .id Section must be placed by linker to address 0x0100.
// We modify avr/lib/ldscripts/avr5.x to allow place .id between .vectors and .init0
// It normally use --script-file option to linker instead it, but it work bad
// New file store at name "ld-script.x"
// ---------------------------------------------------------------------------------

#endif /* ID_H_ */
