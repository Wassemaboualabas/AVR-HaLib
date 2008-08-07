/** \addtogroup share */
/*@{*/
/** 
 *	\file	include/avr-halib/share/syncsensor.h
 *	\brief	Defines SyncSensor
 *	\author Karl Fessel
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

/**
 *	\class	SyncSensor syncsensor.h "avr-halib/share/syncsensor.h"
 *	\brief	Simplifies use of Sensor classes
 *	\param	ReturnType	type of the value returned and the memory it needs
 *	\param	Sensor		Sensor class to use
 *	
 *	\par Pseudo interface to be implemented by Sensors
 *	template<class ReturnType> class Sensor
 *	{
 *	protected:
 *		typedef ReturnType ReturnType;
 *		ReturnType value;
 *	public:
 *		Sensor(){}
 *		
 *		/// returns true if process started by startGetValue() had been completed.
 *		bool isReady(){return true;}
 *	
 *		/// returns true if value-getting-process could be started
 *		bool startGetValue()
 *		{
 *			//here the process of getting the realworld-value to this.value schould be started
 *			return true;
 *		}
 *	
 *		/// returns true if isReady() and writes gotten realworld-value to value
 *		bool getCachedValue(ReturnType &value)
 *		{
 *			if(isReady())
 *			{
 *				value = this.value;
 *				return true;
 *			}
 *			else	
 *				return false;
 *		}
 *	};
 */
template <class SensorClass>
	class SyncSensor:
		public SensorClass
{
	typedef typename SensorClass::ReturnType ReturnType;

public:
	
	ReturnType getValue()
	{
		while (!SensorClass::startGetValue());
		ReturnType value;
		while (!SensorClass::getCachedValue(value));
		return value;
	}
};

#if 0
template <class Sensors>
	class FusionSensor
{
	typedef typename SensorClass::ReturnType ReturnType;
	
	public:Sensor(){}
		
		/// returns true if process started by startGetValue() had been completed.
		bool isReady(){return true;}
	
		/// returns true if value-getting-process could be started
		bool startGetValue()
		{
			//here the process of getting the realworld-value to this.value schould be started
			return true;
		}
	
		/// returns true if isReady() and writes gotten realworld-value to value
		bool getCachedValue(ReturnType &value)
		{
			if(isReady())
			{
				value = this.value;
				return true;
			}
			else	
				return false;
		}
	
	
};
#endif

/*@}*/