/*
 * ProgSpi.h
 *
 *	Программирование соседней микросхемы через передатчик SPI
 *	*********************************************************
 *
 *	Требования к подключению:
 *	- Модуль использует SPI передатчик в режиме Мастер, который должен быть подключен к программируемой микросхеме посредствам MOSI, MISO, SCK.
 *	  ( SS для программирования не используется, но его необходимо указывать, чтобы сконфигурировать его на выход (и использовать как обычный пин),
 *	  тем самым избежать ситуации, когда Мастер переключается в Раба по падению сигнала на  SS )
 *	- Для переведения микросхемы в режим программирования необходимо также подключить её RESET к пину PIN_RST порта PORT_RST.
 *
 *	Работа модуля:
 *	- Конструктор конфигурирует направления пинов, передатчик SPI и с трёх раз пытается перевести микросхему в режим программирования
 *	  Если это удаётся, то функция good() возвращает true, в противном случае --- false()
 *	- Чтение/запись происходит в стиле работы с патоками (см. примеры). Поддерживается ввод/вывод байт и слов.
 *  - Задание типа программируемой памяти производится посредством оператора (). Например: mk(flash)
 *	  Для передачи в качестве параметра доступны две константы flash и eeprom, определённые в пространестве имён ProgSpi
 *	- Задание позиции курсора производится посредствам оператора []. Напрмер: mk[0x10]
 *	- Текущую позицию курсора возврщает функция position ()
 *	- Запись данных с помощью << ещё не гарантирует, что данные уже запсаны. Это гарантирует вызвов функции flush()
 *	  Функция flush() автоматически вызвается при изменениии типа программируемой памяти, при манипуляциях с курсором и при вызове деструктора
 *	- Работами с битами fuse осуществляется с помощью фунций, начинающихся на fuse...
 *	- Деструктор обеспечивает завершение всех операций чтения/записи и перезагружает микросхему в режим нормальной работы
 *
 */
// -- Пример использования --
//
//	using namespace ProgSpi;
//
//
//		ProgSpiSimple<PortB,	// порт SPI
//						0,		// SS
//						1,		// SCK
//						2,		// MOSI
//						3,		// MISO
//						PortB,	// порт управления RESET соседней мк
//						4		// номер пина на этом порту
//						> neighbour; // в этом месте вызвается конструктор
//
//		if ( neighbour.good() )		// если переход в режим программирования совершён успешно
//		{
//			neighbour[0x100](flash) << 0xFF;	// записать во флеш память по адресу 0x100 байт 0xff
//			neighbour(flash)[0x100] << 0xFF; 	// аналогично
//			neighbour << 0xAA;					// 0xAA будет записано по адресу 0x101
//			neighbour << 0xBB << 0xCC;			// 0xBB будет записано по адресу 0x102, 0xCC будет записано по адресу 0x103
//			neighbour << 0xDDEE;				// поддерживается запись словами
//												// в этом месте не гарантируется, что предыдущие значения были записаны в память
//												// при выключении питания, отсоединении микросхемы и т.д. данные могу быть утеряны
//			neighbour.flush ();					// после вызова этой функции
//												// здесь гарантированно, что все данные полностью записаны в нужное место
//			neighbour[0x200];					// возможно просто изменить позицию курсора, не выполняя операций чтения/записи
//			if (neighbour.position () == 0x200)	// унать текущую позицию
//				neighbour ++;					// сдвинуть курсор на единичку вправо
//			-- neighbour;						// или влево
//			uint8_t temp;
//			neighbour >> temp;					// операции чтения выполняются аналогично операциям записи
//			neighbour(eeprom);					// возможно только изменить тип памяти, курсор останется на той же позиции, но на другой памяти
//			// такой код:
//			neighbour << 0x00;
//			neighbour >> temp;					// ... ошибочен
//			// првильно так:
//			neighbour << 0x00;
//			neighbour.flush ();
//			neighbour >> temp;
//		}										// в этом месте заканчивается область видимости переменной neighbour,
//												// т.е. вызывается деструктор, завершающий все операции и переводящий микросхему в нормальный режим работы
//


#ifndef PROGSPI_H_
#define PROGSPI_H_

