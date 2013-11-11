/*
 * DiscreteInput.h
 *
 * Ввод дискретных сигналов
 * ************************
 *
 *  Created on: 21.02.2013
 *      Author: nazemnyh
 */

#ifndef DISCRETEINPUT_H_
#define DISCRETEINPUT_H_

#include <cpp/dispatcher.h>
#include <cpp/timers.h>
//#include <cpp/loki/Typelist.h>
//using namespace Loki;
//using namespace Loki::TL;
//

template < typename ClockType, ClockType& clock >
class DiscreteInput
{
public:
	DiscreteInput (bool complementaryComplect, SoftIntHandler pushHandler, SoftIntHandler releaseHandler);
	uint16_t getState () const;
	void setPushHandler (SoftIntHandler pushHandler);
	void setReleaseHandler (SoftIntHandler releaseHandler);

	struct InputsField
	{
		uint16_t	in1		:1;
		uint16_t	in2		:1;
		uint16_t	in3		:1;
		uint16_t	in4		:1;
		uint16_t	in5		:1;
		uint16_t	in6		:1;
		uint16_t	in7		:1;
		uint16_t	in8		:1;
		uint16_t	in9		:1;
		uint16_t	in10	:1;
		uint16_t	in11	:1;
		uint16_t	in12	:1;
		uint16_t			:4;
	};
	typedef Bitfield<InputsField> Inputs;

private:
	void poll (uint16_t );
	void configPorts ();
	Inputs readPorts () const;
	void configLeds ();
	void lightLeds ();

	bool complementaryComplect;
	Inputs state;
	typename ClockType::Time lastTime[16];
	SoftIntHandler pushHandler;
	SoftIntHandler releaseHandler;
};

template < typename ClockType, ClockType& clock >
DiscreteInput<ClockType, clock>::DiscreteInput (bool complementaryComplect, SoftIntHandler pushHandler, SoftIntHandler releaseHandler)
	: complementaryComplect (complementaryComplect), pushHandler (pushHandler), releaseHandler (releaseHandler)
{
	configPorts();
	configLeds();

	state = readPorts();
	auto currentTime = clock.getTime();

	// Вызываем push и release, чтобы передать начальные значения
	uint16_t current = state;
	for (uint8_t i = 0; i < 10; i++)
	{
		if (current & 1)
			pushHandler ( i+1 );
		else
			releaseHandler ( i+1 );

		lastTime[i] = currentTime;

		current /= 2;
	}

	poll (0);
}

template < typename ClockType, ClockType& clock >
uint16_t DiscreteInput<ClockType, clock>::getState() const
{
	return state;
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::setPushHandler(SoftIntHandler pushHandler)
{
	DiscreteInput::pushHandler = pushHandler;
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::setReleaseHandler(SoftIntHandler releaseHandler)
{
	DiscreteInput::releaseHandler = releaseHandler;
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::configPorts ()
{
	reg.portA.pin2.in();
	reg.portA.pin3.in();
	reg.portA.pin4.in();
	reg.portA.pin5.in();
	reg.portA.pin6.in();
	reg.portA.pin7.in();

	reg.portC.pin1.in();
	reg.portC.pin2.in();
	reg.portC.pin3.in();
	reg.portC.pin4.in();
	reg.portC.pin5.in();
	reg.portC.pin6.in();
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::configLeds ()
{
	reg.portE.pin2.out();
	reg.portE.pin3.out();
	reg.portE.pin4.out();
	reg.portE.pin5.out();
	reg.portE.pin6.out();
	reg.portE.pin7.out();

	reg.portB.pin4.out();
	reg.portB.pin5.out();
	reg.portB.pin6.out();
	reg.portB.pin7.out();

	reg.portG.pin3.out();
	reg.portG.pin4.out();
}

template < typename ClockType, ClockType& clock >
typename DiscreteInput<ClockType, clock>::Inputs DiscreteInput<ClockType, clock>::readPorts () const
{
	struct SignificantPinsOfPortA
	{
		uint8_t			:2;
		uint8_t	in1		:1;
		uint8_t	in2		:1;
		uint8_t	in3		:1;
		uint8_t	in4		:1;
		uint8_t	in5		:1;
		uint8_t	in6		:1;
	};
	Bitfield<SignificantPinsOfPortA> inPA (reg.portA);
	struct SignificantPinsOfPortC
	{
		uint8_t			:1;
		uint8_t	in12	:1;
		uint8_t in11	:1;
		uint8_t	in10	:1;
		uint8_t in9		:1;
		uint8_t	in8		:1;
		uint8_t	in7		:1;
		uint8_t			:1;
	};
	Bitfield<SignificantPinsOfPortC> inPC (reg.portC);

	Inputs currentState ( InputsField{  inPA.in1, inPA.in2, inPA.in3, inPA.in4, inPA.in5, inPA.in6,
										inPC.in7, inPC.in8, inPC.in9, inPC.in10, inPC.in11, inPC.in12 } );
	currentState = (uint16_t) currentState ^ 0xFFFF;
	return currentState;
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::lightLeds ()
{
	if (!complementaryComplect)
	{
		reg.portE.pin2 = state.in1;
		reg.portE.pin3 = state.in2;
		reg.portE.pin4 = state.in3;
		reg.portE.pin5 = state.in4;
		reg.portE.pin6 = state.in5;
		reg.portE.pin7 = state.in6;

		reg.portB.pin4 = state.in7;
		reg.portB.pin5 = state.in8;
		reg.portB.pin6 = state.in9;
		reg.portB.pin7 = state.in10;

		reg.portG.pin3 = state.in11;
		reg.portG.pin4 = state.in12;
	}
	else
	{
		reg.portE.pin2 = !state.in1;
		reg.portE.pin3 = !state.in2;
		reg.portE.pin4 = !state.in3;
		reg.portE.pin5 = !state.in4;
		reg.portE.pin6 = !state.in5;
		reg.portE.pin7 = !state.in6;

		reg.portB.pin4 = !state.in7;
		reg.portB.pin5 = !state.in8;
		reg.portB.pin6 = !state.in9;
		reg.portB.pin7 = !state.in10;

		reg.portG.pin3 = !state.in11;
		reg.portG.pin4 = !state.in12;
	}
}

template < typename ClockType, ClockType& clock >
void DiscreteInput<ClockType, clock>::poll (uint16_t )
{
	uint16_t current = readPorts();
	uint16_t last = state;
	auto time = clock.getTime();

	// Проверка, изменилось ли значение.
	// Новое значение должно продержаться в течении 30 мсек, чтобы оно применилось.
	for (uint8_t i = 0; i < 12; i++)
	{
		if ( (current & 1) != (last & 1) )
		{
			if ( time - lastTime[i] > 30 )
			{
				if (current & 1)
				{
					state = state | (1 << i);
					pushHandler ( i+1 );
				}
				else
				{
					state = state & ~(1 << i);
					releaseHandler ( i+1 );
				}

				lastTime[i] = time;
			}
		}
		else
		{
			lastTime[i] = time;
		}

		current /= 2;
		last /= 2;
	}

	lightLeds();

	dispatcher.add ( SoftIntHandler::from_method<DiscreteInput, &DiscreteInput::poll>(this), 0 );
}


#endif /* DISCRETEINPUT_H_ */
