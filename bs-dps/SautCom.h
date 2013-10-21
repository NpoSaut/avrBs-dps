/*
 * SautCom.h
 *
 *	Связь по протоколу САУТ-485
 *	***************************
 *
 *  ~~~ Задача: ~~~
 *  Используя USART обеспечить связь по протоколу САУТ, описанному в соответсвующем документе, а именно:
 *  - Читать все проходящие по линии связи данные
 *    проверять их на ошибки,
 *    если они относятся к нужному полукомплекту,
 *    то формировать полученный пакет
 *  - При запросе на чтение из устройства, которое помечено как наше
 *    обеспечить передачу данных
 *  - Передавать 3-ий байт если мы БС-ДПС и в этом есть необходимость
 *
 *  ~~~ Решение: ~~~
 *
 *  ~~~ Интерфейс: ~~~
 *  1. Класс Com в качестве параметров шаблона принимает
 *     - регистры того usart-передатчика, с которым нужно работать
 *     - пины к которым подключены выводы usart-передатчика
 *     - пин, который конфигурирует линию связи САУТ на вход или на выход
 *     - пин, на который устройству подаётся к какому полукомплекту оно относится
 *     - номер устройства, от имени которого будет осуществляться отправка данных
 *     - функция размещения полученных пакетов (см. SautDat)
 *  2. В качестве аргументов для конструктора передаются делегаты обработчиков
 *     прерываний выбранного usart-передатчика. Подробнее смотри <avr/cpp/interrupt-dynamic.h>
 *  3. Для работы необходимо создать объект класса Com.
 *     Нельзя создавать два объекта одинаковых классов (с одинаковыми шаблонными параметрами),
 *     т.е. два коммуникатора на одном устройстве
 *  4.
 *
 *
 *
 *template	< typename ucsra, typename ucsrb, typename ucsrc, typename ubrr, typename udr,
			  typename rxPin, typename txPin, typename ioSwitchPin, typename scPin,
			  uint8_t myAdr,
			  void (*recipient) (const Packet&) >
 */

#ifndef SautCom_H_
#define SautCom_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include <cpp/universal.h>
#include <cpp/io.h>
#include <cpp/interrupt-dynamic.h>
#include <cpp/thread-safe.h>

#include "hw_defines.h"

#include "CanDesriptors.h"


namespace Saut
{

struct Packet
{
	struct Data
	{
		uint16_t d3	:4;
		uint16_t d2	:4;
		uint16_t d1	:4;
		uint16_t d0	:4;
	};
	Bitfield<Data> data;

	struct HeadFields
	{
		uint8_t	port	:2;
		uint8_t	input	:1;
		uint8_t	adr		:5;
	};
	typedef Bitfield<HeadFields> Head;
	Head head;
};

template	< volatile Bitfield<UsartControl> Register::* usartControl, volatile Bitfield<UsartBaudRate> Register::* usartBaudRate, volatile uint8_t Register::* usartData,
			  Port Register::* rxPort, uint8_t rxPin, Port Register::* txPort, uint8_t txPin, Port Register::* ioSwitchPort, uint8_t ioSwitchPin, Port Register::* scPort, uint8_t scPin,
			  uint8_t myAdr,
			  typename DatType, DatType& dat >
class Com
{
public:
	Com (InterruptHandler& rxHandler_, InterruptHandler& txHandler_, InterruptHandler& udreHandler_)
	{
		(reg.*txPort).pin<txPin>().out ();
		(reg.*txPort).pin<txPin>() = 1;
		(reg.*rxPort).pin<rxPin>().inPulled ();
		(reg.*ioSwitchPort).pin<ioSwitchPin>().out ();
		(reg.*scPort).pin<scPin>().inPulled ();

		// Устанавливаем наши обработчики прерываний в качестве обработчиков прерываний
		rxHandler_ = InterruptHandler::from_method <Com, &Com::rxHandler>(this);
		txHandler_ = InterruptHandler::from_method <Com, &Com::txHandler>(this);
		udreHandler_ = InterruptHandler::from_method <Com, &Com::udreHandler>(this);
//		rxHandler_ = InterruptHandler (this, &Com::rxHandler);
//		txHandler_ = InterruptHandler (this, &Com::txHandler);
//		udreHandler_ = InterruptHandler (this, &Com::udreHandler);

		UsartControl control;

		control.doubleSpeed = baudRateSet (115200);

		control.charSize2bit = true;												//
		control.charSize1bit = true;												//
		control.charSize0bit = true;												// 9 бит

		control.synchronous = false;
		control.parityGeneration = false;
		control.parityOdd = false;
		control.stopBitDouble = false;

		control.txEnable = true;													// Включен всегда, но молчит, когда нет отправки.
		control.rxCompleteInterrupt = true;										// Всегда заходить в прерывание по получении данных.
		control.txCompleteInterrupt = true;

		(reg.*usartControl) = control;

		block3Byte = 1;

		reset ();

		// DEBUG
		reg.timer2Compare = 255;
		reg.timer2Control.clockType = TimerControl8_2::ClockType::Prescale8; // 0,66667 мкс
		reg.timer2Control.waveform = TimerControl8_2::Waveform::Normal;
		reg.timer2Control.outputMode = TimerControl8_2::OutputMode::OutPinDisconnect;
		reg.timer2InterruptMask.CompInterrupt = false;
		reg.timer2InterruptMask.OverflowInterrupt = false;
	}

