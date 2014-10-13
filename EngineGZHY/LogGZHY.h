
#ifndef __LOG_GZHY_1453_
#define __LOG_GZHY_1453_

#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>



using namespace std;


class CMutexGZHY;



class CLogGZHY
{
public:
	CLogGZHY(const string &_strFirstName);
	~CLogGZHY();
	
public:
	static int start_log();
	static int stop_log();
	static CLogGZHY *get_log_object(const string &_strFirstName);
public:
	void add_log(const string &_strLogBuffer);
	void add_log(const char *_pcFormat, ...);
	void add_log_hex(const string &_strBuffer);
	void add_Log(const string& strLog, char* pFileName, int iLine, long int lId);
	static void write_all_log();
private:
	int write_log();
	int get_log_data(vector<string>& logDataVec);
private:
	const string m_strFirstName;
	queue<string> m_LogData;
	static map<string, CLogGZHY*>  m_mapLogObj;
	static CMutexGZHY *m_ThreadMutex;
	static bool  m_bIsContinue;                //标志线程是否正在运行
	static CMutexGZHY* m_pLogDataMutex;
	static int m_threadId;
};


#endif
