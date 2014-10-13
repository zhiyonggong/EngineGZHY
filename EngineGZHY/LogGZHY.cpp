#include <string>
#include <list>
#include <fstream>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#ifdef WIN32
#include <SDKDDKVer.h>
#include <process.h>
#endif
#include <windows.h>
#include "MutexGZHY.h"
#include "LogGZHY.h"
#include "ToolGZHY.h"
using namespace std;


bool g_bIsContinue = true;

unsigned int __stdcall thread_func(void* p)
{
	while (g_bIsContinue)
	{
		CLogGZHY::write_all_log();
		Sleep(1);
	}
	return 1;
}

map<string, CLogGZHY*> CLogGZHY::m_mapLogObj;
CMutexGZHY *CLogGZHY::m_ThreadMutex = NULL;

#define UN_START_THREAD     1        //线程ID初始值，
int CLogGZHY::m_threadId = UN_START_THREAD;
bool CLogGZHY::m_bIsContinue = true;
CMutexGZHY *CLogGZHY::m_pLogDataMutex = NULL;



CLogGZHY::CLogGZHY(const string &_strFirstName)
	:m_strFirstName(_strFirstName)
{

}
/*
*  功能     --  日志析构函数
*  输入参数 --  无
*  输出参数 --  无
*  return   --  无
*/
CLogGZHY::~CLogGZHY()
{
	stop_log();
	Sleep(10);
}

int CLogGZHY::write_log()
{
	m_pLogDataMutex->Lock();
	if ( m_LogData.empty() )
	{
		m_pLogDataMutex->UnLock();
		return 0;
	}
	m_pLogDataMutex->UnLock();

	string szIniPath;
	GetApplicationDir(szIniPath); 

	string strFilename;
	string strFileDir = szIniPath + string("\\");

	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);
	char sFileName[200] = {0};
	sprintf (sFileName, "%4d%02d%02d%02d%02d%02d",currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond);
	string strCurDate = string(sFileName);
	strFilename = strFileDir + m_strFirstName + string(".log");

	//write log
	long lMaxSize = 1024*1024*10;
	//文件太大(超过10M),则先备份
	if (GetFileSize(strFilename.c_str()) >= lMaxSize)
	{
		string strFilenameBak;
		string strCurDateTime;
		GetTimeWithUSecond(strCurDateTime);
		strFilename = strFileDir + m_strFirstName + strCurDateTime + string(".log");	
	}

	vector<string> logDataVec;
	if (0 != get_log_data(logDataVec))
	{
		Sleep(1);
	}

	ofstream ofsLog;
	ofsLog.open(strFilename.c_str(), ofstream::app);
	if (!ofsLog)
	{
		cout << "open log fail." << endl;
		return -1;
	}	

	int iSize = logDataVec.size();
	for(int i=0; i<iSize; ++i)
	{
		ofsLog << logDataVec[i] << endl;	
	}

	ofsLog.close();
	ofsLog.clear();
	logDataVec.clear();

	return 0;
}

void CLogGZHY::add_log(const char *_pcFormat, ...)
{
	char scBuffer[1024]={0};

	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);
	char sBuf[500] = {0};
	sprintf (sBuf, "%4d-%02d-%02d %02d:%02d:%02d:%8d", currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond, currentTime.wMilliseconds);
	string strHead(sBuf);

	va_list args;
	va_start(args, _pcFormat);
	vsprintf(scBuffer, _pcFormat, args);
	va_end(args);

	string strLog = strHead + string(scBuffer);
	m_pLogDataMutex->Lock();
	m_LogData.push(strLog);
	m_pLogDataMutex->UnLock();
}


void CLogGZHY::add_log(const string &_strLogBuffer)
{
	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);
	char sBuf[500] = {0};
	sprintf (sBuf, "%4d-%02d-%02d %02d:%02d:%02d:%8d", currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond, currentTime.wMilliseconds);
	string strHead(sBuf);

	string strLog = strHead + _strLogBuffer;

	m_pLogDataMutex->Lock();
	m_LogData.push(strLog);
	m_pLogDataMutex->UnLock();
}

