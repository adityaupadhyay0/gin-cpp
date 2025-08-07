#pragma once

#include <string>

namespace mycppwebfw {
namespace utils {

class RequestIdGenerator {
public:
    static std::string generate();
};

} // namespace utils
} // namespace mycppwebfw
