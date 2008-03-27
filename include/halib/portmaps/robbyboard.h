/* PortMap header for avr-halib
 * generated by avr-pmgen 0.05 from test.portmap
 * 
 * Do not modify this file unless you really know what you are doing! * Changetest.portmap ...
 * 
 */

#include <stdint.h>


struct Button0
{
	enum { pressedLevel = false, usePullup = true };
	volatile bool : 4;
	volatile bool pinButton: 1;		// PINA, bit 4
	volatile bool : 7;
	volatile bool ddrButton: 1;		// DDRA, bit 4
	volatile bool : 7;
	volatile bool portButton: 1;		// PORTA, bit 4
};

struct Button1
{
	enum { pressedLevel = false, usePullup = true };
	volatile bool : 5;
	volatile bool pinButton: 1;		// PINA, bit 5
	volatile bool : 7;
	volatile bool ddrButton: 1;		// DDRA, bit 5
	volatile bool : 7;
	volatile bool portButton: 1;		// PORTA, bit 5
};

struct Button2
{
	enum { pressedLevel = false, usePullup = true };
	volatile bool : 6;
	volatile bool pinButton: 1;		// PINA, bit 6
	volatile bool : 7;
	volatile bool ddrButton: 1;		// DDRA, bit 6
	volatile bool : 7;
	volatile bool portButton: 1;		// PORTA, bit 6
};

struct Button3
{
	enum { pressedLevel = false, usePullup = true };
	volatile bool : 7;
	volatile bool pinButton: 1;		// PINA, bit 7
	volatile bool : 7;
	volatile bool ddrButton: 1;		// DDRA, bit 7
	volatile bool : 7;
	volatile bool portButton: 1;		// PORTA, bit 7
};

struct Led0
{
	enum { onLevel = true };
	volatile bool pinLed: 1;		// PINA, bit 0
	volatile bool : 7;
	volatile bool ddrLed: 1;		// DDRA, bit 0
	volatile bool : 7;
	volatile bool portLed: 1;		// PORTA, bit 0
};

struct Led1
{
	enum { onLevel = true };
	volatile bool : 1;
	volatile bool pinLed: 1;		// PINA, bit 1
	volatile bool : 7;
	volatile bool ddrLed: 1;		// DDRA, bit 1
	volatile bool : 7;
	volatile bool portLed: 1;		// PORTA, bit 1
};

struct Led2
{
	enum { onLevel = true };
	volatile bool : 2;
	volatile bool pinLed: 1;		// PINA, bit 2
	volatile bool : 7;
	volatile bool ddrLed: 1;		// DDRA, bit 2
	volatile bool : 7;
	volatile bool portLed: 1;		// PORTA, bit 2
};

struct Led3
{
	enum { onLevel = true };
	volatile bool : 3;
	volatile bool pinLed: 1;		// PINA, bit 3
	volatile bool : 7;
	volatile bool ddrLed: 1;		// DDRA, bit 3
	volatile bool : 7;
	volatile bool portLed: 1;		// PORTA, bit 3
};

struct LedBlock0123
{
	enum { onLevel = 0xff };
	volatile uint8_t pinLed0123: 4;		// PINA, bit 0-4
	volatile uint8_t : 4;
	volatile uint8_t ddrLed0123: 4;		// DDRA, bit 0-4
	volatile uint8_t : 4;
	volatile uint8_t portLed0123: 4;		// PORTA, bit 0-4

	void setLedsPin(uint8_t i)
	{
		pinLed0123 = i;
	}
	uint8_t getLedsPin()
	{
		return pinLed0123;
	}
	void setLedsDdr(uint8_t i)
	{
		ddrLed0123 = i;
	}
	uint8_t getLedsDdr()
	{
		return ddrLed0123;
	}
	void setLedsPort(uint8_t i)
	{
		portLed0123 = i;
	}
	uint8_t getLedsPort()
	{
		return portLed0123;
	}

};


