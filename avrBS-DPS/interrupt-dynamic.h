/*
 * interrupt-dynamic.h
 *
 *	Динамические обработчики прерываний
 *	***********************************
 *
 *	INTERRUPT_DELEGATE_DECLARE(NAME_without_vect):
 *	- Объявляет делегат с именем NAME_handler для прерывания с именем NAME_vect
 *	- Создаёт фиктивный обработчик NAME_vect, вызывающий соответсвующий делегат
 *
 *	+ Делегаты весьма быстры (~2 машинные команды), но каждый занимает 6 байт
 *
 *	// -- Пример использования --
 *	|	INTERRUPT_DELEGATE_DECLARE (INT0_vect)
 *	|	void f1 () { // some code... }
 *	|	void f2 () { // some code... }
 *	|	//...
 *	|	int main ()
 *	|	{
 *	|		INT0_handler = &f1;
 *	|		// ...
 *	|		INT0_handler = &f2;
 *	|	//...
 *
 *	Примечание: см. как работать с делегатами
 */

#ifndef INTERRUPT_DYNAMIC_H_
#define INTERRUPT_DYNAMIC_H_

#include <avr/io.h>
#include <avr/cpp/Delegate.h>

typedef void (* InterruptHadlerPointer) ();

#define INTERRUPT_DELEGATE_DECLARE(NAME_without_vect)\
		FastDelegate NAME_without_vect ## _handler;\
		ISR (NAME_without_vect ## _vect)\
		{\
			NAME_without_vect ## _handler ();\
		}\

#define INTERRUPT_HACK_DECLARE(NAME_without_vect)\
		InterruptHadlerPointer NAME_without_vect ## _handler;\
		ISR (NAME_without_vect ## _vect)\
		{\
			NAME_without_vect ## _handler ();\
		}\


#endif /* INTERRUPT_DYNAMIC_H_ */
