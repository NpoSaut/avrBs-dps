/*
 * CanDat.h
 *
 * Уровень приложения CAN-сети САУТ
 * ********************************
 *
 *  ~~~ Задача: ~~~
 *  Обеспечить передачу и приём данных по сети CAN, канальный и физический уровень которого реализованы аппаратно в AT90CAN:
 *	1. Произвести настройку аппаратной части, чтобы она корректно функционировала в данной сети.
 *	2. Предоставить возможность отправлять пакеты фиксированной длины с фиксированными ID, известными на момент разработки.
 *	3. Предоставить возможность принимать пакеты с фиксированными ID.
 *	   Что делать, если пользователь ещё не считал старый пакет с ID, но уже пришёл новый пакет с тем же ID?
 *	4. Предоставить возможность по приходу пакета с определённым ID вызывать пользовательскую функцию обработки.
 *	5. Свести к минимуму загрузку CPU, возникающую при приходе пакетов с ненужными ID, фильтрацию максимально выполнять на аппаратном уровне.
 *	6. Обеспечить возможную похожесть пользовательского интерфейса на SautDat.
 *
 *  ~~~ Решение: ~~~
 *  1. Настройка прозводиться в конструкторе, в качестве параметра требуется baud rate и samplePoint(по умолчанию 75%).
 *     Тайминги CAN подбираются в диапазонах, которые могут быть реализованы, при наличии нескольких вариантов выбирается с меньшим Tq.
 *  2. Списки необходимых для приёма и отправки ID и передаются в качестве шаблонных параметров, как в SautDat.
 *  3. Новый принятый пакет затерает старый. => По приёму в прерывании реинициализируем MOb на приём, принятое значение храним в своём буфере.
 *  4. Делегат пользовательской функции ставиться в очередь dispatcher'а. Список ID, по которым необходимы прерывания передаётся как в SautDat.
 *  5. Т.к. количество необходимых ID для приёма и передачи может превышать кол-во доступных MOb'ов, то может возникнуть необходимость:
 *     1. В качестве ID на передачу задвать список id. Для одного списка отводится один Mob.
 *        Цена: при каждой отправке нужно заносить ID, нельзя сражу же поставить в очередь на отправку несколько ID из одной группы.
 *     2. ID для приёма сгруппировать в группы с масками, таким образом, чтобы одну группу обрабатывал один MOb,
 *        однако при этом может получиться так, что по этой маске будут приниматься нежелаемые пакеты, который нужно программно отфильтровывать.
 *
 *     Принятие любого ID (в том числе лишнего) влечёт вызов прерывания и работу по реинициализации MOb'а, это существенно нагружает CPU,
 *     поэтому необходимо стремиться к минимизации лишних ID. Однако задача нахождения оптимальных групп при заданом их кол-ве оказывается
 *     весьма сложной и не может быть выполнена автоматически. Поэтому от пользователя требуется самому распределить ID по группам и
 *     предоставить классу уже готовые списки групп с масками.
 *
 *  ~~~ Интерфейс: ~~~
 *  Класс CanDat с шаблонными параметрами:
 *  TxDescriptorGroupList		- список декскрипторов (или групп дескпирторов), которые необходимо отправлять.
 *  				 			  Дескриптор = Id(11bit) * 0x20 + размер пакета.
 *  							  Список является списком типов Loki. Для его генерации можно использовать дефайны INT_TYPELIST_N (1,2,...,N)
 *  							  Как работать с группами смотри ниже
 * 	RxDescriptorGroupList		- список групп дескрипторов, которые необходимо принемать.
 * 								  Дескрипторы (при необходимости) объединяются в группы таким образом,
 * 								  чтобы у дескрипторов одной группы было по возможности меньше отличающихся бит.
 * 								  Каждая группа представляет из себя список дескрипторов. Список геренируется с помощью INT_TYPELIST_N.
 * 								  Таким образом, RxDescriptionGroupList - список списков.
 * 								  Если в группе только один id, то онм может быть записан как группа из одного элемента INT_TYPELIST_1(id)
 * 								  или как одиночный элемент-тип Int2Type<id>
 * 	RxDescriptorInterruptList	- список тех дескрипторов, по приёму которых необходимо выполнять пользовательский код ("программное прерываение")
 * 								  Этот список - обычный негруппированный список.
 * 	baudKbit					- baud rate в килобитах.
 * 	samplePointPercent			- позиция точки выборки в процентах (необязательный параметр: по умолчанию = 75)
 * 								  Если для требуемых baud rate и sample point на данной тактовой частосте (берётся из F_CPU)
 * 								  невозможно подобрать реализуемые тайминги, то будет выдана ошибка компиляции.
 * 								  В этом случае попробуйте немого изменить точку выборки.
 * 	Функции:
 *	bool send<descriptor> (data[])	- инициирует отправку данных uint8_t data[] с дескриптором descriptor
 *									  В массиве байт data[] должно быть не менее N элементов, где N - длина, указанная в дескрипторе
 *									  Массив передаётся по ссылке.
 *									  descriptor должен быть из списка TxDescriptorList, в противном случае будет выдана ошибка при компиляции
 *									  descriptor должен быть константой, известной на момент компиляции
 *									  Если предыдущее сообщение в этим дескриптором до сих пор не было отправлено, то возвращается false,
 *									  текущее сообщение теряется, прошлое остаётся в очереди на отправку
 *	get<descriptor> ()				- возвращает последние полученные данные с дескрипторов descriptor
 *									  Возвращается ссылка на буфер данных с требуемым дескриптором. В буфере храниться последнее принятое сообщение.
 *									  Однако в момент чтения этого буфера могут прийти новые данные, и значение буфера изменится.
 *									  В таких случаях пользователь сам должен обеспечивать корректность считывания путём применения критический секций.
 *									  descriptor должен находиться в одной из групп в списке RxDescriptorGroupList
 *									  descriptor должен быть константой, известной на момент компиляции
 *	rxHandler<descriptor> ()		- возвращает ссылку на делегат, который будет поставлен в очередь dispatcher'а по приходу данных с descriptor
 *									  Подробнее о работе с делегатами и диспетчером смотри описание в соответсвующих файлах.
 *
 *  ~~~ Комментарии: ~~~
 *  1. Вся настройка на основе предоставленных списков происходит на момент компиляции.
 *     Т.е. происходит генерация кода, который заточен под кокретную конфигурацию, поэтому везде, где это теоретически возможно, отсутсвует overhead.
 *     Всё это реализованно в русле подхода метапрограммирования, описанного в книге Андрея Александреску.
 *
 *  ~~~ Пример использования: ~~~
	typedef INT_TYPELIST_2 (0x1000, 0x1080) GP1;	// Группа дескрипторов на приём
	typedef INT_TYPELIST_1 (0x3004) GP2;			// Одиночный дескриптор, оформленный как группа

	CanDat < INT_TYPELIST_4(						// Список дескрипторов для отправки
						0x4668,
						0x1888,
						0x63E7,
						0x63C8
							),
			LOKI_TYPELIST_3(						// Список дескрипторов на приём (по группам)
						GP1,						// Группа. Маска для группы расчитывется автоматически
						Int2Type<0x2008>, 			// Одиночный дескриптор
						GP2							// Одиночный дескриптор в группе
							),
			INT_TYPELIST_2(							// Список дескрипторов, по приёму которых необходимо выполнять действия
						0x1000,						// По приёму этого дескриптора наша функция будет добавлена в очередь dispatcher'а на выполнение
						0x3004						// А этот дескриптор я объявил по ошибке и не задал никакой функции. Всё будет хорошо, но появиться небольшой overhead.
						  ),
			100 >									// BaudRate = 100 Кбит, SamplePoint = 75% (по умолчанию)
		canDat;

	void f (uint16_t a)								// Эту функцию зададим в качестве действия после принятия сообщения с дескриптором 0x1000
	{
		uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		canDat.send<0x4668> (data); 				// Отправляем пакет с дескрипторов 0x4668 и данными data

		uint8_t something = canDat.get<0x2008>()[7]; // Читаю 7 байт последнего принятого сообщения с дескриптором 0x2008

		if ( !canDat.send<0x1888> (data) )			// Отправляю, но проверяю, возможно ли.
			asm volatile ("nop");					// Если не получилось, то выполняю какой-то код
	}

	int main ()
	{
		canDat.rxHandler<0x1000>() = SoftIntHandler (f); // Передаём делегат на нашу функцию в CanDat
		//...
		for (;;)
			dispatcher.invoke ();
	}
 *
 */

