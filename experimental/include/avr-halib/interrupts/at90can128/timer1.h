#pragma once

#include <avr-halib/interrupts/interrupt.h>
#include <avr-halib/avr/interrupt.h>

namespace avr_halib
{
namespace interrupts
{
namespace at90can128
{
	struct Timer1IntMap
	{
		/** \brief interrupts defined by this device **/
		enum Interrupts
		{
			capture_Int=27,			/**< input capture**/
			compareMatchA_Int=28,	/**< compare match in unit A **/
			compareMatchB_Int=29,	/**< compare match in unit B **/
			compareMatchC_Int=30,	/**< compare match in unit C **/
			overflow_Int=31			/**< timer overflow **/
		};
	};
}

template<>
struct Interrupt<at90can128::Timer1IntMap>
{
	private:
	typedef at90can128::Timer1IntMap IntMap;
	typedef IntMap::Interrupts Int;
	

	public:
	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER1_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER1_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER1_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, typename T, void (T::*F)(void)>
	static void setInt(const T& obj)
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRM(TIMER1_OVF_vect, F, obj);
					break;
			case(IntMap::capture_Int)       : redirectISRM(TIMER1_CAPT_vect, F, obj);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRM(TIMER1_COMPA_vect, F, obj);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRM(TIMER1_COMPB_vect, F, obj);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRM(TIMER1_COMPC_vect, F, obj);
					break;
		}
	}

	template<Int i, void (*F)(void)>
	static void setInt()
	{
		switch(i)
		{
			case(IntMap::overflow_Int)      : redirectISRF(TIMER1_OVF_vect, F);
					break;
			case(IntMap::capture_Int)       : redirectISRF(TIMER1_CAPT_vect, F);
					break;
			case(IntMap::compareMatchA_Int) : redirectISRF(TIMER1_COMPA_vect, F);
					break;
			case(IntMap::compareMatchB_Int) : redirectISRF(TIMER1_COMPB_vect, F);
					break;
			case(IntMap::compareMatchC_Int) : redirectISRF(TIMER1_COMPC_vect, F);
					break;
		}
	}
};

}
}