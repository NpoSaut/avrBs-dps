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
	reg.portA.pin0.in();
}

bool isSelfComplectA ()
{
	return (reg.portA.pin0 == 0);
}

inline void lconfig ()
{

}

template <uint8_t n>
inline void ldown ()
{
//	reg.portC.pin<n+4>() = true;
}

template <uint8_t n>
inline void lup ()
{
//	reg.portC.pin<n+4>() = false;
}

template <uint8_t n>
inline void ltoggle ()
{
//	reg.portC.pin<n+4>().toggle ();
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
