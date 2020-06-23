// lz4c.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include "lz4.h"
//#include "common.h"
#include "ilog.h"
#include "func.h"

bool zip_file(char* infile, char* outfile);

void printf_cmd()
{
	printf("lz4c -f/-d:[in] -o:[out] -c:[code]\n");
	printf("\t -c [code] \t :指定压缩编码格式（默认utf-8）支持：utf-8、unicode、gbk、gb2312\n");
}

char* reverse_pchar(char* lpchar) {
	int len = strlen(lpchar);
	if (len <= 0)
		return NULL;
	char* pchar = (char*)malloc(sizeof(char) * len);
	for (int i = len - 1; i >= 0; i--)
	{
		pchar[len - i - 1] = lpchar[i];
	}
	pchar[len] = '\0';
	return pchar;
}

int env_test(int argc, char* argv[], char** env) {
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
	return 0;
}

int log_test(int argc, char* argv[], char** env) {
	ilog::init_log("D:\\Temp\\", "0087.log");
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
	try
	{
		char* hh = "hello my world!";
		SPD_CRITICAL(hh);
		char* rhh = reverse_pchar(hh);
		int len = strlen(rhh);
		if (len > 0 && len < 100) {
			SPD_CRITICAL(rhh);
		}
		printf("EXE:: %s \n", _pgmptr);
		printf("PATH:: %s \n", _pgmptr);
		printf("lz4 num:\t%d\n", LZ4_versionNumber());
		printf("lz4 str:\t%s\n", LZ4_versionString());
	}
	catch (const std::exception& ex)
	{
		SPDLOG_ERROR(ex.what());
	}
	ilog::shutdown_log();
	return 0;
}

int func_test(int argc, char* argv[], char** env) {
	func::func_main();
	return 0;
}

int main(int argc, char* argv[], char** env)
{
	//return env_test(argc, argv, env);
	//return log_test(argc, argv, env);
	return func_test(argc, argv, env);
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