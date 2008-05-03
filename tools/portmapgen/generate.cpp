
string toupper(string s)
{
	for (int i = s.length() - 1; i >= 0; i--)
		s[i] = toupper(s[i]);
	return s;
}

void PortmapEntry::generateCpp(ostream & stream, PinType pt)
{
	static const char * pts [] = { "", "pin", "port", "ddr" };

	int pinCount = getPinCount();
	string dataType;
	string pinType = pts[pt];
	
	if (pinCount == 1)
		dataType = "bool";
	else if (pinCount <= 8)
		dataType = "uint8_t";
	else if (pinCount <= 16)
		dataType = "uint16_t";
	else if (pinCount <= 32)
		dataType = "uint32_t";
	else
		dataType = "uint64_t";

	stream << "\tvolatile " << dataType << " " << pinType << identifier << ": " << pinCount << ";\t\t// " << toupper(pinType) << (char)toupper(port) << ", bit " << firstPin;

	if (lastPin != firstPin)
		stream << "-" << lastPin;

	stream << "\n";
}

void PortMap::generateCpp(ostream & stream)
{
	Generate a map of pins

	int bits = 3 * options.countOfPorts * options.countOfPins;

	PortMapEntry::PinType * pinType = new PortMapEntry::PinType [bits];	// Type of pin: PIN, DDR, PORT ('p')
	PortMapEntry ** pinData = new PortMapEntry* [bits];			// Data of pin
	
	for (int i = 0; i < bits; i++)
	{
		pinType[i] = PortMapEntry::NONE;
		pinData[i] = 0;
	}

	
	Generate map
	map<string, PortMapEntry *>::const_iterator it = entries.begin();
	for (; it != entries.end(); it++)
	{
		Generate 3 entries per pin/pin group (pin, ddr, port)
		PortMapEntry * p = it->second;
		int port = p->getPort() - 'a';
		int pin = p->getFirstPin();

		int pinIdx = port * 3 * options.countOfPins + pin;
		pinType[pinIdx] = PortMapEntry::PIN;
		pinData[pinIdx] = p;
		pinIdx += options.countOfPins;
		pinType[pinIdx] = PortMapEntry::DDR;
		pinData[pinIdx] = p;
		pinIdx += options.countOfPins;
		pinType[pinIdx] = PortMapEntry::PORT;
		pinData[pinIdx] = p;
	}


	stream	<< "struct " << identifier << "\n"
		<< "{\n";

	
	int lastPinSeqNum = 0;		// absolute number of the first pin nothing was written about
	for (int i = 0; i < bits; )
	{
		static const char * pts [] = { "PIN", "DDR", "PORT" };
		char port = 'A' + i / (3 * options.countOfPins);
		int pin = i % options.countOfPins;
		int type = (i / options.countOfPins) % 3;

		switch (options.outputLength)
		{
			case 1:
				Generate readable, but long output
				if (pinType[i] == PortMapEntry::NONE)
				{
					
					stream << "\tvolatile bool : 1;\t\t// " << pts[type] << port << ", bit " << pin << "\n";
					i++;
				}
				else
				{
					pinData[i]->generateCpp(stream, pinType[i]);
					i += pinData[i]->getPinCount();
				}
				break;
			default:
				Generate short output
				if (pinType[i] != PortMapEntry::NONE)
				{
					int d = i - lastPinSeqNum;
					if (d)
					{
						Fill the gap from last named pin to this pin
						stream << "\tvolatile bool : " << d << ";\n";
						lastPinSeqNum = i;
					}

					pinData[i]->generateCpp(stream, pinType[i]);

					lastPinSeqNum += pinData[i]->getPinCount();
					i += pinData[i]->getPinCount();
				}
				else
				{
					i++;
				}
				break;
		}
	}
	
	stream << "};\n\n";
	
	delete [] pinData;
	delete [] pinType;
}
 
void generate()
{

// 	{
// 		Generate header file
// 		ofstream oFile();
// 		ostream & oFile = cout;
// 
// 		oFile	<<	"/* Portmap header for avr-halib\n"
// 				" * generated by " << VERSION << " from " << options.iFilename << "\n"
// 				" * \n"
// 				" * Do not modify this file unless you really know what you are doing!"
// 				" * Change" << options.iFilename << " ...\n"
// 				" * \n"
//  				" */\n\n"
// 				"#include <stdint.h>\n\n";
// 
// 		for (map<string, Portmap *>::iterator it = Portmaps.begin(); it != Portmaps.end(); it++)
// 			it->second->generateCpp(oFile);
// 
// 		oFile	<<	"\n\n" << flush;
// 	}
}
