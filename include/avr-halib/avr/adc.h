/** \addtogroup avr */
/*@{*/
/**
 *	\file	include/avr-halib/avr/adc.h
 *	\brief	Defines AnalogDigitalConverter
 *
 *	\warning	TODO: Problem mit Nebenläufigkeit beschreiben, Lösung: entweder bei interrrupt oder im normalen programmablaufs
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include<avr/io.h>

#include "avr-halib/avr/regmaps.h"


template < class ADC_Regmap >
	class AnalogDigitalConverterCommon
{
	protected:
	
	template< class Type, class ADC_Regmap_N >
		class helper
	{
		public:
		static void set_adlra();
		static void write_target(Type &target);
	};
	
	template< class ADC_Regmap_N >
		class helper< uint8_t, ADC_Regmap_N >
	{	
		public:
		static void set_adlra(){UseRegmap(rm, ADC_Regmap_N);rm.adlra = 1;}
		static void write_target(uint8_t &target) { UseRegmap(rm, ADC_Regmap_N); SyncRegmap(rm); target = rm.adch; }
	};
	
	template<class ADC_Regmap_N>
		class helper< uint16_t, ADC_Regmap_N >
	{
		public:
		static void set_adlra(){UseRegmap(rm, ADC_Regmap_N);rm.adlra = 0;}
		static void write_target(uint16_t &target) { UseRegmap(rm, ADC_Regmap_N); SyncRegmap(rm); target = rm.adc; }
	};
	
};

template < class Return_Type, class ADC_Regmap >
	class AnalogDigitalConverter:
		public AnalogDigitalConverterCommon< ADC_Regmap >
{

public:	
	
	AnalogDigitalConverter()
	{}
		
	inline bool getValue(Return_Type &target, uint8_t mux, uint8_t reference, uint8_t prescaler = (ADC_Regmap::recommendedPrescalar))
	{
		UseRegmap(rm, ADC_Regmap);
		
		if (rm.adsc)
			return false;
		
		rm.refs = reference;
		
		AnalogDigitalConverterCommon<ADC_Regmap>::template helper<Return_Type, ADC_Regmap>::set_adlra();	// AD-Kanal, Ausgabe 
		
		rm.mux = mux;			// AD-Kanal,  Vergleichsspannung
		rm.aden = true;
		rm.adps = prescaler;
		rm.adie = false;
		rm.adate = false;
		rm.adif = false;
		rm.adsc = true;			// Starte AD-Wandlung ADSC, Interupt disable|(1 << ADIE)
		
				
		while(rm.adsc)
			SyncRegmap(rm);
		
		AnalogDigitalConverterCommon<ADC_Regmap>::template helper<Return_Type, ADC_Regmap>::write_target(target);
		
		return true;
	}
	//bool isThatTarget(Return_Type &target){return false;}
};



 template < class Return_Type, class ADC_Regmap >
	class AnalogDigitalConverterInterrupt:
			public AnalogDigitalConverterCommon< ADC_Regmap >
{
        /// Zeiger, der auf Wert-Puffer des gerade abgefragten Sensors zeigt. Null, wenn im Moment keine AD-Wandlung im Gange ist.
	Return_Type * target;

public:	

	AnalogDigitalConverterInterrupt() :target(0)
	{}
	
	bool init(){
		ADC_Regmap::template setADCInterrupt< AnalogDigitalConverterInterrupt< Return_Type, ADC_Regmap >, &AnalogDigitalConverterInterrupt< Return_Type, ADC_Regmap >::onConversionComplete > (*this);
		return true;
		}
	
	bool getValue(Return_Type &target, uint8_t mux,uint8_t reference ,uint8_t prescaler = (ADC_Regmap::recommendedPrescalar))
	{
		UseRegmap(rm, ADC_Regmap);
		
		if (rm.adsc)
			return false;
		
		
		if (this->target != 0)
			return false;	// AD-Wandlung im Gange
		
		this->target = &target;	
				
		rm.refs = reference;
		
		AnalogDigitalConverterCommon< ADC_Regmap >::template helper< Return_Type,ADC_Regmap >::set_adlra();// AD-Kanal, Ausgabe 
		
		rm.mux = mux;			// AD-Kanal,  Vergleichsspannung
		rm.aden = true;
		rm.adps = prescaler;
		rm.adie = true;			//Interupt enable|(1 << ADIE)
		rm.adsc = true;			// Starte AD-Wandlung ADSC, 
		
		
		return true;
	}
	
	bool isThatTarget(Return_Type &target){
		//make the compiler belive that this->target can change, makes it really volatile
		SyncObj(this->target);
		return &target==this->target;
	}
	
	void onConversionComplete()
	{
		AnalogDigitalConverterCommon< ADC_Regmap >::template helper< Return_Type, ADC_Regmap >::write_target(*target);
		this->target = 0;
	}
	
};

/*@}*/