// -------- ProgSpi defines --------
// Минимальные временны'е задержки
#define T_WD_FUSE	4.5		// запись FUSE
#define T_WD_FLASH	4.5		// запись страницы Flash
#define T_WD_EEPROM	9.0		// запись страницы EEPROM
#define T_WD_ERASE	9.0		// отчистка Flash и EEPROM
#define T_ST_PROG	20.0	// от включения до инструкции программирования
// Размыры страниц памяти
#define FLASH_PAGE_SIZE	 256
#define EEPROM_PAGE_SIZE 8
// ---------------------------------

#include <cpp/universal.h>
//#include <cpp/interrupt.h>
#include <cpp/io.h>
//#define F_CPU 12000000UL 	// 12 MHz
#include <util/delay.h>


namespace ProgSpi
{
enum MemType : uint8_t
{
	flash,
	eeprom
};

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
class ProgSpiSimple
{
public:
	ProgSpiSimple ();

	bool rebootInProg ();			// Перевод программируемой микросхемы  в режим программирования
	void rebootInWork ();			// Перевод программируемой микросхемы  в режим нормальной работы
	bool isInProgState () const {return progMode;}

	void erase ();					// Отчистиь flash и eeprom

	template <MemType type>
	uint8_t read ();
	template <MemType type>
	Complex<uint16_t> readWord ();

	template <MemType type>
	void write (uint8_t data);
	template <MemType type>
	void write (uint16_t data);

	template<MemType type>
	void flush ();					// Дождаться завершения операций

	void fuseWriteLow (uint8_t bits);
	void fuseWriteHigh (uint8_t bits);
	void fuseWriteExt (uint8_t bits);
	uint8_t fuseReadLow ();
	uint8_t fuseReadHigh ();
	uint8_t fuseReadExt ();

	uint32_t position;

private:
	void pinConfig ();				// Настройка пинов
	void pinRelease ();				// Освобождение управления пинами
	void spiConfig ();				// Конфигурация SPI передатчика
	void spiRelease ();				// Деконфигурация передатчика
	bool enableProg ();				// Послать инструкцию перехода в режим программирования
	uint8_t instruction (uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4); // Выполняет инструкцию, возвращает ответ

