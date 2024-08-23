#include "Log.h"

#include "../Utils/StringFormat.h"
#include <string>
#include <iostream>
#include <ctime>

#define INFO_COLOR  "0;37;40"
#define DEBUG_COLOR "0;34;40"
#define WARN_COLOR  "1;33;40"
#define ERROR_COLOR "1;31;40"
#define FATAL_COLOR "0;30;41"

void Log(LogTypes type, const char *message, ...)
{
    std::string typeString, color;
    switch (type)
    {
    case LogTypes::Info:
        typeString = "Info";
        color = INFO_COLOR;
        break;
    case LogTypes::Debug:
        typeString = "Debug";
        color = DEBUG_COLOR;
        break;
    case LogTypes::Warning:
        typeString = "Warning";
        color = WARN_COLOR;
        break;
    case LogTypes::Error:
        typeString = "Error";
        color = ERROR_COLOR;
        break;
    case LogTypes::Fatal:
        typeString = "Fatal";
        color = FATAL_COLOR;
        break;    
    default:
        break;
    }

    va_list args;
    va_start(args, message);

    std::string formated_message = StringFormat(std::string(message), args);

    va_end(args);

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%H:%M:%S",timeinfo);
    std::string time(buffer);

    std::cout << "\033[" + color + "m";
    std::cout << "[" + time + "][" + typeString + "]: " << formated_message << "\n";
    std::cout << "\033[0m";
}