	uint16_t dataOut;

	// Отправлять ли 3 байт. 0 - да; 1..10 - нет + счётчик непринятых байт
	// Если приходит запрет на отправку, то перестаю отправлять
	volatile uint8_t block3Byte;
//	Safe<uint8_t> block3Byte;

	// Пройдено метров. Для передачи в 3-ем байте.
	uint8_t decimeters;
//	Safe<uint8_t> decimeters;
	volatile uint8_t termTime;

	void rxHandler ();
	void txHandler ();
	void udreHandler ();
//	INTERRUPT_HANDLER(rxHandler);
//	INTERRUPT_HANDLER(txHandler);
//	INTERRUPT_HANDLER(udreHandler);

private:
	bool flag;

	//Коды Хэмминга
	enum MessageType
	{
		Adr		= 1,
		Data	= 0
	};
	static const uint8_t ham[2][16];

	enum Step
	{
		Address 	= 0,
		Address2	= 1,
		Port_Sc		= 2,
		Get3Byte 	= 3,
		Data0		= 4,
		Data1		= 5,
		Data2		= 6,
		Data3		= 7,
		End			= 8
	};
	Step	step;

	Packet packet;

	// Задаёт скорость передачи данных
	// (все расчёты проводятся на момент компиляции, если известно baud)
	inline bool baudRateSet (unsigned long baud)
	{
		unsigned int ubrr_ = (F_CPU/16/baud-1);
		unsigned int ubrr2x = (F_CPU/8/baud-1);
		unsigned long rbaud = (F_CPU/16/(ubrr_+1));
		unsigned long rbaud2x = (F_CPU/8/(ubrr2x+1));

		unsigned long err1;
		if(baud > rbaud)
			err1 = (baud - rbaud)*1000/baud;
		else
			err1 = (rbaud - baud)*1000/rbaud;

		unsigned long err2;
		if(baud > rbaud2x)
			err2 = (baud - rbaud2x)*1000/baud;
		else
			err2 = (rbaud2x - baud)*1000/rbaud2x;

		bool doubleSpeed;
		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			doubleSpeed = true;
			ubrrToUse = ubrr2x;
		}
		else
		{
			doubleSpeed = false;
			ubrrToUse = ubrr_;
		}
		(reg.*usartBaudRate).baudRate = ubrrToUse;
		return doubleSpeed;
	}
	inline void reset ()
	{
		(reg.*usartControl).multiProcessorCommunicationMode = true;			// Получать только фреймы с адресом
		inMode ();
		step = Address;
	}
	inline void inMode ()
	{
//		ucsrb.txCompleteInterrupt = false;
//		ucsrb.dataRegEmptyInterrupt = false;								// Запрет прерываний по освобождению буфера - то же, что запрет отправки
//		ATOMIC
//		{
			(reg.*ioSwitchPort).pin<ioSwitchPin>() = 1;							// Конфигурируем селектор
			(reg.*usartControl).rxEnable = true;								// Разрешаем приём на UART'е
//		}
	}
	inline void outMode ()
	{
//		ucsrb.txCompleteInterrupt = true;
//		ATOMIC
//		{
			(reg.*usartControl).rxEnable = false;							// Запрещаем приём на UART'е
			(reg.*ioSwitchPort).pin<ioSwitchPin>() = 0;						// Конфигурируем селектор
//		}
//		ucsrb.dataRegEmptyInterrupt = true;								// Разрешаем прерывания по освобождению буфера, т.е. начинаем передачу
	}

	bool ioSwitch ()
	{
		return (reg.*ioSwitchPort).pin<ioSwitchPin>();
	}
};

