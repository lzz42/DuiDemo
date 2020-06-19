#include <direct.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <WinBase.h>
#include <Windows.h>
#include <winerror.h>
#include <winreg.h>
#include <tchar.h>
#include <stringapiset.h>
#include <vector>
#include <map>
#include "common.h"
#include <cstdio>
//#include "zlibunzip.h"

using std::vector;
using std::map;

#pragma region env info

#ifdef _DEBUG    

#define DP0(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);}    
#define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);}    
#define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);}    
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);}    

#endif    

#ifndef _DEBUG    

#define DP0(fmt) ;    
#define DP1(fmt, var) ;    
#define DP2(fmt,var1,var2) ;    
#define DP3(fmt,var1,var2,var3) ;    

#endif    


/// <summary>
/// 获取当前进程执行文件的全路径
/// </summary>
/// <returns></returns>
char* get_app_path() {
	return _pgmptr;
}

/// <summary>
/// 获取当前进程执行文件所在目录
/// </summary>
/// <returns></returns>
char* get_app_dir() {
	char buf1[256];
	_getcwd(buf1, sizeof(buf1));
	return buf1;
}

#pragma endregion

#pragma region type convert

LPCTSTR to_lpctstr(const char* chars) {
	int num = MultiByteToWideChar(0, 0, chars, -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, chars, -1, wide, num);
	return wide;
}

char* to_pchar(LPCTSTR lpstr) {
	int num = WideCharToMultiByte(CP_OEMCP, NULL, lpstr, -1, NULL, 0, NULL, FALSE);
	char* pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, lpstr, -1, pchar, num, NULL, FALSE);
	return pchar;
}

char* to_lpchar(TCHAR* lptchar) {
	return to_pchar(lptchar);
}

char* to_lpchar(TCHAR* lptchar, int len) {
	TCHAR* tempT = (TCHAR*)malloc(sizeof(TCHAR) * len);
	for (size_t i = 0; i < len; i++)
	{
		tempT[i] = lptchar[i];
	}
	char* temp = to_lpchar(tempT);
	return temp;
}

#pragma endregion

#pragma region ini file program

/// <summary>
/// 读取指定ini文件的所有区域
/// </summary>
/// <param name="inifilepath"></param>
/// <returns></returns>
vector<char*> ini_read_sections(LPCTSTR inifilepath) {
	vector<char*> vRet;
	TCHAR buf[2048] = { 0 };
	long nSize = sizeof(buf) / sizeof(buf[0]);
	GetPrivateProfileSectionNames(buf, nSize, inifilepath);
	TCHAR* p, * q;
	p = q = buf;
	while (*p)//即 '\0' != *p
	{
		while (*q)
		{
			++q;
		}
		TCHAR* tempT = (TCHAR*)malloc(sizeof(TCHAR) * (q - p));
		for (size_t i = 0; i < q-p; i++)
		{
			tempT[i] = p[i];
		}
		char* temp = to_lpchar(tempT);
		vRet.push_back(temp);
		p = q + 1;
		q = q + 1;
	}
	return vRet;
}


/// <summary>
/// 读取指定ini文件指定区域的所有key-value值
/// </summary>
/// <param name="section"></param>
/// <param name="inifilepath"></param>
/// <returns></returns>
map<char*, char*> ini_read_keys_values(LPCTSTR section, LPCTSTR inifilepath) {
	map<char*, char*> mapRet;
	TCHAR buf[2048] = { 0 };
	long nSize = sizeof(buf) / sizeof(buf[0]);
	GetPrivateProfileSection(section, buf, nSize, inifilepath);
	TCHAR* p = buf;
	TCHAR* q = buf;
	while (*p)
	{
		char* strKey, *strValue;
		while (*q)
		{
			if (_T('=') == *q)
			{
				strKey = to_lpchar(p, q - p);
				p = q + 1;
			}
			++q;
		}
		strValue = to_lpchar(p, q - p);
		mapRet.insert(std::make_pair(strKey, strValue));
		p = q + 1;
		q = q + 1;
	}
	return mapRet;
}

char* ini_read_value_cstring(LPCTSTR key, LPCTSTR section, LPCTSTR inifile) {
	TCHAR* str = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
	GetPrivateProfileString(section, key, _T(""), str, MAX_PATH, inifile);
	return to_lpchar(str);
}

