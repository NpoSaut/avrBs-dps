/*
 * SautCom.h
 *
 *	Связь по протоколу САУТ-485 с управляющими устройствами
 *	*******************************************************
 *
 *	- Модуль использует USART передатчик
 *	- По запросу от управляющего устройства передаётся запрошенная информация из OutData[адрес_в_устройстве]
 *	- При приходе информация помещается в InData[адрес_в_устройстве] и вызывается функция обработки (определяется при инициализации)
 *	- Если работаем в режиме подстановки импульса ДПС (3-го байта), то подставляем его в каждый пакет
 *
 */

#ifndef SautCom_H_
#define SautCom_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "universal.h"
#include "hw_defines.h"

// ------------ SautCom defines ------------
#define SautComAddr			1										// Номер устрайства по протоколу САУТ-485 (БС-ДПС = 1)
#define SautComRWPort		PortD									// На этом порту висит
#define SautComRxBit		4										// переключатель in(1) / out(0) передатчика
// Pin полукомплекта
#define SautComPkPort		PortB									// На этом порту
#define SautComPkBit		7										// на это пине: 0 - для 1-ого полукомплекта, 1 - для 2-ой микросхемы
// Выбор USART передатчика  (сейчас: USART1)
#define USARTn_RX_vect		USART1_RX_vect
#define USARTn_UDRE_vect	USART1_UDRE_vect
#define USARTn_TX_vect		USART1_TX_vect
#define UCSRnA				UCSR1A
#define UCSRnB				UCSR1B
#define UCSRnC				UCSR1C
#define UBRRnH				UBRR1H
#define UBRRnL				UBRR1L
#define UDRn				UDR1
// Порт USART передатчика
#define USART_PORT			PortD
#define USART_RX_PIN		2
#define USART_TX_PIN		3
// -----------------------------------------


SIGNAL(USARTn_RX_vect);
SIGNAL(USARTn_UDRE_vect);
SIGNAL(USARTn_TX_vect);

class SautCom
{
public:
	static inline void Init  ();														// Настройка железа и переход в режим ожидания сообщений

	static FastDelegate1<uint8_t> CompleteInInt, CompleteOutInt;

//	static void (*CompleteInInt)  (uint8_t _IntAddr);									// Функция, вызываемая после приема данных
//	static void (*CompleteOutInt) (uint8_t _IntAddr);									// функция, вызываемая после передачи данных

	RegisterDeclare (IntAddrIOSet,
						In3, In2, In1, In0, Out3, Out2, Out1, Out0);

	static IntAddrIOSet CompleteIntEnable;												// Разрешение вызова функций CompleteIn(Out)Int по заврешению передачи на соответсвующий внетренний адрес
	static inline bool IsComplete (IntAddrIOSet WhichLook);								// Проверяет была ли окончена операция хотя бы по одному из адресов, указанных в WhichLook. При чтении сбрасывает соотв. поле.

	static volatile uint16_t DataOut [4];												// Входящие и исходящие данные:
	static volatile uint16_t DataIn	[4];												// номер в массиве соотвествует "внутреннему адресу" САУТ-485

	static volatile uint8_t	Block3Byte;													// Отправлять ли 3 байт. 0 - да; 1..10 - нет + счётчик непринятых байт
																						// Если приходит запрет на отправку, то перестаю оптравлять
	static volatile uint8_t Meters;														// Пройдено метров. Для передачи в 3-ем байте.

private:
	friend void USARTn_RX_vect ();
	friend void USARTn_UDRE_vect ();
	friend void USARTn_TX_vect ();

	static inline void RX_handler();													//Прерывание по окончанию приема фрейма
	static inline void UDRE_handler();													//Прерывание по осовбождению буфера на отправку
	static inline void TX_handler();													//Прерывание по завршениею передачи, когда в буфере ничего нет

	static inline void Reset ();														// Переход в режим ожидания сообщения
	static inline void OutMode ();
	static inline void InMode ();

	static IntAddrIOSet Complete;														// Флаг завершения

	static volatile bool Enable;														// При входе в Complete...Int выставляется в false
	static volatile bool Busy;															// Занятость от начала обработки пришедшего байта до конца или вызова Complete...Int
	static uint8_t	Step;
	static uint8_t	IsToMe;																// Мне ли сообщение, или только подстановка 3-го байта
	static uint8_t	IntAddr;															// Внутренний адрес в устройстве

	//Коды Хэмминга
	static const uint8_t HamDat[16];
	static const uint8_t HamAdr[16];
};

