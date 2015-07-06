#ifndef __STACK_H__
#define __STACK_H__

#include "stdafx.h"

template<typename STACKTYPE>

class CLSCStack
{
public:
	CLSCStack ();
	~CLSCStack () {}

	void CreateStack(unsigned int nSize);
	void InitStack(STACKTYPE nData);
	void DeleteStack(void);
	void DataPush(STACKTYPE nData);
	STACKTYPE DataPop();
	STACKTYPE DataGet();
	STACKTYPE DataGet(unsigned int nNumber);
	void DataSet(STACKTYPE nData);
	void DataSet(unsigned int nNumber, STACKTYPE nData);
	int NumberGet();
	bool IsEmpty();
	bool IsFull();

private:
	unsigned int m_nStackUnit;
	unsigned int m_nMaxSize;
	void * m_pvStack;
	void * m_pvStackTop;
};

#include "Stack.cpp"
#endif