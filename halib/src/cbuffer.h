#pragma once

#include "cdevice.h"
#include "queuebuffer.h"

/*! \brief Zeichenzwischenspeicher
 * 
 * 	Eingaben werden in einen Speicher definierter Gr��e geschrieben und bei Anforderung wieder ausgegeben
 * \param length_t	Typ f�r die Indexierung / Gr��e des Puffer
 * \param bufLen	Gr��e des Puffers
 *
 */


template <class length_t, length_t bufLen> class CBuffer : public CDevice
{
protected:

  QueueBuffer<char, length_t, bufLen> buffer;
	
public:
	///\brief Konstrucktor
	CBuffer(){}
	/**
	 * putc Schiebt ein Zeichen in den Puffer
	 */
	void putc (const char c)
	{
		buffer.put(c);
	}

	/**
	 * getc Liest ein Zeichen aus dem Puffer 
	 */ 
	char getc()
	{
		return buffer.get();
	}

};
