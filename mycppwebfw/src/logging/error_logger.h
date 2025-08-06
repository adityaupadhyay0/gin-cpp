#pragma once

#include <exception>
#include <string>

namespace mycppwebfw
{
namespace logging
{

class ErrorLogger
{
public:
    static void log(const std::exception& e);
    static void log(const std::string& message);
};

}  // namespace logging
}  // namespace mycppwebfw
