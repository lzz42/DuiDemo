#pragma once

//spdlog
#include <iostream>
#include "spdlog/spdlog.h"

#define SPD_INFO spdlog::info
#define SPD_TRACE spdlog::trace
#define SPD_WARN spdlog::warn
#define SPD_ERROR spdlog::error
#define SPD_CRITICAL spdlog::critical

namespace ilog {
	void init_log(char* dir = NULL, char* file = NULL);
	void shutdown_log();
}
