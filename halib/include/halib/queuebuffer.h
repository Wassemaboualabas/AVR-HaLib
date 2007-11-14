/**
 *	\file halib/queuebuffer.h
 *	\brief	Implementiert eine FIFO-Speicher fester L�nge.
 *
 */


#pragma once

/*!	\brief  Queue mit fester Maximal-Gr�$szlig;e
*	\param	Data_Type	Elementtyp der Queue
*	\param	Length_Type	Indexierungs- und L�ngentyp
*	\param	Length	Maximale Anzahl speicherbarer Elemente
*
*/
template <class Data_Type, class Length_Type, Length_Type Length>
		class QueueBuffer
{
	private:
		Data_Type bu[Length];
		Length_Type poss;
		Length_Type pose;

	public:
		/// Konstruktor
		QueueBuffer() :
			poss(0), pose(0)
		{}

		/// Entnimmt ein Element aus der Queue
		Data_Type get();
		
		/// F�gt ein Element in die Queue ein
		void put(Data_Type);
		
		/// L�scht den Inhalt der Queue
		void clear()
		{
			poss = pose;
		}
		
		/// Gibt true zur�ck, wenn die Queue leer ist.
		bool isEmpty()
		{
			return poss == pose;
		}
		
		/// Gibt true zur�ck, wenn die Queue voll ist.
		bool isFull()
		{
			return (pose + 1) % Length == poss;
		}
		
		/// Gibt Anzahl der Elemente zur�ck
		Length_Type count()
		{
			return ((pose + Length) - poss) % Length;
		}
};


template <class Data_Type, class Length_Type, Length_Type Length>
		void QueueBuffer<Data_Type,Length_Type,Length>::put(Data_Type c)
{
	if ((pose + 1) % Length == poss)
	{
		/*#ifndef OLD_BEHAVIOUR*/
		poss++;
		poss %=Length;
/*#else
		return;
#endif*/
	}
	bu[pose]=c;
	pose++;
	pose %=Length;
}

template <class Data_Type, class Length_Type, Length_Type Length>
		Data_Type QueueBuffer<Data_Type,Length_Type,Length>::get()
{
	if(pose-poss!=0){
		Data_Type out = bu[poss];
		poss++;
		poss %=Length;
		return out;
	}
	else return 0;
}


