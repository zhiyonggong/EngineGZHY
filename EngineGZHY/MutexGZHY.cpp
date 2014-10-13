#include "MutexGZHY.h"








CMutexGZHY::CMutexGZHY()
{
	InitializeCriticalSection(&m_cs);
}

CMutexGZHY::~CMutexGZHY()
{
	DeleteCriticalSection(&m_cs);
}

void CMutexGZHY::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CMutexGZHY::UnLock()
{
	LeaveCriticalSection(&m_cs);
}