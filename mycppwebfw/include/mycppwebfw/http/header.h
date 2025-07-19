#pragma once

#include <string>

namespace mycppwebfw {
namespace http {

struct Header {
    std::string name;
    std::string value;
};

} // namespace http
} // namespace mycppwebfw
