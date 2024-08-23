#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include <cstdarg>

// Site de onde peguei a função -> https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
// Lembrando que essa função pode ser substituída por std::format se estiver sendo usado o c++ 20

std::string StringFormat(const std::string &format, va_list args);

std::string StringFormat(const std::string &format, ...);
