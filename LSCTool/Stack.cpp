
template<typename STACKTYPE>
CLSCStack<STACKTYPE>::CLSCStack()
{
	m_nStackUnit = 4; 
	m_nMaxSize = 0; 
	m_pvStack = NULL; 
	m_pvStackTop = NULL;
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::InitStack(STACKTYPE nData)
{
	if(m_pvStack != NULL)
	{
		unsigned int nIndex = 0;
		for(nIndex = 0; nIndex < m_nMaxSize; nIndex++)
		{
			((STACKTYPE *)m_pvStack)[nIndex] = nData;
		}
	}
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::CreateStack(unsigned int nSize)
{
	unsigned int nIndex = 0;
	m_pvStack = new STACKTYPE [nSize];
	m_nMaxSize = nSize;
	m_pvStackTop = m_pvStack;
	m_nStackUnit = sizeof(STACKTYPE);

	for(nIndex = 0; nIndex < nSize; nIndex++)
	{
		((STACKTYPE *)m_pvStack)[nIndex] = 0;
	}
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::DeleteStack(void)
{
	if(m_pvStack != NULL)
		delete []m_pvStack;
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::DataPush(STACKTYPE nData)
{
	if(m_pvStackTop != NULL)
	{
		if(!IsFull())
		{
			*(STACKTYPE *)m_pvStackTop = nData;
			m_pvStackTop = (unsigned char *)m_pvStackTop + m_nStackUnit;
		}
	}
}

template<typename STACKTYPE>
STACKTYPE CLSCStack<STACKTYPE>::DataPop(void)
{
	STACKTYPE nData = 0;
	if(m_pvStackTop != NULL)
	{
		if(!IsEmpty())
		{
			m_pvStackTop = (unsigned char *)m_pvStackTop - m_nStackUnit;
			nData = *(STACKTYPE *)m_pvStackTop;
		}
		else
		{
			nData = *(STACKTYPE *)m_pvStack;
		}
	}
	return nData;
}

template<typename STACKTYPE>
STACKTYPE CLSCStack<STACKTYPE>::DataGet(void)
{
	STACKTYPE nData = 0;
	STACKTYPE *pnStackCurrent = (STACKTYPE *)m_pvStackTop;
	
	if(pnStackCurrent != NULL)
	{
		if(!IsEmpty())
		{
			pnStackCurrent--;
			nData = *pnStackCurrent;
		}
		else
		{
			nData = *(STACKTYPE *)m_pvStack;
		}
	}
	return nData;
}

template<typename STACKTYPE>
STACKTYPE CLSCStack<STACKTYPE>::DataGet(unsigned int nNumber)
{
	STACKTYPE nData = 0;
	STACKTYPE *pnStackCurrent = (STACKTYPE *)m_pvStack;
	
	if(pnStackCurrent != NULL)
	{
		if(nNumber <= m_nMaxSize && nNumber > 0)
		{
			nData = *(pnStackCurrent + nNumber - 1);
		}
	}
	return nData;
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::DataSet(STACKTYPE nData)
{
	STACKTYPE *pnStackCurrent = (STACKTYPE *)m_pvStackTop;
	if(pnStackCurrent != NULL)
	{
		if(!IsEmpty())
		{
			pnStackCurrent--;
			*pnStackCurrent = nData;
		}
		else
		{
			*(STACKTYPE *)m_pvStack = nData;
		}
	}
}

template<typename STACKTYPE>
void CLSCStack<STACKTYPE>::DataSet(unsigned int nNumber, STACKTYPE nData)
{
	STACKTYPE *pnStackCurrent = (STACKTYPE *)m_pvStack;
	if(pnStackCurrent != NULL)
	{
		if(nNumber <= m_nMaxSize && nNumber > 0)
		{
			*(pnStackCurrent + nNumber - 1) = nData;
		}
	}
}

template<typename STACKTYPE>
int CLSCStack<STACKTYPE>::NumberGet(void)
{
	int nNumber = 0;
	if(m_pvStackTop != m_pvStack && m_pvStack != NULL && m_pvStackTop != NULL)
	{
		nNumber = ((char *)m_pvStackTop - (char *)m_pvStack)/m_nStackUnit;
	}
	return nNumber;
}


template<typename STACKTYPE>
bool CLSCStack<STACKTYPE>::IsEmpty(void)
{
	bool bStatus = false;
	if(m_pvStackTop <= m_pvStack)
	{
		bStatus = true;
	}
	return bStatus;
}

template<typename STACKTYPE>
bool CLSCStack<STACKTYPE>::IsFull(void)
{
	bool bStatus = false;
	if((unsigned int)((char *)m_pvStackTop - (char *)m_pvStack)/m_nStackUnit >= m_nMaxSize)
	{
		bStatus = true;
	}
	return bStatus;
}



