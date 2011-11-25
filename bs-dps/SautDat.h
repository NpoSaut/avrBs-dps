/*
 * SautDat.h
 *
 *	Храниение данных из лини связи САУТ
 *	***********************************
 *
 *  ~~~ Задача: ~~~
 *  Из обмена по линии связи извлекать необходимые для работы модуля данные (небольшое число по сравнению со всеми).
 *  Хранить их актуальную версию и предоставлять к ним доступ.
 *  Иметь возможность вызывать определённую функцию по приходу определённого данного (для очень ограниченного числа данных).
 *  Предоставлять SautCom данные для отправки и информацию о том, что нужно отправлять, а что нет.
 *
 *  ~~~ Решение: ~~~
 *  1. Каждое данное имеет заголовок: адрес устройства, направление (от устройства или к устройству), порт.
 *     Диапазон возможных адресов: 1 - 17, направлении: 0 - 1, портов: 0 - 3. Итого для хранения заголовка достаточно 1 байта.
 *  2. Необходимо хранить малую часть данных.
 *     Их заголовки - ключи, по которым осуществляется доступ к данному. Клучи разбросаны произвольно в диапазоне 0 - 255.
 *     Следовательно для хранения удобно использовать ассоциативные массивы.
 *     В нашего случая разработана специальная реализация ассоциативных массивов в map-static.h, см. этот файл за подробностями.
 *  3. Для каждого заголовка необходимо хранить информацию о том, нужно ли его данное отправлять нам.
 *     Для этогоMapStatic реализуется иным образом.
 *     Данным для чтения в массиве присваиваются  номера 1 - N, где N - кол-во неоходимых для полученя данных.
 *     Данным для записи - номера N+1 - N+1+M, где M - кол-во данных для отправки. N+1+M не может быть больше 127.
 *     В хеш-таблице в старшем бите номера хранится информация о необходимости отправки, т.е.
 *     все данные для отправки на самом деле имеют номера 128+N+1 - 128+N+1+M.
 *  4. Так как данных, после которых нужно вызвать функции-обработчики, ещё на порядок меньше, чем сохраняемых данных, то
 *     делегаты обработчиков хранятся в ассоциативном массиве MapStatic, в качестве ключей используются номера из пункта 3.
 *
 *  ~~~ Детали реализации: ~~~
 *  1. Списки заголовков, данные которых нужно принемать, вызывать обработчиков, отправлять хранятся в списках типов Loki::Typelist.
 *  2. При обращении по заголовку
 *     с помощью таблицы, хранимой во flash-памяти, находится номер нужного данного в массиве, к нему предоставляется доступ.
 *     Если на момент компиляции известен заголовок, то можно на момент компиляции же сразу вычислить номер в массиве.
 *     Для этого создана шаблонная функция member<head> ().
 *  3. Детальное описсание алгоритма вызова функци по приходу данного: --------------------!
 *   3.1 При вызове функции ставится пометка о том, что функция активна (любая функция, здесь не различиются)
 *       При возврате из функции соовтетсвенно ставится пометко о том, что активной функции нет.
 *   3.2 Сам вызов функци происходит только если на момент считывания данного для последней отправки не было активных функции.
 *   3.3 В вызываемых функциях разрешены прерывания. Некоторую безопасность при работе с данными обеспечивает тип Safe<Type>.
 *   Такое поведение необходимо, чтобы корректно отрабатывать ситуацию, при которой:
 *     - посылается команда (т.е. данное на определённый порт)
 *     - читается флаг выполения команды (т.е. данное с определённого порта)
 *     - если по этому флагу команда ещё не выполнена, то команда посылается повторно.
 *   В этой схеме необходима защита от выполнения новой команды, если между чтеним флага и приходом повторной команды обработчик завершиться.
 *  4. Данные хранятся в переменных типа Safe<uint16_t>, который обеспечивают атомарный доступ.
 *     Подробнее смотри описание в <avr/cpp/thread-safe.h>
 *  5. По работе с делегатами смотри <avr/cpp/delegate/delegate.hpp>
 *
 *  ~~~ Интерфейс: ~~~
 *  1. В качестве шаблонных параметров классу Saut::Dat передаются 3 списка:
 *  	- список заголовков данных, которые необходимо принемать и хранить
 *  	- список заголовков данных, которые необходимо отправлять
 *  	- список заголовков данных, по приёму которых небходимо вызывать обработчик
 *     Эти списки ялвяются спискоми типов Loki::Typelist, в которых типами являются Loki::Int2Type.
 *     Для удобства генерации таких списков определён ряд макросов INT_TYPELIST_N (n_1, n_2, ... , n_N).
 *  2. Со стороны пользователя доступ к данным осуществляется с помощью:
 *  	- оператора [], где аргумент - заголовок представленный как объект типа Saut::Packet::Head.
 *  	- оператора (), где заголовок передаётся 3 параметрами: адресом, направлением и портом.
 *  	- шаблонной функции member<key>(), где заголовок передаётся в виде uint8_t.
 *  	  функция обеспечивает более быстрый доступ к данным, если заголовок извествен на момент компиляции.
 *  3. Для Saut::Com предоставляются функции:
 *  	- bool send (Packet& packet), которая сообщает о том, нужно ли отправлять данное c заголовком packet.head,
 *  	                              и если его нужно отправлять, то в packet.data заносятся данные на отправку
 *  	- void recieve (const Packet& packet), которая записывает принятое данное, если его заголовок есть в списке RxKeyList
 * 		                                       и вызывает функцию-обработчик, если это прописано в RxIntKeyList
 * 		                                       в соответсвии с алгоритмом изложенным в 3 пункте деталей реализиции
 *
 *  ~~~ Пример использования: ~~~

 	using namespace Saut;

	// Можно задать заголовкам удобные имена
	enum {
		// Входящие
		SomeInDataName1		= SautPacketHead (adr1, true, port1),
		SomeInDataName2		= SautPacketHead (adr1, true, port2),
		// Исходящие
		SomeOutDataName		= SautPacketHead (adr, false, port)
	};

	// Конфигурируем
	Dat <	INT_TYPELIST_2 (	// Список данных, которые нужно принемать
				SomeInDataName1, SomeInDataName2
				),
			INT_TYPELIST_1 (	// Данные для передачи
				SomeOutDataName
				),
			INT_TYPELIST_1 (	// Данные, по приходу которых вызываются прерывания
				SomeInDataName2
				) >
		data;

	// При создании коммуникатора
	Com	< // ... // параметры конфигурирования коммуникатора,
				  typeof(data), data > // передача по ссылке нашего объекта
		com (USART1_RX_handler, USART1_TX_handler, USART1_UDRE_handler); // ... продолжение конфигурирования -
																		// делегаты обработчиков прерываний USART

	if (data[SomeInDataName1] == 0x1234) 	// чтение данных
		else if (data.member<SomeInDataName2>() == 0x4321) // предпочтительный методе, более быстрый
	{
	 data(adr, false, port) = 0x0; // ещё один способ обращения к данным
	 //...
	}

	void f ()		// Функция, которую я хочу вызывать при поступлении данных SomeInDataName2
	{
		// ...
	}

	data.interruptHandler(SomeInDataName2) = InterruptHandler::from_function <&f> ();
 *
 */

