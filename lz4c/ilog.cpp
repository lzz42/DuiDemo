#pragma warning(disable:4996)

//#ifdef  __has_include
//#if __has_include("stdafx.h")
//#include "stdafx.h"
//#elif __has_include("pch.h")
//#include "pch.h"
//#endif
//#endif //__has_include

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

//#include "stdafx.h"
//#include "pch.h"
#include "ilog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/async.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

namespace ilog {
	char* to_pchar(LPCTSTR lpstr) {
		int num = WideCharToMultiByte(CP_OEMCP, NULL, lpstr, -1, NULL, 0, NULL, FALSE);
		char* pchar = new char[num];
		WideCharToMultiByte(CP_OEMCP, NULL, lpstr, -1, pchar, num, NULL, FALSE);
		return pchar;
	}
	char* get_app_name() {
		char* name = _pgmptr;
		if (name == NULL) {
			WCHAR path[MAX_PATH];
			GetModuleFileName(NULL, path, MAX_PATH);
			name = to_pchar(path);
		}
		if (name == NULL) {
			name = "DuiDemo.exe";
		}
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


	void init_log(char* dir, char* file) {
		spdlog::cfg::load_env_levels();
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_every(std::chrono::seconds(3));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S %f] [PID %P] [TID %t] [%^%l%$] %v");
		spdlog::enable_backtrace(100);

		char* app;
		char* logfile;
		if (file == NULL) {
			logfile = get_app_name();
			strcat(logfile, ".log");
		}
		else {
			logfile = (char*)malloc(sizeof(char) * 256);
			strcpy(logfile, file);
		}
		if (dir != NULL) {
			app = (char*)malloc(sizeof(char) * 256);
			strcpy(app, dir);
			strcat(app, logfile);
			free(logfile);
		}
		else {
			app = logfile;
		}
		auto basic_logger = spdlog::basic_logger_mt("basic_logger", app);
		basic_logger->flush_on(spdlog::level::err);
		spdlog::set_default_logger(basic_logger);

		SPD_INFO("Init logger");
		free(app);
	}

	void shutdown_log() {
		SPD_INFO("Shutdown logger");
		spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("log shout down."); });
		spdlog::drop_all();
		spdlog::shutdown();
	}
}