// --------------- SautCom - Инициализация ---------------
SautCom::IntAddrIOSet SautCom::Complete;
SautCom::IntAddrIOSet SautCom::CompleteIntEnable;
uint8_t SautCom::Step;
uint8_t SautCom::IsToMe;
uint8_t SautCom::IntAddr;
volatile uint8_t SautCom::Meters;
volatile uint8_t SautCom::Block3Byte;
volatile bool SautCom::Enable = true;
volatile bool SautCom::Busy = false;

FastDelegate1<uint8_t> SautCom::CompleteInInt;
FastDelegate1<uint8_t> SautCom::CompleteOutInt;

volatile uint16_t SautCom::DataIn [4];
volatile uint16_t SautCom::DataOut [4];

const uint8_t SautCom::HamDat[16]={0x00, 0x1F, 0x2E, 0x31, 0x4D, 0x52, 0x63, 0x7C, 0x8C, 0x93,0xA2,0xBD,0xC1,0xDE,0xEF,0xF0};
const uint8_t SautCom::HamAdr[16]={0x0B, 0x14, 0x25, 0x3A, 0x46, 0x59, 0x68, 0x77, 0x87, 0x98,0xA9,0xB6,0xCA,0xD5,0xE4,0xFB};

SIGNAL(USARTn_RX_vect) {SautCom::RX_handler();}
SIGNAL(USARTn_UDRE_vect) {SautCom::UDRE_handler();}
SIGNAL(USARTn_TX_vect) {SautCom::TX_handler();}
// -------------------------------------------------------


void SautCom::Init ()
{
	USART_PORT::dir() |= (1 << USART_TX_PIN);
	USART_PORT::dir() &= (uint8_t) ~(1 << USART_RX_PIN);
	USART_PORT::port()|= (1 << USART_TX_PIN) | (1 << USART_RX_PIN);

	SautComRWPort::dir() |= (1 << SautComRxBit);

	SautComPkPort::dir() &= (uint8_t) ~(1 << SautComPkBit);
	SautComPkPort::port()|= (1 << SautComPkBit);			// pull-up

	UBRRnH	= 0;											// baud rate..
	UBRRnL	= 12;											// ..UART_115.2k с удвоением;
	UCSRnA	= (1<<U2X);										// Удвоение скорости
	UCSRnB	= (1<<RXCIE) | (1<<TXEN) | (1<<UCSZ2);			// Разрешение прерываний по завершению передачи, включение передатчика, UCSZ2 - для 9 бит
	UCSRnC	= (1<<UCSZ0) | (1<<UCSZ1);						// Асинхронный режим, Parity off, 1 стоп-бит, 9 бит
	UCSRnB |= (1<<TXCIE);

	Block3Byte = 1;

	Reset ();
}

inline bool SautCom::IsComplete (IntAddrIOSet WhichLook)
{
	uint8_t Result = Complete & WhichLook;
	Complete = (IntAddrIOSet) Complete ^ Result;
	return Result;
}

inline void SautCom::Reset ()
{
	UCSRnA |= (1<<MPCM);									// включаем Multi-processor communication mode (получать только фреймы с адресом)
	InMode ();
	Step = 1;
}

inline void SautCom::InMode ()
{
	UCSRnB &= (uint8_t) ~(1<<UDRIE);						// Запрет прерываний по освобождению буфера - то же, что запрет отправки
	SautComRWPort::port() |= (1 << SautComRxBit);			// Конфигурируем селектор
	UCSRnB |= (1<<RXEN);									// Разрешаем приём на UART'е
}

inline void SautCom::OutMode ()
{
	UCSRnB &= (uint8_t) ~(1<<RXEN);							// Запрещаем приём на UART'е
	SautComRWPort::port() &= (uint8_t) ~(1 << SautComRxBit);// Конфигурируем селектор
	UCSRnB |= (1<<UDRIE);									// Разрешаем прерывания по освобождению буфера, т.е. начинаем передачу
}

