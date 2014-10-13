#ifndef _gzhyTool_SZZT_1512_
#define _gzhyTool_SZZT_1512_


//#define  CYCLE_TEST  1

#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define  SUCCESS_GZHY                0                    //成功
#define  FAILD_GZHY                  -1                   //失败


#define WRITE_DEBUG_INFO(b)       WriteClientLog(b, __FILE__, __LINE__, GetCurrentThreadId())  
#define WRITE_ERROR_INFO(b)       WriteClientErrorLog(b, __FILE__, __LINE__, GetCurrentThreadId()) 
#define GET_SUB_STR(a,  b,  c)    getSubString(a, b, c)

#ifndef isblank
#define isblank(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#endif


extern string asc2hex(const string &strAsc, string &strHex);

extern  void StrSafeCopy(char* sDest, const char* sSrc, int iLen);

extern  string TrimString(string strSrc);

extern void ChecValidPara(string& strDest, const int iRightLen);

extern void Sxor(const string& _strMsg, string& _strLRC);

extern string getSubString(const string& strSrc, const int& idx, const int& iSubLen);

#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
//_waccess的第二个参数表示仅判断存在性，返回0表示存在，返回-1表示不存在
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access是MBCS版本
#endif

extern long GetFileSize(const char* pFilePath);
extern void GetTimeWithUSecond(string& strTime);
extern void GetTimeWithSecond(string& strTime);
extern string Bcd2Asc(const string &strBcd, string &strAsc);
extern void GetApplicationDir(string& strAppDir);
#endif