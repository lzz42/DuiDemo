#pragma once

#include <direct.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stringapiset.h>
#include <vector>
#include <map>

char* get_app_path();

char* get_app_dir();

LPCTSTR to_lpctstr(const char* chars);

char* to_pchar(LPCTSTR lpstr);

vector<char*> ini_read_sections(LPCTSTR inifilepath);

map<char*, char*> ini_read_keys_values(LPCTSTR section, LPCTSTR inifilepath);

char* ini_read_value_cstring(LPCTSTR key, LPCTSTR section, LPCTSTR inifile);

int ini_read_value_int(LPCTSTR key, LPCTSTR section, LPCTSTR inifile);

bool ini_write(LPCTSTR section, LPCTSTR key, LPCTSTR value, LPCTSTR inifile);

bool ini_write(LPCTSTR section, LPCTSTR data, LPCTSTR inifile);

bool ini_delete(LPCTSTR section, LPCTSTR inifile, bool bdeletesection);

bool ini_write(const char* section, const char* key, const char* value, const char* inifile);

bool ini_write(const char* section, const char* data, const char* inifile);

bool ini_delete(const char* section, const char* inifile, bool bdeletesection);

char* ini_read_value_cstring(const char* key, const char* section, const char* ini);

int ini_read_value_int(const char* key, const char* section, const char* ini);

char* ini_read_value_cstring(const char* key, const char* default, const char* section, const char* ini);

char* unicode_to_utf8(TCHAR * pchars);

char* unicode_to_gb2312(TCHAR * pchars);

char* unicode_to_ascii(TCHAR * pchars);

char* unicode_to_Ansii(const wchar_t* szStr);

char* unicode_to_utf8(const wchar_t* in, int insize);

//bool c_unzip(char* srcfile, char* dstpath);

void get_net_framework_version();

