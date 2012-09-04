#pragma once

#include <avr-halib/regmaps/regmaps.h>
#include <avr-halib/config/spi.h>
#include <avr-halib/common/delegate.h>
#include <avr-halib/common/singleton.h>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>

namespace avr_halib{
namespace driver
{
    using ::Interrupt::Binding;
    using ::Interrupt::Slot;
    

    /**\brief Driver for Serial Peripheral Intece
     *
     *	This driver uses the specific SPI hardware of different AVR chips to
     *	enable SPI-based communication with other ICs. It currently only
     *	supports Master-Mode, which is the initiator and clock generator of
     *	every SPI communication.
     *
     **/
    struct Spi : public config::Spi
    {
        /** \brief Default Configuration **/
        struct DefaultConfig
        {
                static const bool              useInterrupt = false;
                static const BitOrderType      bitOrder     = BitOrders::mostFirst;
                static const ClockPolarityType polarity     = ClockPolarities::idleOnHigh;
                static const SampleEdgeType    sampleEdge   = SampleEdges::leading;
                static const PrescalerType     prescaler    = 4;
                static const ModeType          mode         = Modes::master;

                typedef avr_halib::regmaps::local::Spi RegMap;
        };

        /** \brief Configuration meta function
         *  \tparam Config Configuration description to use, defaults to \see DefaultConfig
         **/
        template <typename Config = DefaultConfig>	
        class configure
        {
            /** \brief Configured Driver **/
            struct Core
            {
                protected:
                    typedef typename Config::RegMap RegMap;
                    bool busy;

                public:
                
                /** Construct driver and initialize hardware **/
                Core()
                {
                    reset();
                }
                
                /** Reset and initialize hardware**/
                void reset()
                {
                    typedef typename RegMap::template ClockConfig   < Config::prescaler,
                                                                      Config::polarity,
                                                                      Config::sampleEdge > Clock;
                    typedef typename RegMap::template BitOrderConfig< Config::bitOrder   > BitOrder;
                    typedef typename RegMap::template ModeConfig    < Config::mode       > Mode;


                    UseRegMap(rm, RegMap);
                    
                    rm.mosi.ddr  = true;
                    rm.miso.ddr  = false;
                    rm.sck.ddr   = true;
                    rm.sck.port  = Clock::cpol;
                    rm.ss.ddr    = true;
                    
                    SyncRegMap(rm);

                    rm.mstr  = Mode::mstr;
                    rm.spe   = true;
                    rm.spie  = false;
                    rm.dord  = BitOrder::dord;
                    rm.cpol  = Clock::cpol;
                    rm.cpha  = Clock::cpha;;
                    rm.spr   = Clock::spr;
                    rm.spi2x = Clock::spi2x;
                    
                
                    // Reset Flags
                    
                    SyncRegMap(rm);

                    rm.spie  = Config::useInterrupt;
                    SyncRegMap(rm);

                    busy=false;
                }
                
                
                
                /** \brief Transmit one byte to the currently active slave
                 *  \param data the byte to be transmitted
                 **/
                void put(const uint8_t c)
                {
                    UseRegMap(rm, RegMap);
                    rm.spdr = c;
                    SyncRegMap(rm);
                    busy = true;
                }
                
                /**	\brief	Test for ongoing operations
                 *	\return	true if none, false otherwise
                 **/	
                bool ready()
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    if(busy)
                        if(rm.spif)
                        {
                            busy=false;
                            return true;
                        }
                        else
                            return false;
                    else
                        return true;
                }

                /**	\brief	Receive one byte from the currently active slave
                 *	\param	data Reference to be filled with received data
                 */
                void get(uint8_t& c)
                {
                    UseRegMap(rm, RegMap);
                    SyncRegMap(rm);
                    c = rm.spdr;
                }
            };

            /** \brief Interrupt-Feature extension of core driver **/
            struct InterruptExtension : public Core
            {
                public:
                    typedef Delegate<void> CallbackType;
                    typedef avr_halib::object::Singleton< InterruptExtension > Singleton;
                    Delegate<void> callback;

                private:

                    static void operationComplete()
                    {
                        Singleton& base = Singleton::getInstance();
                        base.busy=false;
                        base.callback();
                    }

                    typedef Slot< Config::RegMap::InterruptMap::operationComplete,
                                           Binding::FixedPlainFunction > IntSlot;
                    
                    typedef typename IntSlot::template Bind< &InterruptExtension::operationComplete > BoundInt;

                public:
                    typedef typename boost::mpl::vector< BoundInt >::type InterruptSlotList;
            };

            struct CoreProxy
            {
                private:
                    typedef typename avr_halib::object::Singleton<Core>::type Base;

                public:
                    typedef Config config;
                    typedef typename boost::mpl::vector<>::type InterruptSlotList;

                    void put(const uint8_t value)
                    {
                        Base::getInstance().put(value);
                    }

                    bool ready()
                    {
                        return Base::getInstance().ready();
                    }

                    void get(uint8_t& value)
                    {
                        Base::getInstance().get(value);
                    }
            };

            struct InterruptProxy
            {
                private:
                    typedef typename InterruptExtension::Singleton Base;

                public:
                    typedef Config config;
                    typedef typename Base::CallbackType CallbackType;
                    typedef typename Base::InterruptSlotList InterruptSlotList;

                    void put(const uint8_t value)
                    {
                        Base::getInstance().put(value);
                    }

                    bool ready()
                    {
                        return Base::getInstance().ready();
                    }

                    void get(uint8_t& value)
                    {
                        Base::getInstance().get(value);
                    }

                    const CallbackType& getCallback() const
                    {
                        return Base::getInstance().callback;
                    }

                    void setCallback(const CallbackType& cb)
                    {
                        Base::getInstance().callback=cb;
                    }
            };

            public:
                /** \brief Interrupt-Feature Selector **/
                typedef typename boost::mpl::if_c< Config::useInterrupt,
                                                   InterruptProxy,
                                                   CoreProxy >::type type;
        };
    };
}
}
