/** \addtogroup avr */
/*@{*/
/**
 *	\file	avr-halib/avr/timer.h
 *	\brief	Timer classes
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/interrupt.h"
#include "avr-halib/avr/regmaps.h"
#include "avr-halib/share/delegate.h"



// Some methods might be useless for some timers because they do not support such a feature
template <class TimerRegmap>
	class Timer
{
public:
	typedef TimerRegmap Regmap;
	
	void selectClock(enum TimerRegmap::ClockSelect cs)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCS(cs);
	}
	
	void setWaveformGenerationMode(enum TimerRegmap::WaveformGenerationMode wgm)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setWGM(wgm);
	}
	
	void setCompareMatchOutputModeA(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMA(com);
	}
	
	void setCompareMatchOutputModeB(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMB(com);
	}

	void setCompareMatchOutputModeC(enum TimerRegmap::CompareMatchOutputMode com)
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCOMC(com);
	}

	
	void setOutputCompareA(uint8_t oc)
	{
		UseRegmap(timer, TimerRegmap);
		timer.outputCompareA = oc;
	}
	void setOutputCompareB(uint8_t oc)
	{
		UseRegmap(timer, TimerRegmap);
		timer.outputCompareB = oc;
	}
	void setOutputCompareC(uint8_t oc)
	{
		UseRegmap(timer, TimerRegmap);
		timer.outputCompareC = oc;
	}
	
	
	uint8_t getInterruptMask()
	{
		UseRegmap(timer, TimerRegmap);
		return timer.interruptMask;
	}
	void setInterruptMask(uint8_t i)
	{
		UseRegmap(timer, TimerRegmap);
		timer.interruptMask = i;
	}
	
};















// Todo: PWM, unexakteren weniger häufig Interruptenden EggTimer (in 10 Sekunden, uint8_t)
// Todo: aufsplitten in jeweils 1 datei pro komponente
// Todo: EggTimer optimieren für Ausnutzung 16-Bit-Timer (weniger Interrupts)
// Todo: Stoppuhr


/**
 *	\class	ExactEggTimer timer.h "avr-halib/avr/timer.h"
 *	\brief	Calls a function after x milliseconds (high level timer)
 *	\param	TimerRegmap	Register map of the timer to use (e.g. Timer0, Timer1, ...)
 *
 *	\pre	Uses OutputCompareA-Interrupt of the Timer selected by \c TimerRegmap.
 *		Interrupts have to be enabled (<tt>sei()</tt>)
 *
 *	\see	EggTimer
 */
template <class TimerRegmap>
	class ExactEggTimer
{
	// Number of interrupts to wait when using prescaler 1
	uint32_t counter;

	// Select biggest possible prescaler for minimizing number of interrupts
	void nextStep()
	{
		UseRegmap(timer, TimerRegmap);
	
		if (!counter)
		{
			stop();
			onTimerDelegate();
		}
		else
		{
			uint16_t prescaler;
			typename TimerRegmap::ClockSelect prescalerId;
		
			if (counter >= 1024)
			{
				prescaler = 1024;
				prescalerId = TimerRegmap::ps1024;
			}
			else if ((uint16_t)counter >= (uint16_t)256)
			{
				prescaler = 256;
				prescalerId = TimerRegmap::ps256;
			}
			else  if ((uint8_t)counter >= (uint8_t)64)
			{
				prescaler = 64;
				prescalerId = TimerRegmap::ps64;
			}
			else if ((uint8_t)counter >= (uint8_t)8)
			{
				prescaler = 8;
				prescalerId = TimerRegmap::ps8;
			}
			else
			{
				prescaler = 1;
				prescalerId = TimerRegmap::ps1;
			}	
			
			counter -= prescaler;
			timer.setCS(prescalerId);
		}
	}


public:

	/**
	 *	\brief	Function called on timer event
	 */
	Delegate onTimerDelegate;

