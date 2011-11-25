/*
 * buffer-queue.h
 *
 *  Буфер в виде очереди
 *  ********************
 *
 *  - Представляет собой очередь объектов типа T длины Size
 *  - Предполагается, что клас будет базовым для устройства, нуждающегося в буфере,
 *    а работа с очередью со стороны устройства будет производится в виртуальных функциях
 *    popFunction () и pushFunction ()
 *  1. При использовании в качестве буфера записи
 *     используется связка push / popIm
 * 		1.1 bool popIm (T& data)
 *  		- Работает со стороны устройства
 *  		- При наличии элемента в очереди извлекает его в data и возвращает true
 *  		- При пустой очереди возаращает false и выходит
 *  		  Элемент будет извлечён как только он поступит путём вызова popFunction ()
 * 		1.2 void push (T data)
 * 			- Работает с внешней стороны (интерфейс) (с помощью него принемаются данные)
 * 			- Дожидается пока в очереди появится место и ставит в неё элемент
 * 	2. При использовании в качестве буфера чтения
 * 	   используется связка pushIm / pop
 *  	2.1 bool pushIm (T data)
 *  		- Работает со стороны устройства
 *  		- При наличии свободного места в очереди добавляет элемент в очередь и возвращает true
 *  		- При полной очереди возаращает false и выходит
 *  		  Элемент будет добавлен как только он поступит путём вызова pushFunction ()
 *  	2.2 T pop ()
 *  		- Работает с внешней стороны (интерфейс) (с помощью него отдаются данные)
 *  		- Дожидается данных в очереди и извлекает первый элемент
 *
 */

#ifndef BUFFER_QUEUE_H_
#define BUFFER_QUEUE_H_

#include <util/atomic.h>
#include "universal.h"

template<class T, uint8_t Size> class Queue
{
public:
	inline void push	 (T data)										// Добавляет элемент. Если очередь заполнена, то дожидается освобождения
	{
		while (true)
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (!isFull ())
				{
					queue[Last] = data;
					Increase (last);
					fill ++;
					break;
				}
			}
			asm volatile ("nop");
		}
		if (need.pop)
		{
			need.pop = false;
			popFunction ();
		}
	}
	inline bool pushIm (T data)											// Пытается добавить элемент. При полной очереди: возвращает false, будет вызвана pushFunction при освобождении
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (isFull ())
			{
				need.push = true;
				return false;
			}

			queue[Last] = data;

			increase (last);
			fill ++;
			return true;
		}
	}
	inline T	pop ()													// Берёт элемент. Если очередь пуста, то дожидается прихода элемента.
	{
		while (true)
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (!isEmpty ())
				{
					T data = queue[first];
					increase (first);
					fill -= 1;
					break;
				}
			}
			asm volatile ("nop");
		}
		if (need.push)
		{
			need.push = false;
			pushFunction ();
		}
		return data;
	}
	inline bool popIm  (T& data)										// Пытается взять элемент. При пустой очереди: возвращает false, будет вызвана popFunction при освобождении
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (isEmpty ())
			{
				need.pop = true;
				return false;
			}

			data = queue[First];
			increase (first);
			fill -= 1;
			return true;
		}
	}

	virtual void popFunction () { };
	virtual void pushFunction () { };

	inline bool isEmpty () const { return fill == 0; }
	inline void waitEmpty () const { while (!IsEmpty ()); }
	inline bool isFull () const { return fill == Size; }
	inline void waitFree () const { while (IsFull ()); }

protected:
	volatile T queue[size];
	volatile struct
	{
		volatile bool push : 1;
		volatile bool pop  : 1;
	} need;
	volatile uint8_t first, last, fill;
	inline void increase (volatile uint8_t& pointer)
	{
		if (pointer < size)
			pointer ++;
		else
			pointer = 0;
	}
};



#endif /* BUFFER_QUEUE_H_ */
