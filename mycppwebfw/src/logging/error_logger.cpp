#include "error_logger.h"
#include <exception>
#include <string>
#include "utils/logger.h"

namespace mycppwebfw
{
namespace logging
{

void ErrorLogger::log(const std::exception& e)
{
    LOG_ERROR(e.what());
}

void ErrorLogger::log(const std::string& message)
{
    LOG_ERROR(message);
}

}  // namespace logging
}  // namespace mycppwebfw
