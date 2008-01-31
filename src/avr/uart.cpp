/*!
 *	\file halib/uart.cpp
 *	\brief	Implementiert Uart-Schnittstellen-Klasse
 *
 *	... indem es die Implementierung für die passende Plattform included.
 *
 *	\see halib/atmega32/uart.cpp
 */


#include "halib/config.h"
#include "halib/uart.h"


#if defined (__AVR_ATmega32__)
#	include "./atmega32/uart.cpp"
#elif defined (__AVR_AT90CAN128__)
#	include "./at90can128/uart.cpp"
#else
#	error "device type not defined or no uart implemented for this device"
#endif