#ifndef CANDAT_H_
#define CANDAT_H_

#include <cpp/universal.h>
#include <cpp/io.h>
#include <cpp/interrupt-dynamic.h>
#include <cpp/dispatcher.h>
#include <cpp/int-typelist.h>
#include <cpp/loki/Typelist.h>
#include <cpp/loki/static_check.h>
using namespace Loki;
using namespace Loki::TL;

namespace CanDatPrivate
{
	// Типы - булевые флаги
	class True;
	class False;


	// Если параметр a == 0, то возвращает True
	template<long a> struct IsZero;

		template<>
		struct IsZero <0> { typedef True Result; };

		template<long a>
		struct IsZero { typedef False Result; };

	// Если параметр a == true, то возвращает True
	// В противном случае возвращает False
	template<bool a> struct If;

		template<>
		struct If <true> { typedef True Result; };

		template<bool a>
		struct If { typedef False Result; };


	// Параметр a - дробный: 2 последних разряда - сотые доли
	// Функция возвращает True если a в таком представлении является целым (т.е. если делиться на цело на 100)
	template<long a> struct IsInteger
		{
			typedef typename IsZero<a%100>::Result Result;
		};


	// Логическое И
	// Результать True тогда и только тогда, когда A = True и B = True
	template<class A, class B> struct LogicalAnd;

