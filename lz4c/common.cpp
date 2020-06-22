#ifdef __AFXSTR_H__

#include "stdafx.h"

#endif // __AFXSTR_H__

#pragma warning(disable:4996)

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif


#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#include "common.h"
#include <direct.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stringapiset.h>
#include <vector>
#include <map>
#include <conio.h>

using std::vector;
using std::map;

namespace common {

#pragma region env info

char* get_app_name() {
	char* name = _pgmptr;
	int len = strlen(name);
	char* ret = new char[len];
	int k = 0;
	for (int j = len - 1; j > 0; j--)
	{
		if (name[j] == '\\') {
			ret[len - j - 1] = '\0';
			k++;
			break;
		}
		else if (name[j] == '\0') {
			continue;
		}
		else {
			ret[len - j - 1] = name[j];
			k++;
		}
	}
	char pt;
	for (int j = 0; j < (k - 1) / 2; j++)
	{
		pt = ret[j];
		ret[j] = ret[k - j - 2];
		ret[k - j - 2] = pt;
	}
	return ret;
}

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

#ifdef __AFXSTR_H__
CString get_app_dir_cstring() {
	return CString(get_app_dir());
}

CString get_app_path_cstring() {
	return CString(get_app_path());
}
#endif //__AFXSTR_H__

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

LPCWSTR to_lpcwstr(const char* chars) {
	WCHAR wszClassName[1024];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, chars, strlen(chars) + 1, wszClassName,
	sizeof(wszClassName) / sizeof(wszClassName[0]));
	return wszClassName;
}

#define C2W to_lpcwstr

#pragma endregion

#pragma region ini file program

#ifdef __AFXSTR_H__


/// <summary>
/// 读取指定ini文件的所有区域
/// </summary>
/// <param name="inifilepath"></param>
/// <returns></returns>
vector<CString> ini_read_sections(LPCTSTR inifilepath) {
	vector<CString> vRet;
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
		CString str(p, q - p);
		vRet.push_back(str);
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
map<CString, CString> ini_read_keys_values(LPCTSTR section, LPCTSTR inifilepath) {
	map<CString, CString> mapRet;
	TCHAR buf[2048] = { 0 };
	long nSize = sizeof(buf) / sizeof(buf[0]);
	GetPrivateProfileSection(section, buf, nSize, inifilepath);
	TCHAR* p = buf;
	TCHAR* q = buf;
	while (*p)
	{
		CString strKey, strValue;
		while (*q)
		{
			if (_T('=') == *q)
			{
				strKey = CString(p, q - p);
				p = q + 1;
			}
			++q;
		}
		strValue = CString(p, q - p);
		mapRet.insert(std::make_pair(strKey, strValue));
		p = q + 1;
		q = q + 1;
	}
	return mapRet;
}

CString ini_read_value_cstring(LPCTSTR key, LPCTSTR section, LPCTSTR inifile) {
	CString str;
	GetPrivateProfileString(section, key, _T(""), str.GetBuffer(MAX_PATH), MAX_PATH, inifile);
	return str;
}

CString ini_read_value_cstring(const char* key, const char* section, const char* ini) {
	return ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
}

/// <summary>
/// 读取ini文件中的内容 返回 cstring
/// </summary>
/// <param name="key">键名</param>
/// <param name="default">默认值</param>
/// <param name="section">区域</param>
/// <param name="ini">ini文件</param>
/// <returns></returns>
CString ini_read_value_cstring(const char* key, const char* default, const char* section, const char* ini) {
	CString res = ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
	if (res.IsEmpty()) {
		res = CString(default);
	}
	return res;
}

#endif // __AFXSTR_H__

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
		return	ini_write(section, TEXT(""), inifile);
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


/// <summary>
/// 读取ini文件中的内容 返回 cstring
/// </summary>
/// <param name="key">键名</param>
/// <param name="section">区域</param>
/// <param name="ini">ini文件</param>
/// <returns></returns>
int ini_read_value_int(const char* key, const char* section, const char* ini) {
	return ini_read_value_int(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
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

#ifdef __AFXSTR_H__

#include "zlibunzip.h"

/// <summary>
/// 解压文件到指定文件夹
/// </summary>
/// <param name="srcfile"></param>
/// <param name="dstpath"></param>
/// <returns></returns>
bool c_unzip(CString srcfile, CString dstpath) {
	CString src = CString(srcfile);
	CString dst = CString(dstpath);
	//打开压缩文件夹创建hzip
	HZIP hz = OpenZipU((void*)(LPTSTR)(LPCTSTR)src, 0, ZIP_FILENAME);
	if (hz == NULL) {
		//打开压缩文件失败
		return false;
	}
	ZIPENTRYW ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	for (int zi = 0; zi < numitems; zi++)
	{
		ZIPENTRYW ze;
		GetZipItem(hz, zi, &ze);
		CString zeName;
		zeName.Format(_T("%s"), ze.name);
		//处理zename不为null的文件与文件夹
		if (ze.name != NULL) {
			//zename以/结束的为文件夹 判断后做创建处理
			if (zeName.Right(1) == "/") {
				SHCreateDirectoryEx(NULL, dst + L"\\" + zeName.Left(zeName.GetLength() - 1), NULL);
			}
			//文件处理 将路径中的/变成右斜杠
			else {
				zeName.Replace(_T("/"), _T("\\"));
				SHCreateDirectoryEx(NULL, dst + L"\\" + zeName.Left(zeName.ReverseFind('\\')), NULL);
				ZRESULT a = UnzipItem(hz, zi, (void*)(LPTSTR)(LPCTSTR)(dst + L"\\" + zeName), 0, ZIP_FILENAME);
				SetFileAttributes((dst + L"\\" + zeName), FILE_ATTRIBUTE_NORMAL);
			}
		}
	}
	//关闭压缩文件
	CloseZip(hz);
	return true;
}

#endif // __AFXSTR_H__

#pragma endregion

#pragma region system info

int get_system_bits()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return 64;
	}
	return 32;
}

bool is_x64_system() {
	return get_system_bits() == 64;
}

#pragma endregion

#pragma region net framework version
/*
https://docs.microsoft.com/zh-cn/dotnet/framework/migration-guide/how-to-determine-which-versions-are-installed
*/

static NET_FRAMEWROK_VERSION net_version;

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

char* to_lpchar(TCHAR* pcData, int len) {
	char* lpstr = new char[len + 1];
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		lpstr[i] = pcData[i];
		j++;
	}
	lpstr[j] = '\0';
	return lpstr;
}