inline void SautCom::RX_handler()
{
	static uint8_t GetByte, Status, Type;
	Status = UCSRnA;										// Ошибки передачи
	Type = UCSRnB;											// Адрес или данные
	GetByte = UDRn;											// Данные

	while (Busy);											// Дождаться освобождения
	Busy = true;
	sei ();

	if ( Status & ((1<<FE)|(1<<DOR)) )						// Ошибки USART'а
		Reset ();
	else
	{
		if (Type & (1<<RXB8))								// Адрес
		{
			if (Step == 3)									// не дождались 3-го байта
			{
				if (Block3Byte)								// Если мы в режиме блокирования посылок
				{
					Block3Byte ++;
					if (Block3Byte == 10)					// 10 раз не дождались 3-го байта на наш адрес, то
						Block3Byte = 0;						// отключаем блокировку
				}
				Step = 1;
			}
			if (Step == 1)									// Получаем номер устройства
			{
				IsToMe = (GetByte == HamAdr[SautComAddr]);	// Если получили свой адрес без ошибок
				UCSRnA &= (uint8_t) ~(1<<MPCM);				// Начинаем принимать всё (выключаем MPCM)
				Step ++;
			}
			else if (Step == 2)								// Возможно адрес в дополнительной области
			{
				if (GetByte == HamAdr[0])					// точно так
					IsToMe = 0;								// то просто пропускаем этот байт
				else										// если нет,
					Reset ();
			}
		}
		else												// Данные
		{
			if (GetByte != HamDat[GetByte/16])				// Получили с ошибкой
				Reset ();
			else
			{
				switch (Step)
				{
				case 2:										// Адрес в устройстве
					if ( (bool)((GetByte>>4) & 0b0100) != (bool)(SautComPkPort::pin() & (1<<SautComPkBit)) )	// Если не мой полукомплект
					{
						Reset ();
						break;
					}
					else
						IntAddr = GetByte>>4;
					Step ++;

					if (Block3Byte)
					{
						if (IsToMe)
							break;								// Ждём нового захода по окончанию приёма 3-го байта
						else
							Reset ();							// Наша работа закончина
					}


				case 3:								   			// 3-ий байт (импульс ДПС)
					if (!Block3Byte)							// Если отправлять 3-ий байт нужно нам
					{
						cli ();									// Чтобы не уйти в прервания раньше, чем выйдем из этого обработчика
						OutMode ();								// Отправляем
					}
					else										// Если мы его получили
					{
						Step ++;
						if ( (IntAddr & 0b1000) == 0 )			// Если запрос на чтение
						{
							cli ();            					// Чтобы не уйти в прервания раньше, чем выйдем из этого обработчика
							OutMode ();        					// Отправляем
						}
					}
					break;


				default:										// Приём данных полубайтами 4-7 шаг
					if (!Enable)								// Если заняты обработкой прервывния, то ничего не отправлять и не отправлять
					{
						Reset ();
					}
					else
					{
						if (Step == 4)
							DataIn[IntAddr&0b0011] = 0;
						DataIn[IntAddr&0b0011] |= (GetByte>>4)<<((7-Step)*4);
						Step ++;

						if (Step == 8)							// После принятия последнего байта
						{
							Reset ();
							if ( CompleteIntEnable & (1 << ((IntAddr&0b0011) + 4)) ) // Если после принятия нужно выполнять действия
							{
								Busy = false;					// Разрешаем обработку поступающих байт
								Enable = false;
								CompleteInInt (IntAddr&0b0011);
								Enable = true;
							}
							else
								Complete = Complete | 1 << ((IntAddr&0b0011) + 4);
						}
					}
					break;
				} //switch
			}//ham no error
		}//data
	}//usart no error
	Busy = false;
}

inline void SautCom::UDRE_handler()
{
	if (Step == 3)
	{
		//Meters++;
		UDRn = HamDat[Meters&0x0f];
		if ( (IntAddr & 0b1000)>>3 == 1 || !IsToMe)			// Запрос на запись или не наши данные
		{
			UCSRnB &= (uint8_t) ~(1<<UDRIE);				// Выключить отправку. Дальше пойдём в прерывание по окончанию передачи
		}
	}
	else if (Step < 8) 										// Step = 4..7 - передача полубайтами
	{
		if (!Enable)										// Если заняты обработкой прервывния, то ничего не отправлять
		{
			Reset ();
			return;
		}
		UDRn = HamDat[ ( DataOut[IntAddr&0b0011] >> ((7-Step)*4) ) & 0b1111 ];
	}
	else													// Step = 8
	{
		UCSRnB &= (uint8_t) ~(1<<UDRIE);					// Выключаем прерывания по осовбождению буффера (заканчиваем передачу).
															// После завершения передачи уйдём в прервыание по окончанию передачи
	}
	Step ++;
}

inline void SautCom::TX_handler()
{
	if (Step == 4) 											// Закончили отправку 3-го байта
	{
		if (IsToMe)
			InMode ();
		else
			Reset();
	}
	if (Step == 9)											// Закончили отправку данных
	{
		Reset ();
		if ( CompleteIntEnable & (1 << (IntAddr&0b0011)) )
		{
			Enable = false;
			sei();
			CompleteOutInt (IntAddr&0b0011);
			Enable = true;
		}
		else
			Complete |= 1 << (IntAddr&0b0011);
	}
}

#endif /* SautCom_H_ */