		template<>
		struct LogicalAnd<True, True>
		{
			typedef True Result;
		};

		template<class A, class B>
		struct LogicalAnd
		{
			typedef False Result;
		};


	// Расчитывает тайминги CAN:
	// 	tBit = 1 + tPrs + tPhs1 + tPhs2
	// исходя из:
	//	- основной частоты ЦПУ clock (в КГц),
	//	- делителя brp, такого что время кванта CAN Tq = (brp + 1) / clock
	//	- желаемого baudRate (в КБит)
	//	- желаемой точки выборки (sample point) (в процентах*100)
	// Если удаётся подобрать целые тайминги в требуемом диапазоне, то возвращается True,
	// в противном случае - False
	template<long clock, long baud, long brp, long samplePoint> class Tbit
		{
		private:
			enum { tBitX100 = (clock*100)/(baud*(brp+1)) };
			enum { tPhs2X100 = (tBitX100/100) * (100 - samplePoint) };
		public:
			enum { tBit = tBitX100/100 };
			enum { tPhs2 = tPhs2X100/100 };
			enum { tPrs = (tBit - tPhs2 - 1)/2 + 1 };
			enum { tPhs1 = tBit - 1 - tPrs - tPhs2 };
			typedef typename LogicalAnd<
						typename IsZero< (tPrs <= 8 && tPhs1 <= 8 && tPhs2 <= 8) ? 0 : 1 >::Result,
						typename LogicalAnd<
							typename IsInteger<tBitX100>::Result,
							typename IsInteger<tPhs2X100>::Result
										 	>::Result
										>::Result Result;
		};


	// Пытается подобрать brp, который при расчёте с помощью Tbit дал бы целые тайминги
	// Перебор ведётся начиная от brp до brpMax включительно,
	// при первом удачном brp поиск останавливается, возвращается найденное brp.
	// Если не с одним из brp не подходит, то возвращается brpMax+1
	// Остальные параметры - как в Tbit
	template< long clock, long baud, long samplePoint,
			  long brp, long brpMax, class lastSuccess = False> struct BrpSelect;

	template< long clock, long baud, long samplePoint,
			  long brp, long brpMax>
		struct BrpSelect<clock, baud, samplePoint, brp, brpMax, False>
		{
			enum { number =
					BrpSelect< clock, baud, samplePoint, brp+1, brpMax,
										typename Tbit<clock, baud, brp, samplePoint>::Result >::number };
		};

	template< long clock, long baud, long samplePoint,
			  long brp, long brpMax>
		struct BrpSelect<clock, baud, samplePoint, brp, brpMax, True>
		{
			enum { number = brp - 1 };
		};

	template< long clock, long baud, long samplePoint,
			  long brpMax>
		struct BrpSelect<clock, baud, samplePoint, brpMax, brpMax, False>
		{
			enum { number = brpMax + 1 };
		};