int ini_read_value_int(LPCTSTR key, LPCTSTR section, LPCTSTR inifile) {
	return GetPrivateProfileInt(section, key, 0, inifile);
}

bool ini_write(LPCTSTR section, LPCTSTR key, LPCTSTR value, LPCTSTR inifile) {
	return WritePrivateProfileString(section, key, value, inifile);
}

bool ini_write(LPCTSTR section, LPCTSTR data, LPCTSTR inifile) {
	return WritePrivateProfileSection(section, data, inifile);
}

bool ini_delete(LPCTSTR section, LPCTSTR inifile, bool bdeletesection) {
	if (bdeletesection) {
		return	ini_write(section, NULL, inifile);
	}
	else {
		return	ini_write(section, _T(""), inifile);
	}
}

bool ini_write(const char* section, const char* key, const char* value, const char* inifile) {
	return ini_write(to_lpctstr(section), to_lpctstr(key), to_lpctstr(value), to_lpctstr(inifile));
}

bool ini_write(const char* section, const char* data, const char* inifile) {
	return ini_write(to_lpctstr(section), to_lpctstr(data), to_lpctstr(inifile));
}

bool ini_delete(const char* section, const char* inifile, bool bdeletesection) {
	return ini_delete(to_lpctstr(section), to_lpctstr(inifile), bdeletesection);
}

char* ini_read_value_cstring(const char* key, const char* section, const char* ini) {
	return ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
}

/// <summary>
/// 读取ini文件中的内容 返回 char*
/// </summary>
/// <param name="key">键名</param>
/// <param name="section">区域</param>
/// <param name="ini">ini文件</param>
/// <returns></returns>
int ini_read_value_int(const char* key, const char* section, const char* ini) {
	return ini_read_value_int(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
}

/// <summary>
/// 读取ini文件中的内容 返回 char*
/// </summary>
/// <param name="key">键名</param>
/// <param name="default">默认值</param>
/// <param name="section">区域</param>
/// <param name="ini">ini文件</param>
/// <returns></returns>
char* ini_read_value_cstring(const char* key, const char* default, const char* section, const char* ini) {
	char* res = ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
	if (res == NULL) {
		strcpy(res, default);
	}
	return res;
}
#pragma endregion

#pragma region coding convert

char* unicode_to_utf8(TCHAR * pchars) {
	int num = WideCharToMultiByte(CP_UTF8, NULL, pchars, -1, NULL, 0, NULL, FALSE);
	char* pchar = new char[num];
	WideCharToMultiByte(CP_UTF8, NULL, pchars, -1, pchar, num, NULL, FALSE);
	return pchar;
}

char* unicode_to_gb2312(TCHAR * pchars) {
	int n = WideCharToMultiByte(CP_THREAD_ACP, 0, pchars, -1, 0, 0, 0, 0);
	char* pchar = new char[n];
	WideCharToMultiByte(CP_THREAD_ACP, 0, pchars, -1, pchar, n, 0, 0);
	return pchar;
}

char* unicode_to_ascii(TCHAR * pchars) {
	int nLen = WideCharToMultiByte(CP_ACP, 0, pchars, -1, NULL, 0, "", false);//算出合适的长度
	char* pchar = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, pchars, -1, pchar, nLen, "", false);//UNICODE转ASCII
	return pchar;
}

/// <summary>
/// unicode 转 ascii
/// </summary>
/// <param name="szStr"></param>
/// <returns></returns>
char* unicode_to_Ansii(const wchar_t* szStr) {
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pchar = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pchar, nLen, NULL, NULL);
	return pchar;
}

