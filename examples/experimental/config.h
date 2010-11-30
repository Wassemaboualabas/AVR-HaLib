#pragma once

#include "logConf.h"

#include <avr-halib/avr/regmaps.h>
#include <avr-halib/portmaps/robbyboard.h>

#include <avr-halib/share/freq.h>
#include <avr-halib/regmaps/remote/srf10.h>
#include <avr-halib/regmaps/local.h>

//Configuration helper for ADC
using avr_halib::config::Frequency;

using avr_halib::regmaps::remote::SRF10RegMap;

//Platform dependant ADC RegMap
using avr_halib::regmaps::local::ADCRegMap;

typedef Frequency< F_CPU > CPUClock;
typedef ADCRegMap< CPUClock > ConfiguredADCRegMap;
typedef SRF10RegMap< TWI< CPUClock > > ConfiguredSRF10RegMap;
typedef SensorPowerSupply PowerPortMap;