	// Для рабочей частосты ЦПУ clock (в МГц)
	// требуемого baud Rate (в КБит)
	// и точки выборки (sample point) (в процентах*100)
	// подбирает реализуемое на практике brp, которое даст целые тайминги,
	// которые можно вычислить с помощью Tbit
	// Если подобрать не удаётся, то возвращается False
	template<long clock, long baud, long samplePoint> class Brp
		{
		private:
			enum { brpMinX100 = (clock*100)/(25*baud) - 100 };
			enum { brpMaxX100_ = (clock*100)/(8*baud) - 100 };
			enum { brpMaxX100 = (brpMaxX100_ >= 5900) ? 5900 : (brpMaxX100_) };
			enum { brpMin = (brpMinX100 + 99) / 100 }; // округляем в большую сторону
			enum { brpMax = brpMaxX100 / 100 }; // округляем в меньшую сторону

		public:
			enum { brp = BrpSelect<clock, baud, samplePoint, brpMin, brpMax+1, False>::number };
			enum { success = (brp != (brpMax+1)+1) };
		};


	// Линерилезация списка ->
	template<class GroupList> struct LinerList;

	template<class HeadH, class HeadT, class Tail>
	struct LinerList< Typelist< Typelist<HeadH, HeadT>, Tail> > // если текущий элемент - группа
	{
		typedef typename Append<
					Typelist<HeadH, HeadT>,
					typename LinerList<Tail>::Result
						>::Result Result;
	};

	template<class Head, class Tail>
	struct LinerList< Typelist<Head, Tail> > // если тукущий элемент - одиночный
	{
		typedef Typelist<
					Head,
					typename LinerList<Tail>::Result
						> Result;
	};

	template<>
	struct LinerList< NullType >
	{
		typedef NullType Result;
	};

//	template<class HeadH, class HeadT>
//	struct LinerList< Typelist< Typelist<HeadH, HeadT>, NullType> > // если текущий элемент - группа, и он последний
//	{
//		typedef Typelist <HeadH, HeadT> Result;
//	};
//
//	template<class Head>
//	struct LinerList< Typelist<Head, NullType> > // если тукущий элемент - одиночный, и он одиночный
//	{
//		typedef Typelist<Head, NullType> Result;
//	};
	// <- конец: линерилезация списка

};

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent = 75 >
class CanDat
{
public:
	CanDat ();

	template<uint16_t descriptor>
	bool send (const uint8_t (&data)[descriptor & 0xF]);

	template<uint16_t descriptor>
	bool send ();

	template <uint16_t descriptor>
	const uint8_t (&get())[descriptor & 0xF];

	template <uint16_t descriptor>
	SoftIntHandler& rxHandler();

	template <uint16_t descriptor>
	SoftIntHandler& txHandler();


private:
	enum { txNumber = Length<TxDescriptorGroupList>::value };
	enum { rxGroupNumber = Length<RxDescriptorGroupList>::value };

	void interruptHandler ();

	// Инициализация Tx MOb'ов ->
	template<class TList, uint8_t num = 0> struct InitTxList;

		template<class HeadH, class HeadT, class Tail, uint8_t num>
		struct InitTxList< Typelist< Typelist<HeadH, HeadT>, Tail>, num > // спискок дескрипторов
		{
			static void init ()
			{
				reg.canPage.mobNumber = num;

				CanMobControl t;
				t.type = CanMobControl::Disable; // Включаются, когда в них заносятся данные
				t.dataLength = 8; // Не актуально. Реальное значение заносится при отправке.
				t.idExtension = false;
				t.automaticReply = false;
				reg.canMobControl = t;

				// Id заносится в момент отправки

				CanMobMask m;
				m.idExtension = 1;
				m.remoteTransmissionRequest = 1;
				m.idA = 0x7FF;
				reg.canMobMask = m;

				InitTxList<Tail, num+1>::init ();
			}
		};

		template<class Head, class Tail, uint8_t num>
		struct InitTxList< Typelist< Typelist<Head, NullType>, Tail>, num > // списко из одного дескриптора
		{
			static void init ()
			{
				reg.canPage.mobNumber = num;

				CanMobControl t;
				t.type = CanMobControl::Disable; // Включаются, когда в них заносятся данные
				t.dataLength = Head::value & 0xF;
				t.idExtension = false;
				t.automaticReply = false;
				reg.canMobControl = t;

				CanMobId id;
				id.rb0 = 0;
				id.remoteTransmissionRequest = 0;
				id.idA = Head::value / 0x20;
				reg.canMobId = id;

				CanMobMask mask;
				mask.idExtension = 1;
				mask.remoteTransmissionRequest = 1;
				mask.idA = 0x7FF;
				reg.canMobMask = mask;

				InitTxList<Tail, num+1>::init ();
			}
		};

