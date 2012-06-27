#pragma once

#include <avr-halib/config/timer.h>
#include <avr-halib/interrupts/at90can128/timer0.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace at90can128
{

	/** \brief Regmap for asynchronous timer 0 **/
	struct Timer0 : public base::LocalRegMap
	{
		template< PrescalerType ps>
		struct PrescalerToClockSelect
		{
			static const uint8_t cs = (ps == Prescalers::noClock) ? 0x0:
										(ps == Prescalers::ps1    ) ? 0x1:
										  (ps == Prescalers::ps8    ) ? 0x2:
											(ps == Prescalers::ps64   ) ? 0x3:
											  (ps == Prescalers::ps256  ) ? 0x4: 0x5;
		};

		template< ExternalClockEdgeType edgeType >
		struct ExternalClockEdgeToClockSelect
		{
			static const uint8_t cs = (edgeType == ExternalClockEdges::falling) ? 0x6 : 0x7;
		};

		template< OutputModeType om, WaveFormType wf  >
		struct OutputModeTypeToOCM
		{
			static const uint8_t ocm = (om == OutputModes::noOutput ) ? 0x0:
										 (om == OutputModes::toggle   ) ? 0x1:
										   (om == OutputModes::clear    ) ? 0x2: 0x3;
		};

		struct NormalToWGM
		{
			static const uint8_t wgm0 = 0x0;
			static const uint8_t wgm1 = 0x0;
		};

		template< MatchInputType m >
		struct ClearMatchToWGM
		{
			static const uint8_t wgm0 = 0x0;
			static const uint8_t wgm1 = 0x1;
		};


		template< PWMCorrectionType c, PWMAccuracyType a, MatchInputType m >
		struct PWMConfigToWGM
		{
			static const uint8_t wgm0 = 0x1;
			static const uint8_t wgm1 = (c == PWMCorrections::phaseCorrect) ? 0x0 : 0x1;
		};

		struct PrescalerIterator
		{
			static const PrescalerType ps = ps1;
			static bool hasNext = true;
			struct next
			{
				static const PrescalerType ps = ps8;
				static bool hasNext = true;
				struct next
				{
					static const PrescalerType ps = ps64;
					static bool hasNext = true;
					struct next
					{
						static const PrescalerType ps = ps256;
						static bool hasNext = true;
						struct next
						{
							static const PrescalerType ps = ps1024;
							static bool hasNext = false;
						};
					};
				};
			};
		};

		enum Parameters
		{
			rtc = false,
			numOCUs = 1,
		};
		
		typedef interrupts::at90can128::Timer0IntMap InterruptMap;
		
		typedef uint8_t ValueType;
		
		union
		{
			struct
			{
				uint8_t __base[0x24];
				uint8_t     : 7;
				uint8_t ocm : numOCUs;
			};
			struct
			{
				uint8_t __pad0[0x35];
				union
				{
					struct
					{
						uint8_t tov : 1;
						uint8_t ocf : numOCUs;
					};
					uint8_t tifr;
				};
			};
			struct
			{
				uint8_t __pad1[0x6e];
				union
				{
					struct
					{
						uint8_t toie : 1;
						uint8_t ocie : numOCUs;
					};
					uint8_t timsk;
				};
			};
			struct
			{
				uint8_t __pad2[0x44];
				union
				{
					struct
					{
						uint8_t cs   : 3;
						uint8_t wgm0 : 1;
						uint8_t coma : 2;
						uint8_t wgm1 : 1;
						uint8_t foc  : numOCUs;
					};
					struct
					{
						uint8_t tccra;
					};
					uint16_t tccr;
				};
			};
			struct
			{
				uint8_t __pad3[0x46];
				uint8_t tcnt;
			};
			struct
			{
				uint8_t __pad4[0x47];
				uint8_t ocr[numOCUs];
			};
		};
	}__attribute__((packed));
}
}
}
}
