#pragma once

#include <string>

namespace mycppwebfw {
namespace utils {

class JwtParser {
public:
    JwtParser(const std::string& secret);

    bool verify(const std::string& token, std::string& payload);

private:
    std::string secret_;
};

} // namespace utils
} // namespace mycppwebfw