void CLogGZHY::add_Log(const string& strLog, char* pFileName, int iLine, long int lId)
{
	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);
    char sBuf[500] = {0};
	sprintf (sBuf, "%4d-%02d-%02d %02d:%02d:%02d:%8d F[%s]L[%ld][%ld]:", currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond, currentTime.wMilliseconds, pFileName, iLine, lId);
	string strHead(sBuf);

	string strLogData = strHead + strLog;

	m_pLogDataMutex->Lock();
	m_LogData.push(strLogData);
	m_pLogDataMutex->UnLock();
}

/*
*  功能     --  增加16进制日志记录
*  输入参数 --  BCD码字符串
*  输出参数 --  
*  return   --  无
*/
void CLogGZHY::add_log_hex(const string &_strBuffer)
{
	int i;
	int iOffset;
	int iInsertCount; //插入次数，每两字符插入一个空格或一个换行符(linux下换行符是两个字节\n\r)
	string strHex;

	string strDateTime;
	GetTimeWithSecond(strDateTime);
	string strHead = strDateTime + string(" ");

	Bcd2Asc(_strBuffer, strHex);
	iOffset = 2;        
	iInsertCount = strHex.size() / 2;
	for(i = 0; i < iInsertCount; i++)
	{
		if ( (i+1)%16 == 0)
		{
			strHex.insert(iOffset, 1, '\n');           
			iOffset += 3;             
		}
		else if ( (i+1)%8 == 0)
		{
			strHex.insert(iOffset, string("   "));
			iOffset += 5;             
		}
		else
		{
			strHex.insert(iOffset, 1, ' ');
			iOffset += 3;
		}
	}

	string strLog = strHead + string("\n") + strHex;
	m_pLogDataMutex->Lock();
	m_LogData.push(strLog);
	m_pLogDataMutex->UnLock();
}


CLogGZHY *CLogGZHY::get_log_object(const string &_strFirstName)
{
	if (m_ThreadMutex == NULL)
	{
		cout << "new thread_mutex" << endl;
		m_ThreadMutex = new CMutexGZHY;
	}
	//Lock 
	m_ThreadMutex->Lock();
	CLogGZHY* pLog = NULL;
	do
	{
		if (NULL == m_pLogDataMutex)
		{
			m_pLogDataMutex = new CMutexGZHY;
		}
		if (m_mapLogObj.find(_strFirstName) == m_mapLogObj.end())
		{
			pLog = new CLogGZHY(_strFirstName);
			if (NULL == pLog)
			{
				cout << "create log fail." << endl;
				break;
			}
			m_mapLogObj.insert(make_pair(_strFirstName, pLog));
			break;
		}
		pLog = m_mapLogObj[_strFirstName];
	}while(false);

	//UnLock
	m_ThreadMutex->UnLock();

	return pLog;
}


void CLogGZHY::write_all_log()
{
	map<string, CLogGZHY*>::iterator it;
	for(it=m_mapLogObj.begin(); it!=m_mapLogObj.end(); ++it)
	{
		CLogGZHY* pLog = (CLogGZHY*)(it->second);
		if (NULL != pLog)
		{
			pLog->write_log();
		}
	}
}

int CLogGZHY::start_log()
{
	if (NULL == m_ThreadMutex)
	{
		m_ThreadMutex = new CMutexGZHY;
	}
	if (NULL == m_pLogDataMutex)
	{
		m_pLogDataMutex = new CMutexGZHY;
	}
	if (UN_START_THREAD == m_threadId)
	{
		(HANDLE)_beginthreadex(NULL, 0, thread_func, NULL, 0, NULL);
		m_threadId = UN_START_THREAD + 2;
	}
	return 0;
}

int CLogGZHY::stop_log()
{
	g_bIsContinue = false;
	return 0;
}

int CLogGZHY::get_log_data(vector<string>& logDataVec)
{
	logDataVec.clear();
	int iCount = 0;
	int iSize = m_LogData.size();
	iCount = iSize>50? 50:iSize;
	m_pLogDataMutex->Lock();
	for (int i=0; i<iCount; ++i)
	{
		logDataVec.push_back(m_LogData.front());
		m_LogData.pop();
	}
	m_pLogDataMutex->UnLock();
	return 0;
}