bool reg_read_string(HKEY hk, const char* key, char* value) {
	bool ret = false;
	TCHAR lpData[512];
	DWORD lpType = REG_SZ;
	DWORD lpcbData = sizeof(lpData);
	int vret = RegQueryValueEx(hk, C2W(key), 0, &lpType, (BYTE*)lpData, &lpcbData);
	if (vret == ERROR_SUCCESS) {
		if (lpType == REG_SZ) {
			*value = *to_lpchar(lpData, lpcbData);
			ret = true;
		}
	}
	std::free(lpData);
	return ret;
}

bool reg_read_int(HKEY hk, const char* key, int* value) {
	bool ret = false;
	TCHAR lpData[8];
	DWORD lpType = REG_DWORD;
	DWORD lpcbData = sizeof(lpData);
	int vret = RegQueryValueEx(hk, C2W(key), 0, &lpType, (BYTE*)lpData, &lpcbData);
	if (vret == ERROR_SUCCESS) {
		if (lpType == REG_DWORD && lpcbData >= 4) {
			int retv = lpData[0] & 0xFF;
			retv |= ((lpData[1] << 8) & 0xFF00);
			retv |= ((lpData[2] << 16) & 0xFF0000);
			retv |= ((lpData[3] << 24) & 0xFF000000);
			*value = retv;
			ret = true;
		}
	}
	std::free(lpData);
	return ret;
}

void get_net_framework_info(NET_FRAMEWROK_VERSION* pver) {
	try
	{
		HKEY hkv4full;
		int vret = 0;
		REGSAM flag = KEY_WOW64_32KEY;
		if (is_x64_system()) {
			flag = KEY_WOW64_64KEY;
		};
		vret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(NET_V45_PLUS_FULL), 0, flag | KEY_READ, &hkv4full);
		if (vret != ERROR_SUCCESS) {
			pver->Is_V4 = false;
			return;
		}
		pver->Is_V4 = true;
		char* pdata = new char[200];
		if (reg_read_string(hkv4full, "InstallPath", pdata)) {
			strcpy(pver->InstallPath, pdata);
		}
		char* ppdata = new char[50];
		if (reg_read_string(hkv4full, "Version", ppdata)) {
			strcpy(pver->VersionExact, ppdata);
		}
		if (reg_read_int(hkv4full, "Release", &pver->Release)) {
			pver->Is_V45 = true;
			if (pver->Release < NET_V450_RELEASE) {
				strcpy(pver->Version, "4.5-");
			}
			else if (pver->Release > NET_V480_RELEASE) {
				strcpy(pver->Version, "4.8+");
			}
			else {
				std::map<int, char*>::iterator iter;
				iter = net_versions.find(pver->Release);
				if (iter != net_versions.end()) {
					strcpy(pver->Version, iter->second);
				}
			}
		}
		std::free(pdata);
		std::free(ppdata);
		::RegCloseKey(hkv4full);
	}
	catch (const std::exception& ex)
	{
	}
}
void get_net_framework_versionex() {
	NET_FRAMEWROK_VERSION* pver = (NET_FRAMEWROK_VERSION*)malloc(sizeof(NET_FRAMEWROK_VERSION));
	try
	{
		HKEY hkv4full;
		int vret = 0;
		REGSAM flag = KEY_WOW64_32KEY;
		if (is_x64_system()) {
			flag = KEY_WOW64_64KEY;
		};
		vret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(NET_V45_PLUS_FULL), 0, flag | KEY_READ, &hkv4full);
		if (vret != ERROR_SUCCESS) {
			pver->Is_V4 = false;
			return;
		}
		pver->Is_V4 = true;
		char* pdata =new char[200];
		if (reg_read_string(hkv4full, "InstallPath", pdata)) {
			strcpy(pver->InstallPath, pdata);
		}
		char* ppdata = new char[50];
		if (reg_read_string(hkv4full, "Version", ppdata)) {
			strcpy(pver->VersionExact, ppdata);
		}
		if (reg_read_int(hkv4full, "Release", &pver->Release)) {
			pver->Is_V45 = true;
			if (pver->Release < NET_V450_RELEASE) {
				strcpy(pver->Version, "4.5-");
			}
			else if (pver->Release > NET_V480_RELEASE) {
				strcpy(pver->Version, "4.8+");
			}
			else {
				std::map<int, char*>::iterator iter;
				iter = net_versions.find(pver->Release);
				if (iter != net_versions.end()) {
					strcpy(pver->Version, iter->second);
				}
			}
		}
		std::free(pdata);
		std::free(ppdata);
		::RegCloseKey(hkv4full);
		net_version = *pver;
	}
	catch (const std::exception& ex)
	{
	}
}