	/**
	 *	\brief	Starts the egg timer
	 *	\param	ms	Time to pass until \p onTimerDelegate will be called (in milliseconds)
	 *	\pre	\p onTimerDelegate has to be initialized
	 *	\remark	\p onTimerDelegate will be only called once. To call it again restart the timer in your event handler function.
	 */
	void start(uint16_t ms)
	{
		UseRegmap(timer, TimerRegmap);

		// in vielfaches von 256 takten teilen
		// (F_CPU >> 8) / 1000 * 256 Takte pro ms (so viele OI für eine Sek.)
		// rund (F_CPU >> 18) * 256 Takte pro ms
		counter = ms * (F_CPU / 256000);

		timer.interruptMask = TimerRegmap::im_outputCompareAEnable;
		timer.template setOutputCompareAInterrupt< ExactEggTimer<TimerRegmap>, & ExactEggTimer<TimerRegmap>::nextStep > (*this);
		timer.outputCompareA = 0xff;
		timer.setWGM(TimerRegmap::ctc);		// Clear timer on compare match
		nextStep();
	}


	/**
	 *	\brief	Stops the egg timer if it is running
	 */
	void stop()
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCS(TimerRegmap::stop);
	}
};









/**
 *	\class	EggTimer timer.h "avr-halib/avr/timer.h"
 *	\brief	Calls a function after x tenth of seconds  (high level timer)
 *	\param	TimerRegmap	Register map of the timer to use (e.g. Timer0, Timer1, ...)
 *
 *	Less resolution and less code / interrupts than ExactEggTimer, but exact timing in this resolution 
 *	(maximal timing error: 16384 / CPU_FREQ s, e.g. 0.001 s with 16 MHz)
 *
 *	\pre	Uses OutputCompareA-Interrupt of the Timer selected by \c TimerRegmap.
 *		Interrupts have to be enabled (<tt>sei()</tt>)
 *
 *	\see	ExactEggTimer
 */
template <class TimerRegmap>
	class EggTimer
{
	// Number of interrupts to wait when using prescaler 64
	uint16_t counter;

	// Select biggest possible prescaler for minimizing number of interrupts
	void nextStep()
	{
		UseRegmap(timer, TimerRegmap);
	
		if (!counter)
		{
			stop();
			onTimerDelegate();
		}
		else
		{
			// select highest possible prescaler
			uint8_t diff;
			typename TimerRegmap::ClockSelect prescalerId;
		
			if (counter >= 16)
			{
				diff = 16;
				prescalerId = TimerRegmap::ps1024;
			}
			else if ((uint8_t)counter >= (uint8_t)4)
			{
				diff = 4;
				prescalerId = TimerRegmap::ps256;
			}
			else
			{
				diff = 1;
				prescalerId = TimerRegmap::ps64;
			}	
			
			counter -= diff;
			timer.setCS(prescalerId);
		}
	
	}


public:
	/**
	 *	\brief	Function called on timer event
	 */
	Delegate onTimerDelegate;

	/**
	 *	\brief	Starts the egg timer
	 *	\param	ts	Time to pass until \p onTimerDelegate will be called (in tenth of seconds)
	 *	\pre	\p onTimerDelegate has to be initialized
	 *	\remark	\p onTimerDelegate will be only called once. To call it again restart the timer in your event handler function.
	 */
	void start(uint8_t ts)
	{
		UseRegmap(timer, TimerRegmap);

		// geringster verwendeter Prescaler: 64 -> kleinste gemessene Zeiteinheit: 256 * 64 = 16384 Takte
		// in vielfaches von 16384 Takten teilen
		// (F_CPU / 16384) * 16384 Takte pro sek. (so viele OI für eine Sek.)
		// (F_CPU / 163840) * 16384 Takte pro s/10 (so viele OI für eine Sek.)
		// rund (F_CPU >> 18) * 256 Takte pro ms
		counter = ts * (F_CPU / 163840);

		timer.interruptMask = TimerRegmap::im_outputCompareAEnable;
		timer.template setOutputCompareAInterrupt< EggTimer<TimerRegmap>, & EggTimer<TimerRegmap>::nextStep > (*this);
		timer.outputCompareA = 0xff;
		timer.setWGM(TimerRegmap::ctc);		// Clear timer on compare match
		nextStep();
	}

	/**
	 *	\brief	Stops the egg timer if it is running
	 */
	void stop()
	{
		UseRegmap(timer, TimerRegmap);
		timer.setCS(TimerRegmap::stop);
	}

};

/*@}*/
