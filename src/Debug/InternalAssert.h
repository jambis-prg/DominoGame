#pragma once

#include "../Utils/StringFormat.h"
#include "Log.h"
#include <cstdlib>

#ifdef _DEBUG
    #define ASSERT(x, ...) \
    if(x) {}\
    else \
    {\
        std::string formated_message = StringFormat(__VA_ARGS__);\
        LOG_FATAL("Assertion (%s) Failed in %s line %d - %s", #x, __FILE__, __LINE__, formated_message.c_str());\
        __builtin_trap();\
        exit(1);\
    }
#else
    #define ASSERT(x, ...)
#endif