template <  volatile Bitfield<UsartControl> Register::* usartControl, volatile Bitfield<UsartBaudRate> Register::* usartBaudRate, volatile uint8_t Register::* usartData,
			Port Register::* rxPort, uint8_t rxPin, Port Register::* txPort, uint8_t txPin, Port Register::* ioSwitchPort, uint8_t ioSwitchPin, Port Register::* scPort, uint8_t scPin,
			uint8_t myAdr,
			typename DatType, DatType& dat >
const uint8_t Com<usartControl, usartBaudRate, usartData, rxPort, rxPin, txPort, txPin, ioSwitchPort, ioSwitchPin, scPort, scPin, myAdr, DatType, dat>
	::ham[2][16] = 	{
					{0x00, 0x1F, 0x2E, 0x31, 0x4D, 0x52, 0x63, 0x7C, 0x8C, 0x93, 0xA2, 0xBD, 0xC1, 0xDE, 0xEF, 0xF0}, // 0 - Данные
					{0x0B, 0x14, 0x25, 0x3A, 0x46, 0x59, 0x68, 0x77, 0x87, 0x98, 0xA9, 0xB6, 0xCA, 0xD5, 0xE4, 0xFB}  // 1 - Адреса
					};


template <  volatile Bitfield<UsartControl> Register::* usartControl, volatile Bitfield<UsartBaudRate> Register::* usartBaudRate, volatile uint8_t Register::* usartData,
			Port Register::* rxPort, uint8_t rxPin, Port Register::* txPort, uint8_t txPin, Port Register::* ioSwitchPort, uint8_t ioSwitchPin, Port Register::* scPort, uint8_t scPin,
			uint8_t myAdr,
			typename DatType, DatType& dat >
void Com<usartControl, usartBaudRate, usartData, rxPort, rxPin, txPort, txPin, ioSwitchPort, ioSwitchPin, scPort, scPin, myAdr, DatType, dat>::rxHandler ()
{
	bool usartErrors = (reg.*usartControl).dataOverRunError || (reg.*usartControl).frameError ;

	struct
	{
		uint8_t	data;
		MessageType	type;
	} get;

	get.type = (MessageType) ((reg.*usartControl).rxDataBit8);	// Адрес или данные

	struct GetByte												// Проверка целосности
	{
		uint8_t ham :4;											// Защита Хэмминга
		uint8_t val :4;
	};
	Bitfield<GetByte> getByte (reg.*usartData);

	if (!usartErrors)
	{
		if ( getByte != ham[get.type][getByte.val] )			// Получили с ошибками
			goto reset;
		else
			get.data = getByte.val;

//		(reg.*usartControl).rxCompleteInterrupt = false;		// На время обработки запрещаем наши прерывания
//		sei ();													// но все другие разрешаем
start:
		if (step == Address)
		{
			if (get.type == Adr)
			{
				packet.head.adr = get.data;
				(reg.*usartControl).multiProcessorCommunicationMode = false; // Начинаем принимать всё
			}
			else
			{
				goto reset;
			}
		}
		else if (step == Address2)
		{
			if (get.type == Adr)								// Снова адрес - признак дополнительной области
				packet.head.adr |= (1 << 4);
			else												// На самом деле получили порт
				step = Port_Sc;;								// Cразу переходим на следющий step
		}
		if (step == Port_Sc)
		{
			struct GetCompl
			{
				uint8_t port			:2;
				uint8_t selfComplect	:1;
				uint8_t input			:1;
			};
			Bitfield<GetCompl> getCompl (get.data);

			if ( getCompl.selfComplect == (reg.*scPort).pin<scPin>() )		// Наш полукомплект
			{
				packet.head.input = getCompl.input;
				packet.head.port = getCompl.port;

				if (!block3Byte)								// Если 3-ий байт нужно отправлять нам
				{
					outMode ();
					(reg.*usartData) = ham[Data][decimeters&0x0f]; 			// Отправляем
//					start = reg.timer3Counter;
//					flag = true;

					step = Get3Byte;							// ещё +1 на выходе. Реально step = Data0  - Перепрыгиваем через получение 3-го байта

					if (dat.send(packet))
					{
						dataOut = (uint16_t) packet.data;
						cli ();									// Дождаться выхода из этого обработчика
						(reg.*usartControl).dataRegEmptyInterrupt = true;	// Оттуда отправка данных
					}
					else
					{
						flag = true;
						reg.timer2Counter = 0;
					}
				}
			}
			else
			{
				goto reset;
			}
		}
		else if (step == Get3Byte)
		{
			if (get.type == Adr)								// Не дождались 3-го байта - старый пакет не окончен, а начат новый.
			{
				if (block3Byte)									// Если мы в режиме блокирования посылок
				{
					block3Byte ++;
					if (block3Byte == 5)						// 10 раз не дождались 3-го байта на наш адрес, то
					{
						block3Byte = 0;							// отключаем блокировку
					}
				}
				goto reset;
			}
			else												// Действительно получили 3-ий байт
			{
				if (dat.send(packet))
				{
					outMode ();
					dataOut = (uint16_t) packet.data;
					cli ();										// Дождаться выхода из этого обработчика
					(reg.*usartControl).dataRegEmptyInterrupt = true;	// Оттуда отправка данных
				}
			}
		}
		else if (step == Data0)
		{
			if (get.type == Adr)								// Устройство не ответило на запрос, управляющее направило новый запрос
			{
				step = Address;
				goto start;
			}
			packet.data = 0;
			packet.data.d0 = (get.data);						// Получаем полубайт
		}
		else if (step == Data1)
		{
			packet.data.d1 = (get.data);						// Получаем полубайт
		}
		else if (step == Data2)
		{
			packet.data.d2 = (get.data);						// Получаем полубайт
		}
		else if (step == Data3)
		{
			packet.data.d3 = (get.data);						// Получаем полубайт
		}
	step = (Step) (step + 1);
	} // Получили без ошибок
	else
reset:		reset ();

	if (step == End)
	{
		reset ();
//		cli ();
//		(reg.*usartControl).rxCompleteInterrupt = true;		// Закончиил обработку. Можно принемать новые.
																// Нужно разрешить приём новых данных, т.к. долго
																// можем провисеть в обработчике принятых данных
																// я надеюсь на то, что в обработчике будет sei
		dat.recieve (packet);									// Передаём полученные данные дальше на обработку
	}
//	else
//		(reg.*usartControl).rxCompleteInterrupt = true;		// Закончиил обработку. Можно принемать новые.



}

