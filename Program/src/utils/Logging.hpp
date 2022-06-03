#pragma once

#include <iostream>

#define LOG_TARGET std::cout

#define _LOG_DEBUG true
#define LOG_DEBUG() if(_LOG_DEBUG) LOG_TARGET 

#define _LOG_INFO true
#define LOG_INFO() if(_LOG_INFO) LOG_TARGET 

