#pragma once

#include "generalTimer.h"
#include <avr-halib/interrupts/at90can128/timer3.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace at90can128 
{
	namespace timerDescription
	{
		struct Timer3
		{
			typedef interrupts::at90can128::Timer3IntMap InterruptMap;

			enum Addresses
			{
				ocmOutput=0x2D,
				ocmOffset=3,
				tifr=0x38,
				timsk=0x71,
				tccr=0x90,
				tcnt=0x94,
				icr=0x96,
				ocr=0x98,
			};
		};
	}
		
	typedef generalTimer::16Bit< timerDescriptions::Timer3 > Timer3;
}
}
}
}

