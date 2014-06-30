/*
 * ProgSpiBuffered.h
 *
 *	Программирование соседней микросхемы через передатчик SPI (c использование буфера)
 *	**********************************************************************************
 *
 *
 *
 *
 */

#ifndef PROGSPIBUFFERED_H_
#define PROGSPIBUFFERED_H_


#include "universal.h"
#include "ProgSpi.h"
#include "buffer-queue.h"



template <class PortSpi, uint8_t SCK, uint8_t MOSI, uint8_t MISO, class PortRST, uint8_t RST>
class ProgSpiBuffered : public ProgSpi<PortSpi, SCK, MOSI, MISO, PortRST, RST>, private Queqe<uint8_t, 24>
{
public:
	inline ProgSpiBuffered ();

	enum TypeMem {PROGMEM, EEPROM};
	inline void setMemType (TypeMem type){	ProgSpiBuffered::type.mem = type; }

	inline void setPosition (uint32_t pos)
	{
		position.stream = pos;
		position.program = pos;

		if (type.op == WRITE)
			need.pop = true;
		else
			need.push = true;
	}
	inline uint32_t getPosition ()
	{
		return position.stream;
	}

	inline ProgSpiBuffered& operator<< (Fflush flush)
	{
		disablePop ();
		waitFree ();
	}
	inline ProgSpiBuffered& operator>> (FstopRead stopRead)
	{
		disablePush ();
	}

	inline ProgSpiBuffered& operator<< (uint8_t  data)
	{
		push (data);
		position.stream ++;
		return this;
	}
	inline ProgSpiBuffered& operator<< (uint16_t data)
	{
		push (data>>8);
		push (data&0x00ff);
		position.stream += 2;
		return this;
	}
	inline ProtSpiBuffered& operator>> (uint8_t& data)
	{
		pop (data);
		position.stream ++;
		return this;
	}
	inline ProtSpiBuffered& operator>> (uint16_t& data)
	{
		register uint8_t hdata;
		pop (hdata);
		pop (data);
		data |= uint16_t (hdata << 8);
		position.stream ++;
		return this;
	}

	inline void completeInt ()											// Наследуется от ProgSpi. Возникает при окончании передачи инструкции.
	{
		switch (type.mem)
		{
		case PROGMEM:
			switch (type.op)
			{
				case WRITE:
					register uint8_t data;
					cli ();
					if (popIm (data))
					{
						if (uint8_t (position.program) == 0x0) 				//  Если  закончилась страница
							ProgmemWritePage (position.program >> 8);
						ProgmemLoadToPageIm (position.program++, data);
					}
					else
					{
						waitInterrupt = true;
					}
					break;
				case READ:
					push (answerFlush ());
					++ position.program;
					progmemReadIm (position.program);
					break;
			}
			break;
		case EEPROM:
			switch (type.op)
			{
				case WRITE:
					break;
				case READ:
					break;
			}
			break;
		}
	}

private:
	struct { uint32_t stream, program; } position;
	enum TypeOp {READ, WRITE};
	struct
	{
		MemType mem : 4;
		OpType	op	: 4;
	} type;
	bool waitInterrupt;
};

inline ProgSpiBuffered::ProgSpiBuffered ()
	: ProgSpi ()
{
}


#endif /* PROGSPIBUFFERED_H_ */
