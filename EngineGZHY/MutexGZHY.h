#ifndef  _mutexGZHY_1500_
#define  _mutexGZHY_1500_


#ifdef WIN32
#include <windows.h>
#endif

class CMutexGZHY
{
public:
	CMutexGZHY();
	~CMutexGZHY();
public:
	void Lock();
	void UnLock();
private:
	CRITICAL_SECTION m_cs;
};


#endif