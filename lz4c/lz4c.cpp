// lz4c.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include "lz4.h"

//spdlog
#include <cstdio>
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/ostr.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"

#include "spdlog/fmt/bin_to_hex.h"
namespace spd = spdlog;
using namespace std;

bool zip_file(char* infile, char* outfile);
char* get_app_name();

#define SPD_INFO spdlog::info
#define SPD_TRACE spdlog::trace
#define SPD_WARN spdlog::warn
#define SPD_ERROR spdlog::error
#define SPD_CRITICAL spdlog::critical
void init_log() {
	spdlog::cfg::load_env_levels();
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_every(std::chrono::seconds(3));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S %f] [PID %P] [TID %t] [%^%l%$] %v");
	spdlog::enable_backtrace(50);

	char* app = get_app_name();
	strcat(app, ".log");
	auto basic_logger = spdlog::basic_logger_mt("basic_logger", app);
	basic_logger->flush_on(spdlog::level::err);
	spdlog::set_default_logger(basic_logger);

	SPD_INFO("Init logger");
}

void shutdown_log() {
	SPD_INFO("Shutdown logger");
	spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("log shout down."); });
	spdlog::shutdown();
}

void printf_cmd()
{
	printf("lz4c -f/-d:[in] -o:[out] -c:[code]\n");
	printf("\t -c [code] \t :指定压缩编码格式（默认utf-8）支持：utf-8、unicode、gbk、gb2312\n");
}

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
	SPD_ERROR("Before Reverse Len:{} half {}", k, (k - 1) / 2);
	SPD_ERROR(ret);
	char pt;
	for (int j = 0; j < (k-1) / 2; j++)
	{
		pt = ret[j];
		ret[j] = ret[k - j - 2];
		ret[k - j - 2] = pt;
	}
	SPD_ERROR("After Reverse");
	SPD_ERROR(ret);
	return ret;
}

void reverse_lpchar(char*  lpchar) {
	char* pchar = lpchar;
	int len = strlen(pchar);
	if (len <= 0)
		return;
	char pt;
	for (int j = 0; j < (len - 1) / 2; j++)
	{
		pt = pchar[j];
		pchar[j] = pchar[len - j - 2];
		pchar[len - j - 2] = pt;
	}
}

int main(int argc, char* argv[], char** env)
{
	init_log();
	SPD_TRACE(_pgmptr);
	SPD_CRITICAL("----------");
	SPD_WARN(_pgmptr);
	SPD_CRITICAL("----------");
	SPD_INFO(_pgmptr);
	SPD_CRITICAL("----------");
	SPD_ERROR(LZ4_versionNumber());
	SPD_CRITICAL("----------");
	SPD_CRITICAL(_pgmptr);
	SPD_CRITICAL("----------");
	SPD_CRITICAL(get_app_name());

	char* hh = "hello my world!";
	SPD_CRITICAL(hh);
	reverse_lpchar(hh);
	SPD_CRITICAL(hh);
	printf("EXE:: %s \n", _pgmptr);
	printf("PATH:: %s \n", _pgmptr);
	printf("lz4 num:\t%d\n", LZ4_versionNumber());
	printf("lz4 str:\t%s\n", LZ4_versionString());
	if (argc == 0 || argv == NULL)
	{
		printf_cmd();
		return 0;
	}
	else
	{
		for (size_t i = 0; i < argc; i++)
		{
			printf("\t %s \n", argv[i]);
		}
		/*int j = 0;
		while (env[j] != NULL)
		{
			printf("\t %s \n", env[j]);
			j++;
		}*/
	}
	/*char* infile = "D:\\WIFI2.txt";
	zip_file(infile, NULL);*/
	char chs[100];
	gets_s(chs);
	shutdown_log();
	return 0;
}


bool zip_file(char* infile, char* outfile)
{
	FILE* pfile = NULL, * ofile = NULL;
	pfile = fopen(infile, "r");
	if (pfile == NULL)
		return false;
	if (outfile == NULL) {
		size_t len = strlen(infile);
		outfile = (char*)malloc(sizeof(char) * (len + 5));
		strcpy(outfile, infile);
		outfile[len] = '.';
		outfile[len + 1] = 'z';
		outfile[len + 2] = 'i';
		outfile[len + 3] = 'p';
		outfile[len + 4] = '\0';
	}
	printf("\t IN:\t %s\n", infile);
	printf("\t OU:\t %s\n", outfile);
	ofile = fopen(outfile, "w");
	char* buff = (char*)malloc(sizeof(char) * 1024 * 300);
	char* dst = (char*)malloc(sizeof(char) * 1024 * 300);
	size_t dstsize = 1024 * 300;
	printf("*********************\n");
	while (!feof(pfile))
	{
		fgets(buff, sizeof(buff) - 1, pfile);
		printf("\t%s\n", buff);
		LZ4_compress_default(buff, dst, sizeof(buff) - 1, dstsize);
		fputs(dst, ofile);
	}
	printf("*********************\n");
	free(buff);
	free(dst);
	if (pfile != NULL) {
		fclose(pfile);
	}
	if (ofile != NULL) {
		fclose(ofile);
	}
	return true;
}

bool unzip_file(char* infile, char* outfile)
{
	FILE* pfile = NULL;
	pfile = fopen(infile, "r");
	if (pfile == NULL)
		return false;

	if (outfile == NULL) {
		size_t len = strlen(infile);
		outfile = (char*)malloc(sizeof(char) * (len - 4));
		strcpy(outfile, infile);
		outfile[len] = '\0';
	}


	printf("\t IN:\t %s\n", infile);
	printf("\t OU:\t %s\n", outfile);
	if (pfile != NULL) {
		fclose(pfile);
	}
	return true;
}