#ifndef SautDat_H_
#define SautDat_H_

#include <cpp/thread-safe.h>
#include <cpp/map-static.h>
#include <cpp/loki/static_check.h>
#include <cpp/delegate/delegate.hpp>
#include <cpp/dispatcher.h>

#include "SautCom.h"


namespace Saut
{

template <class RxKeyList, class TxKeyList, class RxIntKeyList>
class Dat
{
public:
	Dat ()
	{
		LOKI_STATIC_CHECK (size <= 127, Size_of_RxKeyList_plus_TxKeyList_must_be_less_then_127);
		LOKI_STATIC_CHECK (sizeRx <= 15, Please_Increase_InterruptHandler_hash_table);
	}

	// Обспечивает доступ к данным по ключу
	//  если ключ данного не был объявлен ни в RxKeyList, ни в TxKeyList
	//  доступ будет предоставлен к "корзине"
	// ! Варианты operator[] или operator() медленные,  !
	// ! но если key известен на момент компиляции,     !
	// ! то возможно использовать быстрый member<key>() !
//	Safe<uint16_t>& operator[] (Packet::Head key)
//	{
//		uint8_t num = hash[ (uint8_t)key ] & 0x7F;
//		return data[num];
//	}
//	Safe<uint16_t>& operator() (uint8_t adr, bool input, uint8_t port)
//	{
//		return operator[] (Packet::Head(adr, input, port));
//	}

