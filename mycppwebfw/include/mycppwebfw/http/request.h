#pragma once

#include <string>
#include <vector>
#include "mycppwebfw/http/header.h"

namespace mycppwebfw {
namespace http {

struct Request {
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
    std::string body;
};

} // namespace http
} // namespace mycppwebfw