	bool progMode;
};


template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::ProgSpiSimple()
	: position (0)
{
	rebootInWork();
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
bool ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::rebootInProg()
{
	pinConfig ();
	spiConfig ();

	for (uint8_t i = 0; i < 3; ++i)							// Заводим с трёх попыток
	{
		(reg.*resetPort).pin<resetPin>().out ();
		(reg.*resetPort).pin<resetPin>() = 0;
		_delay_loop_1 (2);
		(reg.*resetPort).pin<resetPin>() = 1;
		_delay_loop_1 (2);
		(reg.*resetPort).pin<resetPin>() = 0;
		_delay_ms (T_ST_PROG);

		if ( enableProg() )
		{
			progMode = true;
			return true;
		}
	}
	return false;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::rebootInWork()
{
	spiRelease();
	pinRelease();
	progMode = false;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::erase()
{
	instruction (0xAC,0x80,0,0);
	_delay_ms (T_WD_ERASE);
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
template <	MemType type	>
uint8_t ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::read ()
{
	static_assert (type == flash || type == eeprom, "Unknown memory type");
	uint8_t data;
	if (type == flash)
		data = instruction ( 0x20 | ((position & 0b1) << 3), position >> 9, position >> 1, 0 );
	if (type == eeprom)
		data = instruction ( 0xA0, (position >> 8) & 0x7, position, 0 );
	position ++;
	return data;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
template <	MemType type	>
Complex<uint16_t> ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::readWord ()
{
	return Complex<uint16_t>{ read<type>(), read<type>() };
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
template <	MemType type	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::write (uint8_t data)
{
	static_assert (type == flash || type == eeprom, "Unknown memory type");
	if (type == flash)
	{
		if (position % FLASH_PAGE_SIZE == 0)							// Если началась новая страница
			flush<flash> ();
		instruction ( 0x40 | ((position & 0b1) << 3), 0, position >> 1, data );
	}
	if (type == eeprom)
	{
		if (position % EEPROM_PAGE_SIZE == 0)							// Если началась новая страница
			flush<eeprom> ();
		instruction ( 0xC1, 0, position & 0x7, data );
	}
	position ++;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
template <	MemType type	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::write (uint16_t data)
{
	write<type> ((uint8_t)data);
	write<type> ((uint8_t)(data>>8));
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
template <	MemType type	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::flush ()
{
	if (type == flash)
	{
		instruction ( 0x4C, (position-1) >> 9, ((position-1) >> 1) & 0x80, 0 );
		_delay_ms (T_WD_FLASH);
	}
	if (type == eeprom)
	{
		instruction( 0xC2, ((position-1) >> 8) & 0xf, (position-1) & 0xf8, 0 );
		_delay_ms (T_WD_EEPROM);
	}
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseWriteLow (uint8_t bits)
{
	instruction ( 0xAC, 0xA0, 0, bits );
	_delay_ms (T_WD_FUSE);
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseWriteHigh (uint8_t bits)
{
	instruction ( 0xAC, 0xA8, 0, bits );
	_delay_ms (T_WD_FUSE);
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseWriteExt (uint8_t bits)
{
	instruction ( 0xAC, 0xA4, 0, bits );
	_delay_ms (T_WD_FUSE);
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
uint8_t ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseReadLow ()
{
	return instruction ( 0x50, 0x00, 0, 0 );
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
uint8_t ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseReadHigh ()
{
	return instruction ( 0x58, 0x08, 0, 0 );
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
uint8_t ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::fuseReadExt ()
{
	return instruction ( 0x50, 0x08, 0, 0 );
}

// private:

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::pinConfig ()
{
	(reg.*port).pin<misoPin>().inPulled ();
	(reg.*port).pin<mosiPin>().out ();
	(reg.*port).pin<sckPin>().out ();
	(reg.*port).pin<ssPin>().out ();									// SS пин конфигурируется на выход, чтобы при 0 на нём (вдруг - это вдруг случилось) не сбрасывался мастер-режим
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::pinRelease ()
{
	(reg.*resetPort).pin<resetPin>().inPulled ();
	(reg.*port).pin<misoPin>().in ();
	(reg.*port).pin<mosiPin>().in ();
	(reg.*port).pin<sckPin>().in ();
	(reg.*port).pin<ssPin>().in ();
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::spiConfig ()
{
	Bitfield<SpiStatusControl> ctr (0);
	ctr.enable = true;
	ctr.master = true;
	ctr.prescale = SpiStatusControl::Prescale::F4;
	(reg.*control) = ctr;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
void ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::spiRelease ()
{
	(reg.*control).enable = false;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
bool ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::enableProg ()
{
	// Реализация повторяет вызов instruction (). Однако "ответ" приходит не в последнем байте, а в предпоследнем.
	(reg.*dataReg) = 0xAC;
	while(! (reg.*control).transferComplete );
	(reg.*dataReg) = 0x53;
	while(! (reg.*control).transferComplete );
	(reg.*dataReg) = 0;
	while(! (reg.*control).transferComplete );
	register bool result = ( (reg.*dataReg) == 0x53 );
	(reg.*dataReg) = 0;
	while(! (reg.*control).transferComplete );
	return result;
}

template <	volatile Bitfield<SpiStatusControl> Register::* control, volatile uint8_t Register::* dataReg,
			Port Register::* port, uint8_t ssPin, uint8_t sckPin, uint8_t mosiPin, uint8_t misoPin,
			Port Register::* resetPort, uint8_t resetPin	>
uint8_t ProgSpiSimple<control, dataReg, port, ssPin, sckPin, mosiPin, misoPin, resetPort, resetPin>::instruction (uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
	(reg.*dataReg) = b1;
	while(! (reg.*control).transferComplete );							// Дожидаемся конца передачи
	(reg.*dataReg) = b2;
	while(! (reg.*control).transferComplete );
	(reg.*dataReg) = b3;
	while(! (reg.*control).transferComplete );;
	(reg.*dataReg) = b4;
	while(! (reg.*control).transferComplete );
	return (reg.*dataReg);
}

} // namespace close




//namespace ProgSpi
//{
//enum MemType : uint8_t
//{
//	flash,
//	eeprom
//};
//
//
//template <class PortSpi, uint8_t SS, uint8_t SCK, uint8_t MOSI, uint8_t MISO, class PortRST, uint8_t RST>
//class ProgSpiSimple
//{
//public:
//	ProgSpiSimple ()
//	{
//		pinConfig ();
//		spiConfig ();
//
//
//		_good = false;
//		for (register uint8_t i = 0; i < 3; ++ i)							// Заводим с трёх попыток
//		{
//			rebootInProg ();
//			if ( enableProg() )
//			{
//				_good = true;
//				break;
//			}
//		}
//	}
//	~ProgSpiSimple ()
//	{
//		flush ();
//		rebootInWork ();
//	}
//
//	inline bool good () { return _good; }									// Удачен ли переход в режим программирования (осуществляется в конструкторе)
//
//	inline ProgSpiSimple& operator[] (uint32_t position)					// Установить позицию "курсора". Это адрес в памяти в который будет произведена запись следующего байта при операции записи или адрес байта, который будет получен при операции чтения.
//	{
//		if ( (type == flash  && (pos/FLASH_PAGE_SIZE != position/FLASH_PAGE_SIZE)) ||
//			(type == eeprom && (pos/EEPROM_PAGE_SIZE != position/EEPROM_PAGE_SIZE)) )
//			flush ();
//		pos = position;
//		return *this;
//	}
//	inline uint32_t position()												// Получить текущую позицию "курсора".
//	{
//		return pos;
//	}
//
//	ProgSpiSimple& operator() (MemType mt)									// Установить тип памяти
//	{
//		if (type != mt)
//		{
//			flush ();
//			type = mt;
//		}
//		return *this;
//	}
//
//	inline void erase ()													// Отчистиь flash и eeprom
//	{
//		instruction (0xAC,0x80,0,0);
//		_delay_ms (T_WD_ERASE);
//	}
//
//	uint8_t readFlash ()
//	{
//		uint8_t data = instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 );
//		pos ++;
//		return data;
//	}
//
//	uint8_t readEeprom()
//	{
//		uint8_t data = instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 );;
//		pos ++;
//		return data;
//	}
//
//	inline ProgSpiSimple& operator>> (uint8_t& data)
//	{
//		switch (type)
//		{
//			case flash:
//				data = instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 );
//				break;
//			case eeprom:
//				data = instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 );
//				break;
//		}
//		pos ++;
//		return *this;
//	}
//	inline ProgSpiSimple& operator>> (uint16_t& data)
//	{
//		switch (type)
//		{
//			case flash:
//				data = instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 ) << 8;
//				pos ++;
//				data |= instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 );
//				break;
//			case eeprom:
//				data = instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 ) << 8;
//				pos ++;
//				data |= instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 );
//				break;
//		}
//		pos ++;
//		return *this;
//	}
//	inline ProgSpiSimple& operator<< (const uint8_t& data)
//	{
//		switch (type)
//		{
//			case flash:
//				if (pos % FLASH_PAGE_SIZE == 0)								// Если закончилась страница
//					flush ();
//				instruction ( 0x40 | ((pos & 0b1) << 3), 0, pos >> 1, data );
//				break;
//			case eeprom:
//				if (pos % EEPROM_PAGE_SIZE == 0)							// Если закончилась страница
//					flush ();
//				instruction ( 0xC1, 0, pos & 0x7, data );
//				break;
//		}
//		pos ++;
//		return *this;
//	}
//	inline ProgSpiSimple& operator<< (const uint16_t& data)
//	{
//		return (*this) << (uint8_t (data / 256)) << (uint8_t) (data);
//	}
//
//
//	// volatile
//	inline ProgSpiSimple& operator>> (volatile uint8_t& data)
//		{
//			switch (type)
//			{
//				case flash:
//					data = instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 );
//					break;
//				case eeprom:
//					data = instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 );
//					break;
//			}
//			pos ++;
//			return *this;
//		}
//	inline ProgSpiSimple& operator>> (volatile uint16_t& data)
//	{
//		switch (type)
//		{
//			case flash:
//				data = instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 ) << 8;
//				pos ++;
//				data |= instruction ( 0x20 | ((pos & 0b1) << 3), pos >> 9, pos >> 1, 0 );
//				break;
//			case eeprom:
//				data = data = instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 ) << 8;
//				pos ++;
//				data |= instruction ( 0xA0, (pos >> 8) & 0x7, pos, 0 );
//				break;
//		}
//		pos ++;
//		return *this;
//	}
//	inline ProgSpiSimple& operator<< (const volatile uint8_t& data)
//	{
//		switch (type)
//		{
//			case flash:
//				if (pos % FLASH_PAGE_SIZE == 0)								// Если началась новая страница
//					flush ();
//				instruction ( 0x40 | ((pos & 0b1) << 3), 0, pos >> 1, data );
//				break;
//			case eeprom:
//				if (pos % EEPROM_PAGE_SIZE == 0)							// Если началась новая страница
//					flush ();
//				instruction ( 0xC1, 0, pos & 0x7, data );
//				break;
//		}
//		pos ++;
//		return *this;
//	}
//	inline ProgSpiSimple& operator<< (const volatile uint16_t& data)
//	{
//		volatile uint8_t dataB = data / 256;
//		operator<< ( dataB );
//		dataB = data;
//		operator<< ( dataB );
//		return *this;
//	}
//	inline void flush ()													// Дождаться завершения операций
//		{
//			switch (type)
//			{
//				case flash:
//					instruction ( 0x4C, (pos-1) >> 9, ((pos-1) >> 1) & 0x80, 0 );
//					_delay_ms (T_WD_FLASH);
//					break;
//				case eeprom:
//					instruction( 0xC2, ((pos-1) >> 8) & 0xf, (pos-1) & 0xf8, 0 );
//					_delay_ms (T_WD_EEPROM);
//					break;
//			}
//		}
//
//	inline void fuseWriteLow (uint8_t bits)
//	{
//		instruction ( 0xAC, 0xA0, 0, bits );
//		_delay_ms (T_WD_FUSE);
//	}
//	inline void fuseWriteHigh (uint8_t bits)
//	{
//		instruction ( 0xAC, 0xA8, 0, bits );
//		_delay_ms (T_WD_FUSE);
//	}
//	inline void fuseWriteExt (uint8_t bits)
//	{
//		instruction ( 0xAC, 0xA4, 0, bits );
//		_delay_ms (T_WD_FUSE);
//	}
//	inline uint8_t fuseReadLow ()
//	{
//		return instruction ( 0x50, 0x00, 0, 0 );
//	}
//	inline uint8_t fuseReadHigh ()
//	{
//		return instruction ( 0x58, 0x08, 0, 0 );
//	}
//	inline uint8_t fuseReadExt ()
//	{
//		return instruction ( 0x50, 0x08, 0, 0 );
//	}
//
////private:
//	bool _good;
//	uint32_t pos;
//	MemType type;
//
//	inline void pinConfig ()												// Настройка пинов
//	{
//		PortSpi::dir() &= (uint8_t) ~(1 << MISO);
//		PortSpi::dir() |= (1 << MOSI) | (1 << SCK) | (1 << SS);				// SS пин конфигурируется на выход, чтобы при 0 на нём (вдруг - это вдруг случилось) не сбрасывался мастер-режим
//		PortSpi::port()|= (1 << MISO);
//	}
//	inline void spiConfig ()												// Конфигурация SPI передатчика
//	{
//		SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); 						// Включение, прерывание, мастер, f/16
//	}
//	inline void rebootInProg ()												// Перевод соседней микросхемы  в режим программирования
//	{
//		PortRST::dir() |= (1 << RST);
//		PortRST::port() &= (uint8_t) ~(1 << RST);
//		_delay_loop_1 (2);
//		PortRST::port() |= (1 << RST);
//		_delay_loop_1 (2);
//		PortRST::port() &= (uint8_t) ~(1 << RST);
//		_delay_ms (T_ST_PROG);
//	}
//	inline void rebootInWork ()												// Перевод соседней микросхемы  в режим нормальной работы
//	{
//		PortRST::port() |= (1 << RST);
//		PortRST::dir() &= (uint8_t) ~(1 << RST);
//	}
//	inline bool enableProg ()												// Послать инструкцию перехода в режим программирования
//	{
//		// Реализация повторяет вызов instruction (). Однако "ответ" приходит не в последнем байте, а в предпоследнем.
//		SPDR = 0xAC;
//		while(!(SPSR & (1<<SPIF)));
//		SPDR = 0x53;
//		while(!(SPSR & (1<<SPIF)));
//		SPDR = 0;
//		while(!(SPSR & (1<<SPIF)));
//		register bool result = (SPDR == 0x53);
//		SPDR = 0;
//		while(!(SPSR & (1<<SPIF)));
//		return result;
//	}
//	inline uint8_t instruction (uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) __attribute__ ((noinline))	// Выполняет инструкцию inst, возвращает ответ
//	{
//		SPDR = b1;
//		while(!(SPSR & (1<<SPIF)));										// Дожидаемся конца передачи
//		SPDR = b2;
//		while(!(SPSR & (1<<SPIF)));
//		SPDR = b3;
//		while(!(SPSR & (1<<SPIF)));
//		SPDR = b4;
//		while(!(SPSR & (1<<SPIF)));
//		return SPDR;
//	}
//};
//
//} // namespace ProgSpi
//

//template <class PortSpi, uint8_t SS, uint8_t SCK, uint8_t MOSI, uint8_t MISO, class PortRST, uint8_t RST> class ProgSpi
//{
//public:
//	ProgSpi ()
//	{
//		enableInt = true;
//
//		SPI_STC_handler.Bind (this, &ProgSpi::spiTransferComplete);			// Устанавливаем свой обработчик прерываний
//
//		PortSpi::dir() &= (uint8_t) ~(1 << MISO);
//		PortSpi::dir() |= (1 << MOSI) | (1 << SCK) | (1 << SS);				// SS пин конфигурируется на выход, чтобы при 0 на нём (вдруг - это вдруг случилось) не сбрасывался мастер-режим
//		PortSpi::port()|= (1 << MISO);
//		SPCR = (1 << SPE) | (1 << SPIE) | (1 << MSTR) | (1 << SPR0); 		// Включение, прерывание, мастер, f/16
//	}
//
//	inline void rebootInWork ()
//	{
//		PortRST::port() |= (1 << RST);
//		PortRST::dir() &= (uint8_t) ~(1 << RST);
//	}
//
//	inline void rebootInProg ()
//	{
//		// Перевод соседней микросхемы  в режим программирования
//		PortRST::dir() |= (1 << RST);
//		PortRST::port() &= (uint8_t) ~(1 << RST);
//		_delay_loop_1 (2);
//		PortRST::port() |= (1 << RST);
//		_delay_loop_1 (2);
//		PortRST::port() &= (uint8_t) ~(1 << RST);
//		_delay_ms (T_ST_PROG);
//	}
//
//	inline virtual void completeInt () {}						// Событие возникает после передачи инструкции с индексом Im
//
//	inline uint8_t instTest (uint8_t a)
//	{
//		//SPCR &= (uint8_t) ~(1 << SPIE);
//		SPDR = a;
//		while(!(SPSR & (1<<SPIF)));
//		return SPDR;
//	}
//
//	inline void instructionIm (uint32_t inst)					// Только передает инструкцию на выполнение и немедленно выходит. Возникает прерывание по завершению.
//	{
//		ProgSpi::inst = inst;
//		spiTransferComplete();
//	}
//
//	inline bool isComplete () { return step == 0; }
//	inline void waitComplete ()	{ while (!isComplete ()); }
//	inline uint32_t answerFlush () { waitComplete (); return ans; }
//
//	inline uint32_t instruction (uint32_t Inst)					// Выходит из функции только после выполнения инструкции. Прерывания не возникает.
//	{
//		enableInt = false;
//		instructionIm (Inst);
//		waitComplete ();
//		enableInt = true;
//		return answerFlush ();
//	}
//
//	inline bool 	enableProg () {return uint8_t (instruction (0xAC530000) >> 8) == 0x53;}
//	inline void 	erase () {instruction (0xAC800000); _delay_ms (T_WD_ERASE);}
//	inline uint8_t 	progmemRead (uint32_t adr) {return instruction( (0x20 | ((adr & 0b1) << 3)) << 24 | uint16_t (adr >> 1) <<  8 );}
//	inline void		progmemLoadToPage (uint8_t adrInPage, uint8_t data) {instruction( uint32_t (0x40 | ((adrInPage & 0b1) << 3)) << 24 | uint32_t (adrInPage >> 1) << 8 | data );}
//	inline void		progmemWritePage (uint16_t adr) {instruction( uint32_t (0x4C) << 24 | uint32_t (adr & 0x1FF) << 15 ); _delay_ms (T_WD_FLASH);}
//	inline uint8_t	eepromRead (uint16_t adr) {return instruction( uint32_t (0xA0) << 24 | uint32_t (adr & 0xfff) << 8 );}
//	inline void		eepromLoadToPage (uint8_t adrInPage, uint8_t data) {instruction( uint32_t (0xC1) << 24 | uint32_t (adrInPage & 0x7) << 8 | data );}
//	inline void		eepromWritePage (uint16_t adr) {instruction( uint32_t (0xC2) << 24 | uint32_t (adr & 0x1f) << 11 ); _delay_ms (T_WD_EEPROM);}
//	inline uint8_t	fuseWriteLow (uint8_t bits) {return instruction( uint32_t (0xACA0) << 16 | bits ); _delay_ms (T_WD_FUSE);}
//	inline uint8_t	fuseWriteHigh (uint8_t bits) {return instruction( uint32_t (0xACA8) << 16 | bits ); _delay_ms (T_WD_FUSE);}
//	inline uint8_t	fuseWriteExt (uint8_t bits) {return instruction( uint32_t (0xACA4) << 16 | bits ); _delay_ms (T_WD_FUSE);}
//	inline uint8_t	fuseReadLow () {return instruction( uint32_t (0x5000) << 16 );}
//	inline uint8_t	fuseReadHigh () {return instruction( uint32_t (0x5808) << 16 );}
//	inline uint8_t	fuseReadExt () {return instruction( uint32_t (0x5008) << 16 );}
//
//	inline void	 	progmemReadIm (uint32_t adr) {instructionIm( (0x20 | ((adr & 0b1) << 3)) << 24 | uint16_t (adr >> 1) <<  8 );}
//	inline void		progmemLoadToPageIm (uint8_t AdrInPage, uint8_t Data) {instructionIm( uint32_t (0x40 | ((AdrInPage & 0b1) << 3)) << 24 | uint32_t (AdrInPage >> 1) << 8 | Data );}
//	inline void		progmemWritePageIm (uint16_t adr) {instructionIm( uint32_t (0x4C) << 24 | uint32_t (adr & 0x1FF) << 15 ); _delay_ms (T_WD_FLASH);}
//	inline void		eepromReadIm (uint16_t adr) {return instructionIm( uint32_t (0xA0) << 24 | uint32_t (adr & 0xfff) << 8 );}
//	inline void		eepromLoadToPageIm (uint8_t adrInPage, uint8_t data) {instructionIm( uint32_t (0xC1) << 24 | uint32_t (adrInPage & 0x7) << 8 | data );}
//	inline void		eepromWritePageIm (uint16_t adr) {instructionIm( uint32_t (0xC2) << 24 | uint32_t (adr & 0x1f) << 11 ); _delay_ms (T_WD_EEPROM);}
//
//private:
//	// Вся передача здесь
//	inline void spiTransferComplete()
//	{
//		if (step != 0)
//			ans |= uint32_t (SPDR) << (4-step)*8;							// Читаем полученный байт
//		else
//			ans = 0;
//
//		if (step < 4)														// Если еще не вся инструкция передана,
//			SPDR = uint8_t (inst >> (3-step++)*8);							// передаем следующий байт
//		else
//		{
//			step = 0;
//			if (enableInt)
//			{
//				sei ();
//				completeInt ();
//			}
//		}
//	}
//
//
//	volatile uint32_t inst, ans;
//	volatile uint8_t step;
//	volatile bool enableInt;
//};



#endif /* PROGSPI_H_ */
