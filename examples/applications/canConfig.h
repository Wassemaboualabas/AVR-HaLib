/** \ingroup canary **/
/** \@{ **/
#pragma once

#define F_CPU 16000000ULL

#include <avr-halib/avr/canary/can_default_configs.h>
#include <avr-halib/avr/canary/canary.h>

#include <avr-halib/ext/lcm_16x4.h>
#include <avr-halib/share/coutdevice.h>
#include <avr-halib/portmaps/lcd_hd44780.h>
#include <avr-halib/avr/interrupt.h>

/**Declares, that we want to use the CAN interrupt of the at90canxxx**/
UseInterrupt(SIG_CAN_INTERRUPT1);

using namespace avr_halib::canary;

typedef COutDevice<Lcm_16x4<LcdHd44780Board> > LCD;

struct CANConfig : defaultCANConfig
{
	enum
	{
		version=CAN_20B,
		prescaler=50,
		maxConcurrentMsgs=4,
	};
};

struct SendConfig : CANConfig
{
	enum
	{
		useReceive=false,
		useError=false
	};
};

struct RecvConfig : CANConfig
{
	enum
	{
		useTransmit=false,
		useTimestamp=true
	};
};

struct RTRConfig : CANConfig
{
	enum { useError=false };
};
/** \@} **/