/// <summary>
/// unicode 转 utf-8
/// </summary>
/// <param name="in"></param>
/// <param name="insize"></param>
/// <returns></returns>
char* unicode_to_utf8(const wchar_t* in, int insize)
{
	int i = 0;
	int outsize = 0;
	int charscount = 0;
	uint8_t* result = NULL;
	uint8_t* tmp = NULL;
	charscount = insize / sizeof(uint16_t);
	result = (uint8_t*)malloc(charscount * 3 + 1);
	memset(result, 0, charscount * 3 + 1);
	tmp = result;
	for (i = 0; i < charscount; i++)
	{
		uint16_t unicode = in[i];
		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			*tmp = (uint8_t)unicode;
			tmp += 1;
			outsize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			*tmp = 0xc0 | (unicode >> 6);
			tmp += 1;
			*tmp = 0x80 | (unicode & (0xff >> 2));
			tmp += 1;
			outsize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			*tmp = 0xe0 | (unicode >> 12);
			tmp += 1;
			*tmp = 0x80 | (unicode >> 6 & 0x00ff);
			tmp += 1;
			*tmp = 0x80 | (unicode & (0xff >> 2));
			tmp += 1;
			outsize += 3;
		}
	}
	*tmp = '\0';
	char* res = (char*)malloc(sizeof(char) * (charscount + 1));
	for (int i = 0; i < charscount; i++)
	{
		res[i] = result[i];
	}
	std::free(result);
	return res;
}

#pragma endregion

#pragma region zip unzip
//
///// <summary>
///// 解压文件到指定文件夹
///// </summary>
///// <param name="srcfile"></param>
///// <param name="dstpath"></param>
///// <returns></returns>
//bool c_unzip(CString srcfile, CString dstpath) {
//	CString src = CString(srcfile);
//	CString dst = CString(dstpath);
//	//打开压缩文件夹创建hzip
//	HZIP hz = OpenZipU((void*)(LPTSTR)(LPCTSTR)src, 0, ZIP_FILENAME);
//	if (hz == NULL) {
//		//打开压缩文件失败
//		return false;
//	}
//	ZIPENTRYW ze;
//	GetZipItem(hz, -1, &ze);
//	int numitems = ze.index;
//	for (int zi = 0; zi < numitems; zi++)
//	{
//		ZIPENTRYW ze;
//		GetZipItem(hz, zi, &ze);
//		CString zeName;
//		zeName.Format(_T("%s"), ze.name);
//		//处理zename不为null的文件与文件夹
//		if (ze.name != NULL) {
//			//zename以/结束的为文件夹 判断后做创建处理
//			if (zeName.Right(1) == "/") {
//				SHCreateDirectoryEx(NULL, dst + L"\\" + zeName.Left(zeName.GetLength() - 1), NULL);
//			}
//			//文件处理 将路径中的/变成右斜杠
//			else {
//				zeName.Replace(_T("/"), _T("\\"));
//				SHCreateDirectoryEx(NULL, dst + L"\\" + zeName.Left(zeName.ReverseFind('\\')), NULL);
//				ZRESULT a = UnzipItem(hz, zi, (void*)(LPTSTR)(LPCTSTR)(dst + L"\\" + zeName), 0, ZIP_FILENAME);
//				SetFileAttributes((dst + L"\\" + zeName), FILE_ATTRIBUTE_NORMAL);
//			}
//		}
//	}
//	//关闭压缩文件
//	CloseZip(hz);
//	return true;
//}

#pragma endregion

#pragma region net framework version
/*
https://docs.microsoft.com/zh-cn/dotnet/framework/migration-guide/how-to-determine-which-versions-are-installed
*/

#define NET_V4 "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4" //存在与否 判断是否安装 4.0+版本
#define NET_V45_PLUS_FULL "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\FULL" //存在与否  判断是否安装 4.5+版本
//#define NET_V4_VERSION "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\FULL\\version" // 获取 net 版本号 string

//#define NET_V45_PLUS_RELEASE "SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\FULL\release" //存在与否  判断是否安装 4.5+版本 int值判断具体版本
//#define NET_V4_INSTALLPATH "SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\FULL\InstallPath" //获取安装路径

#define NET_V450_RELEASE 378389
#define NET_V451_RELEASE 378675
#define NET_V452_RELEASE 379893
#define NET_V460_RELEASE 393295
#define NET_V461_RELEASE 394254
#define NET_V462_RELEASE 394802
#define NET_V470_RELEASE 460798
#define NET_V471_RELEASE 461308
#define NET_V472_RELEASE 461808
#define NET_V480_RELEASE 528040