void get_net_framework_version() {
	NET_FRAMEWROK_VERSION* pver = (NET_FRAMEWROK_VERSION*)malloc(sizeof(NET_FRAMEWROK_VERSION));
	try
	{
		HKEY hkv4full;
		int vret = 0;
		REGSAM flag = KEY_WOW64_32KEY;
		if (is_x64_system()) {
			flag = KEY_WOW64_64KEY;
		};
		vret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT(NET_V45_PLUS_FULL), 0, flag | KEY_READ, &hkv4full);
		if (vret != ERROR_SUCCESS) {
			pver->Is_V4 = false;
			return;
		}
		pver->Is_V4 = true;
		DWORD lpType = REG_DWORD;
		DWORD lpcbData = 256;
		LPBYTE lpData = new BYTE[4];
		//读取 release 项值
		vret = RegQueryValueEx(hkv4full, TEXT("release"), 0, &lpType, lpData, &lpcbData);
		if (vret != ERROR_SUCCESS) {
			pver->Is_V45 = false;
		}
		else {
			pver->Is_V45 = true;
			if (lpType == REG_DWORD && lpcbData >= 4) {
				int ret = 0;
				ret = lpData[0] & 0xFF;
				ret |= ((lpData[1] << 8) & 0xFF00);
				ret |= ((lpData[2] << 16) & 0xFF0000);
				ret |= ((lpData[3] << 24) & 0xFF000000);
				pver->Release = ret;
				if (pver->Release < NET_V450_RELEASE) {
					strcpy((char*)pver->Version, "4.5-");
				}
				else if (pver->Release > NET_V480_RELEASE) {
					strcpy((char*)pver->Version, "4.8+");
				}
				else {
					std::map<int, char*>::iterator iter;
					iter = net_versions.find(pver->Release);
					if (iter != net_versions.end()) {
						strcpy((char*)pver->Version, iter->second);
					}
				}
			}
			else {
				pver->Release = 0;
			}
		}
		//读取 InstallPath 项值
		TCHAR pcData[1024] = { 0 };
		lpType = REG_SZ;
		lpcbData = sizeof(pcData);
		vret = RegQueryValueEx(hkv4full, TEXT("InstallPath"), 0, &lpType, (BYTE*)pcData, &lpcbData);
		if (vret == ERROR_SUCCESS) {
			if (lpType == REG_SZ) {
				char* pstr = to_lpchar(pcData, lpcbData);
				strcpy((char*)pver->InstallPath, pstr);
			}
		}
		vret = RegQueryValueEx(hkv4full, TEXT("Version"), 0, &lpType, (BYTE*)pcData, &lpcbData);
		if (vret == ERROR_SUCCESS) {
			if (lpType == REG_SZ) {
				char* pstr = to_lpchar(pcData, lpcbData);
				strcpy((char*)pver->VersionExact, pstr);
			}
		}
		std::free(pcData);
		::RegCloseKey(hkv4full);
		net_version = *pver;
	}
	catch (const std::exception& ex)
	{
	}
}

bool installed_net_4() {
	return net_version.Is_V4;
}

bool installed_net_45() {
	return net_version.Is_V45;
}

char* get_installed_netframework4_plus_version() {
	char ret[50];
	strcpy(ret, net_version.VersionExact);
	return ret;
}

char* get_installed_path_netframework() {
	char ret[200];
	strcpy(ret, net_version.InstallPath);
	return ret;
}

#pragma endregion

}