	// Быстрый доступ к данным по ключу (аналог operator[])
	// Необходимо, чтобы ключ был известен на момент компиляции
	template<uint8_t key>
	Safe<uint16_t>& member ()
	{
		enum { num = Val< Int2Type<key>, KeyNumbList >::Result::value & 0x7F };
		LOKI_STATIC_CHECK (num != 0, Key_cant_be_find_in_either_KeyLists);
		return data[num];
	}

	// Обеспечивает запись данных packet.data в ячейку с индексом packet.head.
	// Вызывает прерывание
	void recieve (const Packet& packet)
	{
		uint8_t num = hash[ packet.head ] & 0x7F;
		data[num] = (uint16_t) packet.data;

		if (!handlerState.busyAtLastSendMoment)				// Данные передаются дальше, только если
		{													// предыдущие уже были обработанны
			handlerState.busy = true;
			sei ();
			handler[num] ();
//			dispatcher.add (handler[num], packet.data);
			handlerState.busy = false;
		}
	}

	// Если packet.head содержится в TxKeyList,
	//	- то в packet.data заносит данные и возвращает true
	// иначе
	//	- возвращает false
	bool send (Packet& packet)
	{
		uint8_t num = hash[ (uint8_t)packet.head ];
		if ( num & (1 << 7) )	// Tx
		{
			handlerState.busyAtLastSendMoment = handlerState.busy;

			packet.data = +data[num & 0x7F];
			return true;
		}
		else
			return false;
	}

	template <uint8_t key>
	InterruptHandler& interruptHandler ()
	{
		enum { test = IndexOf< RxIntKeyList, Int2Type<key> >::value };
		LOKI_STATIC_CHECK (test != -1, Key_cant_be_find_in_RxIntKeyList);
		enum { num = Val< Int2Type<key>, RxKeyNumbList >::Result::value };
		return handler[num];
	}

	InterruptHandler handler0;

private:
	typedef typename NumberedGroup<RxKeyList, 1>::Result RxKeyNumbList;
	typedef typename Numbered<TxKeyList,
										128 + Length<RxKeyList>::value + 1 // offset 128 - старший бит указывает на то, что данные исходящии
										>::Result TxKeyNumbList;			// а нумерация продолжается далее с Length<RxKeyList>+1
	typedef typename Append<
						RxKeyNumbList,
						TxKeyNumbList
							>::Result KeyNumbList;
	ReIndex256< KeyNumbList > hash;

	enum { sizeRx = Length<RxKeyList>::value + 1 }; // проверки на максимальные значения в конструкторе
	enum { size = sizeRx + Length<TxKeyList>::value }; //
	Safe<uint16_t> data[ size ];

//	MapStatic16< SoftIntHandler,
//					typename ValList<RxIntKeyList, RxKeyNumbList>::Result > handler;
	MapStatic16< InterruptHandler,
					typename ValList<RxIntKeyList, RxKeyNumbList>::Result > handler;

	struct
	{
		volatile bool busyAtLastSendMoment :1;
		volatile bool busy :1;
	} handlerState ; // Занятость в обработке прерывания. Запрещает вызов новых прерываний.
};


} // namespace Saut

#define SautPacketHead(adr, input, port) adr*8 + input*4 + port

#endif /* SautDat_H_ */
