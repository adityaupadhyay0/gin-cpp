#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "mycppwebfw/http/header.h"

namespace mycppwebfw
{
namespace http
{

class Request
{
public:
    std::string get_method() const;
    std::string get_path() const;
    std::string get_header(const std::string& name) const;
    std::unordered_map<std::string, std::string> get_query_params() const;

    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
    std::string body;
};

}  // namespace http
}  // namespace mycppwebfw