template <  volatile Bitfield<UsartControl> Register::* usartControl, volatile Bitfield<UsartBaudRate> Register::* usartBaudRate, volatile uint8_t Register::* usartData,
			Port Register::* rxPort, uint8_t rxPin, Port Register::* txPort, uint8_t txPin, Port Register::* ioSwitchPort, uint8_t ioSwitchPin, Port Register::* scPort, uint8_t scPin,
			uint8_t myAdr,
			typename DatType, DatType& dat >
void Com<usartControl, usartBaudRate, usartData, rxPort, rxPin, txPort, txPin, ioSwitchPort, ioSwitchPin, scPort, scPin, myAdr, DatType, dat>::udreHandler()
{
	if (step == Data0)
		(reg.*usartData) = ham[Data][ dataOut / 4096 ];
	else if (step == Data1)
		(reg.*usartData) = ham[Data][ (dataOut / 256) & 0xF ];
	else if (step == Data2)
		(reg.*usartData) = ham[Data][ (dataOut / 16)  & 0xF ];
	else if (step == Data3)
	{
		(reg.*usartData) = ham[Data][ dataOut & 0xF ];
		(reg.*usartControl).dataRegEmptyInterrupt = false;	// Выключаем прерывания по осовбождению буфера (заканчиваем передачу).
															// После завершения передачи уйдём в прервыание по окончанию передачи
	}
	step = (Step) (step + 1);
}

template <  volatile Bitfield<UsartControl> Register::* usartControl, volatile Bitfield<UsartBaudRate> Register::* usartBaudRate, volatile uint8_t Register::* usartData,
			Port Register::* rxPort, uint8_t rxPin, Port Register::* txPort, uint8_t txPin, Port Register::* ioSwitchPort, uint8_t ioSwitchPin, Port Register::* scPort, uint8_t scPin,
			uint8_t myAdr,
			typename DatType, DatType& dat  >
void Com<usartControl, usartBaudRate, usartData, rxPort, rxPin, txPort, txPin, ioSwitchPort, ioSwitchPin, scPort, scPin, myAdr, DatType, dat>::txHandler()
{
	inMode ();
	uint8_t time = reg.timer2Counter;

	if ( step == End )											// Закончили отправку данных
		reset ();
	else if (flag)
	{
		flag = false;
//		if ( reg.timer2InterruptFlag.OverflowOccur )
//		{
//			reg.timer2InterruptFlag.OverflowOccur = 1;
//			time = 0xFF;
//		}

		if ( time  > 144 ) // очень странно, если нет
		{
			time -= 144;
			time = (time * 2) / 3; // время задержки в мкс
		}
		else
		{
			termTime = 0xFE;
			reg.general1 = termTime;
		}
	}

}

} // namespace Saut


#endif /* SautCom_H_ */
