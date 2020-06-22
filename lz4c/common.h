#pragma once

#include <windows.h>
#include <winnt.h>
#include <direct.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stringapiset.h>
#include <vector>
#include <map>

#ifdef __AFXSTR_H__

CString get_app_dir_cstring();

CString get_app_path_cstring();

LPCTSTR to_lpctstr(const char* chars);
vector<CString> ini_read_sections(LPCTSTR inifilepath);

map<CString, CString> ini_read_keys_values(LPCTSTR section, LPCTSTR inifilepath);

CString ini_read_value_cstring(LPCTSTR key, LPCTSTR section, LPCTSTR inifile);
CString ini_read_value_cstring(const char* key, const char* section, const char* ini);

CString ini_read_value_cstring(const char* key, const char* default, const char* section, const char* ini);

bool c_unzip(CString srcfile, CString dstpath);

#endif //__AFXSTR_H__

#pragma region register

#include <winreg.h>
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

typedef struct NET_FRAMEWROK_VERSION
{
	bool Is_V4;
	bool Is_V45;
	int Release;
	char Version[50];
	char VersionExact[50];
	char InstallPath[200];
};

#pragma endregion

namespace common{

	char* get_app_path();
	char* get_app_dir();
	char* to_pchar(LPCTSTR lpstr);
	int ini_read_value_int(LPCTSTR key, LPCTSTR section, LPCTSTR inifile);
	bool ini_write(LPCTSTR section, LPCTSTR key, LPCTSTR value, LPCTSTR inifile);
	bool ini_write(LPCTSTR section, LPCTSTR data, LPCTSTR inifile);

	bool ini_delete(LPCTSTR section, LPCTSTR inifile, bool bdeletesection);

	bool ini_write(const char* section, const char* key, const char* value, const char* inifile);

	bool ini_write(const char* section, const char* data, const char* inifile);

	bool ini_delete(const char* section, const char* inifile, bool bdeletesection);

	int ini_read_value_int(const char* key, const char* section, const char* ini);

	char* unicode_to_utf8(TCHAR* pchars);

	char* unicode_to_gb2312(TCHAR* pchars);

	char* unicode_to_ascii(TCHAR* pchars);

	char* unicode_to_Ansii(const wchar_t* szStr);

	char* unicode_to_utf8(const wchar_t* in, int insize);

	void get_net_framework_versionex();

	void get_net_framework_version();
}