		template<class Head, class Tail, uint8_t num>
		struct InitTxList< Typelist<Head, Tail>, num > // одиночный дескриптор
		{
			static void init ()
			{
				reg.canPage.mobNumber = num;

				CanMobControl t;
				t.type = CanMobControl::Disable; // Включаются, когда в них заносятся данные
				t.dataLength = Head::value & 0xF;
				t.idExtension = false;
				t.automaticReply = false;
				reg.canMobControl = t;

				CanMobId id;
				id.rb0 = 0;
				id.remoteTransmissionRequest = 0;
				id.idA = Head::value / 0x20;
				reg.canMobId = id;

				CanMobMask mask;
				mask.idExtension = 1;
				mask.remoteTransmissionRequest = 1;
				mask.idA = 0x7FF;
				reg.canMobMask = mask;

				InitTxList<Tail, num+1>::init ();
			}
		};

		template<uint8_t num>
		struct InitTxList<NullType, num>
		{
			static void init () {}
		};
	// <- конец: Инициализация Tx MOb'ов


	// Определение маски для списка элементов ->
	// 0 - в позициях, одинаковых для всех элементов
	template <class TList> struct Mask;

		template <class Head, class Head2, class Tail>
		struct Mask< Typelist< Head, Typelist<Head2, Tail> > >
		{
			enum { value = (Head::value ^ Head2::value) | Mask< Typelist<Head2, Tail> >::value };
		};

		template <class Head>
		struct Mask< Typelist<Head, NullType> >
		{
			enum { value = 0 };
		};
	// <- конец: определение маски для списка элементов


	// Инициализация Rx MOb'ов в соответсвии с группами ->
	template<class GroupList, uint8_t num = txNumber> struct InitRxGroupList;

		template<class HeadHead, class HeadTail, class Tail, uint8_t num>
		struct InitRxGroupList< Typelist< Typelist<HeadHead, HeadTail>, Tail >, num > // Элемент - список id (группа)
		{
			static void init ()
			{
				reg.canPage.mobNumber = num;

				CanMobControl contr;
				contr.type = CanMobControl::Receive; // Включены всегда, получение по прерываниям
				contr.dataLength = 8; // Реальную длину всегда получаем вместе с сообщением
				contr.idExtension = false;
				contr.automaticReply = false;
				reg.canMobControl = contr;

				CanMobId id;
				id.rb0 = 0;
				id.remoteTransmissionRequest = 0;
				id.idA = HeadHead::value / 0x20; // id нулевого элемента (можно брать любого)
				reg.canMobId = id;

				CanMobMask mask;
				mask.idExtension = 1;
				mask.remoteTransmissionRequest = 1;
				mask.idA = (Mask< Typelist<HeadHead, HeadTail> >::value / 0x20) ^ 0x7FF ;
				reg.canMobMask = mask;

				InitRxGroupList<Tail, num+1>::init ();
			}
		};

		template<class Head, class Tail, uint8_t num>
		struct InitRxGroupList< Typelist< Head, Tail >, num > // Элемент - одиночный id
		{
			static void init ()
			{
				reg.canPage.mobNumber = num;

				CanMobControl contr;
				contr.type = CanMobControl::FrameBufferReceive; // Включены всегда, получение по прерываниям
				contr.dataLength = 8; // Реальную длину всегда получаем вместе с сообщением
				contr.idExtension = false;
				contr.automaticReply = false;
				reg.canMobControl = contr;

				CanMobId id;
				id.rb0 = 0;
				id.remoteTransmissionRequest = 0;
				id.idA = Head::value / 0x20; // id
				reg.canMobId = id;

				CanMobMask mask;
				mask.idExtension = 1;
				mask.remoteTransmissionRequest = 1;
				mask.idA = 0x7FF;; // только этот id
				reg.canMobMask = mask;

				InitRxGroupList<Tail, num+1>::init ();
			}
		};

		template<uint8_t num>
		struct InitRxGroupList<NullType, num>
		{
			static void init () {}
		};
	// <- конец: Инициализация Rx MOb'ов


	typedef typename CanDatPrivate::LinerList<RxDescriptorGroupList>::Result RxDescriptorList;
	// Буфер принемаемых данных. Для каждого дескриптора хранятся последние принятые данные
	uint8_t data[Length<RxDescriptorList>::value][8];

