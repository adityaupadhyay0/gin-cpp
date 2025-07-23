#include "mycppwebfw/http/request.h"
#include <algorithm>

namespace mycppwebfw
{
namespace http
{

std::string Request::get_method() const
{
    return method;
}

std::string Request::get_path() const
{
    size_t pos = uri.find('?');
    if (pos != std::string::npos)
    {
        return uri.substr(0, pos);
    }
    return uri;
}

std::string Request::get_header(const std::string& name) const
{
    auto it =
        std::find_if(headers.begin(), headers.end(),
                     [&](const Header& header) { return header.name == name; });
    if (it != headers.end())
    {
        return it->value;
    }
    return "";
}

std::unordered_map<std::string, std::string> Request::get_query_params() const
{
    std::unordered_map<std::string, std::string> params;
    size_t pos = uri.find('?');
    if (pos != std::string::npos)
    {
        std::string query_string = uri.substr(pos + 1);
        std::string key;
        std::string value;
        size_t start = 0;
        for (size_t i = 0; i < query_string.length(); ++i)
        {
            if (query_string[i] == '=')
            {
                key = query_string.substr(start, i - start);
                start = i + 1;
            }
            else if (query_string[i] == '&')
            {
                value = query_string.substr(start, i - start);
                params[key] = value;
                start = i + 1;
            }
        }
        value = query_string.substr(start);
        params[key] = value;
    }
    return params;
}

}  // namespace http
}  // namespace mycppwebfw
