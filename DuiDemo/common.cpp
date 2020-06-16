
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "pch.h"
#include "common.h"

using std::vector;
using std::map;

#pragma region env info

void debug_info(const char* msg) {
	WCHAR wsz[256];
	swprintf(wsz, L"%S", msg);
	OutputDebugString(L"*****************************");
	OutputDebugString(wsz);
	OutputDebugString(L"*****************************");
	free(wsz);
}

/// <summary>
/// 获取当前进程执行文件的全路径
/// </summary>
/// <returns></returns>
char* get_app_path() {
#ifdef PCH_H
	char* path = (char*)malloc(sizeof(char) * MAX_PATH);
	_get_pgmptr(&path);
	return path;
#else
	return _pgmptr;
#endif
}

/// <summary>
/// 获取当前进程执行文件所在目录
/// </summary>
/// <returns></returns>
char* get_app_dir() {
	char buf1[256];
	_getcwd(buf1, sizeof(buf1));

	debug_info(buf1);

	return buf1;
}

CString get_app_dir_cstring() {
	return CString(get_app_dir());
}

CString get_app_path_cstring() {
	return CString(get_app_path());
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

#pragma endregion

#pragma region ini file program

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

CString ini_read_value_cstring(const char* key, const char* section, const char* ini) {
	return ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
}

int ini_read_value_int(const char* key, const char* section, const char* ini) {
	return ini_read_value_int(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
}

CString ini_read_value_cstring(const char* key, const char* default, const char* section, const char* ini) {
	CString res = ini_read_value_cstring(to_lpctstr(key), to_lpctstr(section), to_lpctstr(ini));
	if (res.IsEmpty()) {
		res = CString(default);
	}
	return res;
}
#pragma endregion

#pragma region coding convert

char* unicode_to_utf8(TCHAR* pchars) {
	int num = WideCharToMultiByte(CP_UTF8, NULL, pchars, -1, NULL, 0, NULL, FALSE);
	char* pchar = new char[num];
	WideCharToMultiByte(CP_UTF8, NULL, pchars, -1, pchar, num, NULL, FALSE);

	debug_info(pchar);

	return pchar;
}

char* unicode_to_gb2312(TCHAR* pchars) {
	int n = WideCharToMultiByte(CP_THREAD_ACP, 0, pchars, -1, 0, 0, 0, 0);
	char* pchar = new char[n];
	WideCharToMultiByte(CP_THREAD_ACP, 0, pchars, -1, pchar, n, 0, 0);

	debug_info(pchar);

	return pchar;
}

char* unicode_to_ascii(TCHAR* pchars) {
	////将异常中的乱码转为正常的中文
	//int nwLen = MultiByteToWideChar(CP_UTF8, 0, pchars, -1, NULL, 0);//算出合适的长度
	//LPWSTR strUnicode = new WCHAR[nwLen];
	//MultiByteToWideChar(CP_UTF8, 0, pchars, -1, strUnicode, nwLen);//ASCII转UNICODE

	int nLen = WideCharToMultiByte(CP_ACP, 0, pchars, -1, NULL, 0, "", false);//算出合适的长度
	char* pchar = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, pchars, -1, pchar, nLen, "", false);//UNICODE转ASCII

	debug_info(pchar);

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

	debug_info(pchar);

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
	free(result);
	return res;
}

#pragma endregion