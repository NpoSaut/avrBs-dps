/*
 * ProgSpi.h
 *
 *	Программирование соседней микросхемы через передатчик SPI
 *	*********************************************************
 *
 *	- Модуль использует SPI передатчик в режиме Мастер, который должен быть подключен к соседней микросхеме посредствам MOSI, MISO, SCK
 *	- Для переведения микросхемы в режим программирования необходимо также подключить RESET к пину PIN_RST порта PORT_RST
 *	- Init () подготавливает SPI передатчик
 *	- RebootInProg () переводит микросхему в режим программирования
 *
 */

#ifndef PROGSPI_H_
#define PROGSPI_H_

// -------- ProgSpi defines --------
// Минимальные временны'е задержки
#define T_WD_FUSE	4.5		// запись FUSE
#define T_WD_FLASH	4.5		// запись страницы Flash
#define T_WD_EEPROM	9.0		// запись страницы EEPROM
#define T_WD_ERASE	9.0		// отчистка Flash и EEPROM
#define T_ST_PROG	20.0	// от включения до инструкции программирования
// ---------------------------------

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 12000000UL 	// 12 MHz

#include "interrupt-dynamic.h"
#include "SautCom.h"

INTERRUPT_DELEGATE_DECLARE(SPI_STC);


template <class PortSpi, uint8_t SS, uint8_t SCK, uint8_t MOSI, uint8_t MISO, class PortRST, uint8_t RST> class ProgSpi
{
public:
	ProgSpi ()
	{
		enableInt = true;

		SPI_STC_handler.Bind (this, &ProgSpi::spiTransferComplete);			// Устанавливаем свой обработчик прерываний

		PortSpi::dir() &= (uint8_t) ~(1 << MISO);
		PortSpi::dir() |= (1 << MOSI) | (1 << SCK) | (1 << SS);				// SS пин конфигурируется на выход, чтобы при 0 на нём (вдруг - это вдруг случилось) не сбрасывался мастер-режим
		PortSpi::port()|= (1 << MISO);
		//SPCR = (1 << SPE) | (1 << SPIE) | (1 << MSTR) | (1 << SPR0); 		// Включение, прерывание, мастер, f/16
		SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	}

	inline void rebootInWork ()
	{
		PortRST::port() |= (1 << RST);
		PortRST::dir() &= (uint8_t) ~(1 << RST);
	}

	inline void rebootInProg ()
	{
		// Перевод соседней микросхемы  в режим программирования
		PortRST::dir() |= (1 << RST);
		PortRST::port() &= (uint8_t) ~(1 << RST);
		_delay_loop_1 (2);
		PortRST::port() |= (1 << RST);
		_delay_loop_1 (2);
		PortRST::port() &= (uint8_t) ~(1 << RST);
		_delay_ms (T_ST_PROG);
	}

	inline virtual void completeInt () {}						// Событие возникает после передачи инструкции с индексом Im

	inline uint8_t instTest (uint8_t a)
	{
		SPCR &= (uint8_t) ~(1 << SPIE);
		SPDR = a;
		while(!(SPSR & (1<<SPIF)));
		return SPDR;
	}

	inline void instructionIm (uint32_t inst)					// Только передает инструкцию на выполнение и немедленно выходит. Возникает прерывание по завершению.
	{
		ProgSpi::inst = inst;
		spiTransferComplete();
	}

	inline bool isComplete () { return step == 0; }
	inline void waitComplete ()	{ while (!isComplete ()); }
	inline uint32_t answerFlush () { waitComplete (); return ans; }

	inline uint32_t instruction (uint32_t Inst)					// Выходит из функции только после выполнения инструкции. Прерывания не возникает.
	{
		ans = 0;
		for (register int8_t i = 24; i >= 0; i -= 8)						// Смещение для поочерёдного выделения байтов из двойного слова
		{
			SPDR = uint8_t (Inst >> i);
			while(!(SPSR & (1<<SPIF)));										// Дожидаемся конца передачи
			ans |= uint32_t (SPDR) << i;
		}
		return ans;

//		enableInt = false;
//		instructionIm (Inst);
//		waitComplete ();
//		enableInt = true;
//		return answerFlush ();
	}

	inline bool 	enableProg () {return uint8_t (instruction (0xAC530000) >> 8) == 0x53;}
	inline void 	erase () {instruction (0xAC800000); _delay_ms (T_WD_ERASE);}
	inline uint8_t 	progmemRead (uint32_t adr) {return instruction( ((0x20 | ((adr & 0b1) << 3)) << 24) | ((uint32_t (adr >> 1) <<  8) & 0xff00) );}
	inline void		progmemLoadToPage (uint8_t adrInPage, uint8_t data) {instruction( uint32_t (0x40 | ((adrInPage & 0b1) << 3)) << 24 | uint32_t (adrInPage >> 1) << 8 | data );}
	inline void		progmemWritePage (uint32_t adr) {instruction( uint32_t (0x4C) << 24 | uint32_t ((adr >> 8) & 0x1FF) << 15 ); _delay_ms (T_WD_FLASH);}
	inline uint8_t	eepromRead (uint16_t adr) {return instruction( uint32_t (0xA0) << 24 | uint32_t (adr & 0xfff) << 8 );}
	inline void		eepromLoadToPage (uint8_t adrInPage, uint8_t data) {instruction( uint32_t (0xC1) << 24 | uint32_t (adrInPage & 0x7) << 8 | data );}
	inline void		eepromWritePage (uint16_t adr) {instruction( uint32_t (0xC2) << 24 | uint32_t (adr & 0x1f) << 11 ); _delay_ms (T_WD_EEPROM);}
	inline void		fuseWriteLow (uint8_t bits) {instruction( uint32_t (0xACA0) << 16 | bits ); _delay_ms (T_WD_FUSE);}
	inline void		fuseWriteHigh (uint8_t bits) {instruction( uint32_t (0xACA8) << 16 | bits ); _delay_ms (T_WD_FUSE);}
	inline void		fuseWriteExt (uint8_t bits) {instruction( uint32_t (0xACA4) << 16 | bits ); _delay_ms (T_WD_FUSE);}
	inline uint8_t	fuseReadLow () {return instruction( uint32_t (0x5000) << 16 );}
	inline uint8_t	fuseReadHigh () {return instruction( uint32_t (0x5808) << 16 );}
	inline uint8_t	fuseReadExt () {return instruction( uint32_t (0x5008) << 16 );}

	inline void	 	progmemReadIm (uint32_t adr) {instructionIm( (0x20 | ((adr & 0b1) << 3)) << 24 | uint16_t (adr >> 1) <<  8 );}
	inline void		progmemLoadToPageIm (uint8_t AdrInPage, uint8_t Data) {instructionIm( uint32_t (0x40 | ((AdrInPage & 0b1) << 3)) << 24 | uint32_t (AdrInPage >> 1) << 8 | Data );}
	inline void		progmemWritePageIm (uint16_t adr) {instructionIm( uint32_t (0x4C) << 24 | uint32_t (adr & 0x1FF) << 15 ); _delay_ms (T_WD_FLASH);}
	inline void		eepromReadIm (uint16_t adr) {return instructionIm( uint32_t (0xA0) << 24 | uint32_t (adr & 0xfff) << 8 );}
	inline void		eepromLoadToPageIm (uint8_t adrInPage, uint8_t data) {instructionIm( uint32_t (0xC1) << 24 | uint32_t (adrInPage & 0x7) << 8 | data );}
	inline void		eepromWritePageIm (uint16_t adr) {instructionIm( uint32_t (0xC2) << 24 | uint32_t (adr & 0x1f) << 11 ); _delay_ms (T_WD_EEPROM);}

private:
	// Вся передача здесь
	inline void spiTransferComplete()
	{
		if (step != 0)
			ans |= uint32_t (SPDR) << (4-step)*8;							// Читаем полученный байт
		else
			ans = 0;

		if (step < 4)														// Если еще не вся инструкция передана,
			SPDR = uint8_t (inst >> (3-step++)*8);							// передаем следующий байт
		else
		{
			step = 0;
			if (enableInt)
			{
				sei ();
				completeInt ();
			}
		}
	}


	volatile uint32_t inst, ans;
	volatile uint8_t step;
	volatile bool enableInt;
};



#endif /* PROGSPI_H_ */
