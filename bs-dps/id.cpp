/*
 * id.cpp
 *
 *	Храним ID во флеше
 *	******************
 *
 */

#include "id.h"



// ----------------------------------ATTENTION--------------------------------------
// .id Section must be placed by linker to address 0x0100.
// We modify avr/lib/ldscripts/avr5.x to allow place .id between .vectors and .init0
// It normaly use --script-file option to linker instead it, but it work bad
// New file store at name "ld-script.x"
// ---------------------------------------------------------------------------------
