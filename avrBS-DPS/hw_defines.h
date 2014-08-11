/*
 * hw_defines.h
 *
 *  Логические имена для физических портов
 *  **************************************
 *
 */

#ifndef HW_DEFINES_H_
#define HW_DEFINES_H_

#include <cpp/io.h>

void configSelfComplectPin ()
{
	reg.portB.pin7.in();
}

bool isSelfComplectA ()
{
	return (reg.portB.pin7 == 0);
}

inline void lconfig ()
{
	reg.portC.pin4.out ();
	reg.portC.pin5.out ();
}

template <uint8_t n>
inline void ldown ()
{
	reg.portC.pin<n+4>() = true;
//	switch (n)
//	{
//		case 0:
//			portC.pin4 = true;
//			break;
//		case 1:
//			portC.pin5 = true;
//			break;
//	}
}

template <uint8_t n>
inline void lup ()
{
	reg.portC.pin<n+4>() = false;
}

template <uint8_t n>
inline void ltoggle ()
{
//	PORTC ^= (1 << (n+4));
	reg.portC.pin<n+4>().toggle ();
//	switch (n)
//	{
//		case 0:
//			portC.pin4.toggle();
//			break;
//		case 1:
//			portC.pin5.toggle();
//			break;
//	}
}

template <uint8_t n>
inline void lset (bool on)
{
	reg.portC.pin<n+4>() = !on;
}

//void lam (uint8_t n) __attribute__ ((noinline));
template <uint8_t n, uint16_t repeat>
inline void lam ()
{
	static uint16_t count[2];
	count[n] ++;
	if (count[n] == repeat)
	{
		ltoggle<n> ();
		count[n] = 0;
	}
}



#endif /* HW_DEFINES_H_ */