static map<int, char*> net_versions = {
	{NET_V450_RELEASE,"4.5"},
	{NET_V451_RELEASE,"4.5.1"},
	{NET_V452_RELEASE,"4.5.2"},
	{NET_V460_RELEASE,"4.6"},
	{NET_V461_RELEASE,"4.6.1"},
	{NET_V462_RELEASE,"4.6.2"},
	{NET_V470_RELEASE,"4.7"},
	{NET_V471_RELEASE,"4.7.1"},
	{NET_V472_RELEASE,"4.7.2"},
	{NET_V480_RELEASE,"4.8"},
};

struct NET_FRAMEWROK_VERSION
{
	bool Is_V4;
	bool Is_V45;
	int Release;
	char* Version;
	char* InstallPath;
};

static NET_FRAMEWROK_VERSION net_version;

void get_net_framework_version() {
	DP0("get_net_framework_version TEST");
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	NET_FRAMEWROK_VERSION* pver = (NET_FRAMEWROK_VERSION*)malloc(sizeof(NET_FRAMEWROK_VERSION));
	try
	{
		HKEY hkv4full;
		int vret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(NET_V45_PLUS_FULL), 0, KEY_WOW64_64KEY | KEY_ALL_ACCESS, &hkv4full);
		if (vret != ERROR_SUCCESS) {
			DP0("打开v4\\Full 失败,Exit Code:" + toascii(vret));
			printf("打开v4\\Full 失败,Exit Code:\t %d \n", vret);
			pver->Is_V4 = false;
			return;
		}
		DP0("打开v4\\Full 成功,Exit Code:" + toascii(vret));
		pver->Is_V4 = true;
		DWORD lpType = REG_DWORD;
		DWORD lpcbData = 256;
		LPBYTE lpData = new BYTE[4];
		//读取 release 项值
		vret = RegQueryValueEx(hkv4full, TEXT("release"), 0, &lpType, lpData, &lpcbData);
		if (vret != ERROR_SUCCESS) {
			pver->Is_V45 = false;
			DP0("打开v4\\Full\\Release 失败,Exit Code:" + toascii(vret));
			printf("打开v4\\Full\\Release 失败,Exit Code:\t %d \n", vret);
		}
		else {
			DP0("打开v4\\Full\\Release 成功,Exit Code:" + toascii(vret));
			pver->Is_V45 = true;
			if (lpType == REG_DWORD && lpcbData >= 4) {
				pver->Release = lpData[0] & 0xFF;
				pver->Release |= ((lpData[1] << 8) & 0xFF00);
				pver->Release |= ((lpData[2] << 16) & 0xFF0000);
				pver->Release |= ((lpData[3] << 24) & 0xFF000000);
				if (pver->Release < NET_V450_RELEASE) {
					pver->Version = "4.5-";
				}
				else if (pver->Release > NET_V480_RELEASE) {
					pver->Version = "4.8+";
				}
				else {
					std::map<int, char*>::iterator iter;
					iter = net_versions.find(pver->Release);
					if (iter != net_versions.end()) {
						strcpy(pver->Version, iter->second);
					}
				}
			}
			else {
				pver->Release = 0;
			}
		}
		//读取 InstallPath 项值
		char* pcData = (char*)malloc(sizeof(char) * 1024 * 2);
		lpType = REG_SZ;
		lpcbData = 1024 * 2;
		vret = RegQueryValueEx(hkv4full, TEXT("release"), 0, &lpType, (LPBYTE)pcData, &lpcbData);
		if (vret == ERROR_SUCCESS) {
			if (lpType == REG_SZ) {
				pcData[lpcbData] = '\0';
				char* pstr = new char[sizeof(BYTE) * lpcbData];
				strcpy(pstr, pcData);
				pver->InstallPath = pstr;
			}
		}
		std::free(pcData);
		::RegCloseKey(hkv4full);
		net_version = *pver;
	}
	catch (const std::exception& ex)
	{
		DP0("exception");
		//DP0(ex.what());
	}
	//DP0(pver->Version);
	//DP0(pver->InstallPath);
	//DP0(pver->Is_V4 ? "V4 True" : "V4 False");
	//DP0(pver->Is_V45 ? "V45 True" : "V45 False");
	DP0("get_net_framework_version exit");
}

bool installed_net_4() {
	return false;
}

char* get_installee_net_4_version() {
	return false;
}

bool installed_net_45() {
	return false;
}


//char* get_installed_net_45_version() {
//
//}
//
//
//int get_installed_net_45_release() {
//
//}

#pragma endregion