	// Делегаты функций, вызываемые по приёму данных
	SoftIntHandler handlerRx[Length<RxDescriptorInterruptList>::value];
	// Делегаты функций, вызываемые по окончанию отправки данных
	SoftIntHandler handlerTx[Length<RxDescriptorInterruptList>::value];

	// Принятые данные для обработчиков. Данные записываются последовательно.
	// Обработчику передаётся указатель на начало.
	uint8_t dataInterrupt[dataBufferSize];
	uint8_t dataInterruptPointer;

	// Генерация кода нахождения номера по декскриптору ->
	// функция index возвращает номер, если дескриптор найден в списке
	// в противном случае 255.
	template<class TList, int num = 0> struct IndexFinder;

		template<class Head, class Tail, int num>
		struct IndexFinder< Typelist<Head, Tail>, num >
		{
			static uint8_t index (const uint16_t& descriptor)
			{
				if (descriptor == Head::value)
					return num;
				else
					return IndexFinder<Tail, num+1>::index (descriptor);
			}
		};

		template <int num>
		struct IndexFinder< NullType, num >
		{
			static uint8_t index (const uint16_t& descriptor)
			{
				return 255;
			}
		};
	// <- конец: генерация кода нахождения номера по декскриптору


	// Нахождения номера группы элемента ->
	template<class GroupList, class T> class GroupOf;

		template<class GroupHead, class GroupTail, class Tail, class T>
		class GroupOf< Typelist< Typelist<GroupHead, GroupTail>, Tail>, T >
		{
		private:
			enum { temp = GroupOf<Tail, T>::value };
		public:
			enum { value = (IndexOf< Typelist<GroupHead, GroupTail>, T >::value != -1) ?
							0 :
							( temp != -1 ? temp+1 : -1 ) };
		};

		template<class Tail, class T>
		class GroupOf< Typelist<T, Tail> , T > // одиночный элемент - тот, которого ищем
		{
		public:
			enum { value = 0 };
		};

		template<class Alone, class Tail, class T> // одиночный элемент - но не наш
		class GroupOf< Typelist<Alone, Tail> , T >
		{
		private:
			enum { temp = GroupOf<Tail, T>::value };
		public:
			enum { value = (temp != -1) ? temp+1 : -1 };
		};

		template<class T>
		class GroupOf< NullType, T >
		{
		public:
			enum { value = -1 };
		};
	// <- конец: Нахождения номера группы элемента

	// Для всех дескрипторов из DescriptorList находит номера их групп в GroupList
	// и составляет маску используемых групп ->
	template<class DescriptorList, class GroupList> struct GroupBusy;

		template<class Head, class Tail, class GroupList>
		struct GroupBusy< Typelist<Head, Tail>, GroupList >
		{
		public:
			enum { value = (1 << GroupOf <GroupList, Head>::value) | GroupBusy <Tail, GroupList>::value };
		};

		template<class GroupList>
		struct GroupBusy< NullType, GroupList >
		{
		public:
			enum { value = 0 };
		};

	// Является ли класс Typelist'ом с хотябы 2 элементами ->
	template<class T> struct IsGroup;

		template<class Head, class Tail>
		struct IsGroup< Typelist<Head, Tail> >
		{
			enum { value = 1 };
		};

		template<class Head>
		struct IsGroup< Typelist<Head, NullType> >
		{
			enum { value = 0 };
		};

