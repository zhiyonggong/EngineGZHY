#include "ToolGZHY.h"
#include <stdio.h>
#include <time.h>  
#include <iostream>
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <tchar.h>
#endif



extern string asc2hex(const string &strAsc, string &strHex)
{
	int  i, iLen;
	char cTmp1, cTmp2;

	iLen = strAsc.size();
	strHex = "";
	for (i = 0; i < iLen; i++)
	{
		cTmp1 = (strAsc[i] & 0xf0) >> 4;
		cTmp2 = (cTmp1 > 9) ? cTmp1 + 0x41 - 10 : cTmp1 + 0x30;
		strHex.insert(strHex.size(), 1, cTmp2);

		cTmp1 = strAsc[i] & 0xf;
		cTmp2 = (cTmp1 > 9) ? cTmp1 + 0x41 - 10 : cTmp1 + 0x30;
		strHex.insert(strHex.size(), 1, cTmp2);
	}
	return strHex;
}

#ifndef isblank
#define isblank(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#endif

extern void StrSafeCopy(char* sDest, const char* sSrc, int iLen)
{
	//strncpy(sDest, sSrc, iLen);
	memcpy(sDest, sSrc, iLen);
}

extern string TrimString(string strSrc)
{	
	int iLen = strSrc.length() + 1;
	if (1 == iLen)
	{
		return strSrc;
	}
	char* szDest = new char[iLen+1];
	memset(szDest, 0, iLen+1);
	StrSafeCopy(szDest, strSrc.c_str(), iLen);
	int n = 0;
	int nLen = 0;	
	if (szDest != NULL)	
	{		
		/* ―――― 以下删除字符串右边的空格 ―――― */
		for (n = strlen(szDest); n > 0; n--)	
		{			
			if (!isblank(szDest[n - 1])) 
			{
				break;
			}
		}		
		szDest[n] = '\0'; 		
		/* 将右边最靠左的一个空格替换为0既可 */		
		/* ―――― 以下删除字符串左边的空格 ―――― */
		nLen = strlen(szDest);		
		for (n = 0; n < nLen; n++)		
		{			
			if (!isblank(szDest[n])) 
			{
				break;
			}
		}		
		strcpy(szDest, szDest + n);
		/* 从左边第一个非空格起向前移动到串首既可*/	
	}
	string strDest = string(szDest);
	delete[] szDest;
	return strDest;
}

extern void ChecValidPara(string& strDest, const int iRightLen)
{
	int iLen = strDest.length();
	if (iLen < iRightLen)
	{
		int iAdd = iRightLen - iLen;
		string strTemp(iAdd, ' ');
		strDest += strTemp;
	}
	else
	{
		strDest.resize(iRightLen);
	}
}

extern void Sxor(const string& _strMsg, string& _strLRC)
{
	int i;    
	unsigned char ucLRC;
	ucLRC = static_cast<unsigned char>(_strMsg[0]);
	for(i=1; i<_strMsg.size(); i++)
	{
		ucLRC = ucLRC ^ static_cast<unsigned char>(_strMsg[i]);
	}   
	_strLRC.assign(1, static_cast<char>(ucLRC));
}


extern string getSubString(const string& strSrc, const int& idx, const int& iSubLen)
{
	string strDest;
	int iLen = strSrc.length();
	int iIndex = idx;
	int iDestLen = iSubLen;
	if (0 == iLen)
	{
		return string();
	}

	if (iIndex < 0)
	{
		iIndex = 0;
	}
	if (iIndex >= iLen)
	{
		iIndex = iLen - 1;
	}
	if (iLen >= iIndex + iDestLen)
	{
		strDest = strSrc.substr(iIndex, iDestLen);
		return strDest;
	}
	iDestLen = iLen - iIndex;
	strDest = strSrc.substr(iIndex, iDestLen);
	return strDest;
}

long GetFileSize(const char* pFilePath)
{
	ifstream ofs(pFilePath, ios::binary);    // 要处理的文件
	ofs.seekg(0, ios::end);                         // 文件指针指向末尾
	long llength = (long)ofs.tellg();                           // 得到文件的长度
	ofs.close();
	return llength;
}

void GetTimeWithUSecond(string& strTime)
{
	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);

	char sBuf[500] = {0};
	sprintf (sBuf, "%4d%02d%02d%02d%02d%02d%8d", currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond, currentTime.wMilliseconds);
	strTime = string(sBuf);
}

void GetTimeWithSecond(string& strTime)
{
	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);

	char sBuf[500] = {0};
	sprintf (sBuf, "%4d%02d%02d%02d%02d%02d", currentTime.wYear, currentTime.wMonth,
		currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond);
	strTime = string(sBuf);
}

/*
*  功能     --  BCD码转换到ASCII码
*  输入参数 --  strBcd  要转换的BCD码字符串;
*  输出参数 --  strAsc  转换后的ASCII码字符串;
*  return   --  返回ASCII字符串;
*/
string Bcd2Asc(const string &strBcd, string &strAsc)
{
	int i, iLen;
	char c;

	iLen = strBcd.size();
	strAsc = "";
	for (i = 0; i < iLen; ++i)
	{
		c = (strBcd[i] >> 4) & 0x0f;
		if(c < 10)
		{
			c += '0';
		}
		else
		{
			c = c - 10 + 'A';
		}
		strAsc.insert(strAsc.size(), 1, c);

		c = strBcd[i] & 0x0f;
		if(c < 10)
		{
			c += '0';
		}
		else
		{
			c = c - 10 + 'A';
		}
		strAsc.insert(strAsc.size(), 1, c);
	}

	return strAsc;
}

void GetApplicationDir(string& strAppDir)
{
	char tcExePath[MAX_PATH] = {0};  
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	char *pFind = _tcsrchr(tcExePath, '\\');  
	if (pFind == NULL)  
	{  
		return;  
	}  
	*pFind = '\0';  
	strAppDir = string(tcExePath);
}