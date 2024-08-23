#include "StringFormat.h"

std::string StringFormat(const std::string &format, va_list args)
{
    int size_s = std::vsnprintf(nullptr, 0, format.c_str(), args) + 1; // Extra space for '\0'
    if (size_s <= 0)
        throw std::runtime_error("Error during formatting.");
    
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);

    std::vsnprintf(buf.get(), size, format.c_str(), args);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::string StringFormat(const std::string &format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = StringFormat(format, args);
    va_end(args);

    return s;
}