		template<class Alone>
		struct IsGroup
		{
			enum { value = 0 };
		};
	// <- конеы: Является ли класс Typelist'ом с хотябы 2 элементами
};

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
		::CanDat()
{
	// Настройка CAN
	enum { clock = F_CPU/1000 }; // Частота ЦПУ в КГц
	enum { brp = CanDatPrivate::Brp<clock, baudKbit, samplePointPercent>::brp };
	enum { success = CanDatPrivate::Brp<clock, baudKbit, samplePointPercent>::success };
	// Удалось ли подобрать целочисленные brp и тайминги, чтобы удовлетворить желаемым условиям.
	// Если нет, измените условия.
	// Выдать ошибку компиляции:
	LOKI_STATIC_CHECK (success, For_necessary_parameters_cant_be_find_correct_BRP_and_timings___Try_to_change_some_of_them);

	typedef typename CanDatPrivate::Tbit<clock, baudKbit, brp, samplePointPercent> Tbits;

	reg.canGeneralConfig.softwareReset = true;

	CanTiming canTiming;
	canTiming.baudRatePrescaler = brp;
	canTiming.propagationTime = Tbits::tPrs - 1;
	canTiming.phaseSegment1Time = Tbits::tPhs1 - 1;
	canTiming.phaseSegment2Time = Tbits::tPhs2 - 1;
	if (brp != 0)
		canTiming.threePointSampling = true;
	else
		canTiming.threePointSampling = false;
	reg.canTiming = canTiming;

	// Настройка MOb'ов
	LOKI_STATIC_CHECK (txNumber + rxGroupNumber <= 15, Maximum_number_of_TxDescriptors_plus_RxDescriptorGroups_must_be_less_then_15__Try_to_group_RxDescriptros_to_lesser_number_of_group);

	InitTxList<TxDescriptorGroupList>::init();
	InitRxGroupList<RxDescriptorGroupList>::init();

	for (uint8_t i = txNumber + rxGroupNumber; i < 15; i ++) // Отключить остальных
	{
		reg.canPage = CanPage {0, 0, i};
		reg.canMobControl = CanMobControl {0, 0, 0, CanMobControl::Disable};
	}

	// Прерывания
	uint16_t interruptFlags = GroupBusy <TxDescriptorInterruptList, TxDescriptorGroupList>::value;
	interruptFlags |= (GroupBusy <RxDescriptorInterruptList, RxDescriptorGroupList>::value << txNumber);
	reg.canMobInterruptEnable = interruptFlags;

	CanGeneralInterruptEnable interruptEnable;
	interruptEnable.receive = true;
	interruptEnable.transmit = true;
	interruptEnable.general = true;
	interruptEnable.bussOff = true;
	reg.canGeneralInterruptEnable = interruptEnable;
	CANIT_handler = InterruptHandler::from_method <CanDat, &CanDat::interruptHandler>(this);

	// Настройка highestPriorityMob, чтобы его полностью копировать в canPage
	reg.canHighestPriorityMob = 0;

	reg.canGeneralConfig.enable = true;

	while (!reg.canGeneralStatus.enable);
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
template<uint16_t descriptor>
bool CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
		::send (const uint8_t (&data)[descriptor & 0xF])
{
	enum { mobN = GroupOf< TxDescriptorGroupList, Int2Type<descriptor> >::value };
	enum { size = descriptor & 0xF };

	LOKI_STATIC_CHECK (mobN >= 0, Required_descriptor_cant_be_find_in_TxDescriptorGroupList);

	if ( reg.canMobEnable & (1 << mobN) )
		return false;
	else
	{
		uint8_t canPageSave = reg.canPage; // чтобы вернуть назад

		CanPage page;
		page.mobNumber = mobN;
		page.dataBufferIndex = 0;
		page.autoIncrementDisable = false;
		reg.canPage = page;

		if (IsGroup< typename TypeAt<TxDescriptorGroupList, mobN>::Result >::value) // если этот моб для нескольких id
		{
			CanMobId id;
			id.rb0 = 0;
			id.remoteTransmissionRequest = 0;
			id.idA = descriptor / 0x20;
			reg.canMobId = id;
		}

		for (uint8_t i = 0; i < size; ++i)
		{
			reg.canMobData = data[i];
		}

		CanMobControl control;
		control.type = CanMobControl::Transmit; // Включаются, когда в них заносятся данные
		control.dataLength = size;
		control.idExtension = false;
		control.automaticReply = false;
		reg.canMobControl = control;

		reg.canPage = canPageSave; // возвращаем на место
		return true;
	}
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
template<uint16_t descriptor>
bool CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
		::send ()
{
	enum { mobN = GroupOf< TxDescriptorGroupList, Int2Type<descriptor> >::value };
	enum { size = descriptor & 0xF };

	LOKI_STATIC_CHECK (mobN >= 0, Required_descriptor_cant_be_find_in_TxDescriptorGroupList);
	LOKI_STATIC_CHECK (size == 0, Required_descriptor_hase_nonzero_size_Try_to_use_send_method_with_array_arg);

	if ( reg.canMobEnable & (1 << mobN) )
		return false;
	else
	{
		uint8_t canPageSave = reg.canPage; // чтобы вернуть назад

		CanPage page;
		page.mobNumber = mobN;
		page.dataBufferIndex = 0;
		page.autoIncrementDisable = false;
		reg.canPage = page;

		if (IsGroup< typename TypeAt<TxDescriptorGroupList, mobN>::Result >::value) // если этот моб для нескольких id
		{
			CanMobId id;
			id.rb0 = 0;
			id.remoteTransmissionRequest = 0;
			id.idA = descriptor / 0x20;
			reg.canMobId = id;
		}

		CanMobControl control;
		control.type = CanMobControl::Transmit; // Включаются, когда в них заносятся данные
		control.dataLength = size;
		control.idExtension = false;
		control.automaticReply = false;
		reg.canMobControl = control;

		reg.canPage = canPageSave; // возвращаем на место
		return true;
	}
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
template<uint16_t descriptor>
const uint8_t (&CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
				::get())[descriptor & 0xF]
{
	enum { index = IndexOf< RxDescriptorList, Int2Type<descriptor> >::value };
	LOKI_STATIC_CHECK (index >= 0, Required_descriptor_cant_be_find_in_RxDescriptorGroupList);
	return (uint8_t (&)[descriptor & 0xF])data[index];
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
template<uint16_t descriptor>
SoftIntHandler& CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
				::rxHandler()
{
	enum { index = IndexOf< RxDescriptorInterruptList, Int2Type<descriptor> >::value };
	LOKI_STATIC_CHECK (index >= 0, Required_descriptor_cant_be_find_in_RxInterruptList);
	return handlerRx[index];
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
template<uint16_t descriptor>
SoftIntHandler& CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
				::txHandler()
{
	enum { index = IndexOf< TxDescriptorInterruptList, Int2Type<descriptor> >::value };
	LOKI_STATIC_CHECK (index >= 0, Required_descriptor_cant_be_find_in_TxInterruptList);
	return handlerTx[index];
}

template <	class TxDescriptorGroupList, class RxDescriptorGroupList,
			class RxDescriptorInterruptList, uint8_t dataBufferSize,
			class TxDescriptorInterruptList,
			long baudKbit, long samplePointPercent>
void CanDat<TxDescriptorGroupList, RxDescriptorGroupList, RxDescriptorInterruptList, dataBufferSize, TxDescriptorInterruptList, baudKbit, samplePointPercent>
		::interruptHandler ()
{
	if ( reg.canGeneralStatus.busOff )
		reboot ();
	reg.canGeneralStatus = 0xFF; // Снимаем флаги. Интерфейся для снятия флага - запись 1.

	uint8_t highestPriorityMob = reg.canHighestPriorityMob;
	if ( highestPriorityMob != 0x0F ) // Если прерывание вызвано MOb'ом
	{
		uint8_t canPageSave = reg.canPage; // чтобы вернуть назад
		reg.canPage = highestPriorityMob;

		Bitfield<CanMobStatus> status = reg.canMobStatus;
		reg.canMobStatus = 0; // Снимаем флаг прерывания, чтобы не войти вновь
		sei (); // После этого можно разрешить прерывания глобально

		if ( status.receiveFinish )
		{
			uint8_t len = reg.canMobControl.dataLength;
			uint16_t descript = reg.canMobId.idA * 0x20 + len;
			uint8_t n = IndexFinder<RxDescriptorList>::index(descript);

			if (n != 255) // Пришедший Descriptor есть в наших списках
			{
				for (uint8_t i = 0; i < len; ++i)
					data[n][i] = reg.canMobData;

				// Добавляем в очередь пользовательский обработчик
				uint8_t ni = IndexFinder<RxDescriptorInterruptList>::index(descript);
				if (ni != 255) // Есть прерывание для этого дескриптора
				{
					// Копирование данных
					if (dataInterruptPointer > dataBufferSize-len)
						dataInterruptPointer = 0;

					uint8_t startPointer = dataInterruptPointer;
					for (uint8_t i = 0; i < len; i ++)
						dataInterrupt[dataInterruptPointer++] = data[n][i];

					dispatcher.add( handlerRx[ni], uint16_t(&dataInterrupt[startPointer]) );
				}
			}

			reg.canMobControl = CanMobControl{ len, 0, 0, CanMobControl::Receive }; // реинициализация
		}
		else if ( status.transmitFinish )
		{
			uint8_t len = reg.canMobControl.dataLength;
			uint16_t descript = reg.canMobId.idA * 0x20 + len;

			uint8_t ni = IndexFinder<TxDescriptorInterruptList>::index(descript);
			if (ni != 255)
				dispatcher.add (handlerTx[ni], 0);
		}

		reg.canPage = canPageSave;
	}
}

#endif /* CANDAT_H_ */
