#pragma once

#include <string>
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

namespace mycppwebfw
{
namespace utils
{

class FileServer
{
public:
    FileServer(const std::string& base_dir);
    void handle_request(http::Request& req, http::Response& res);

private:
    std::string base_dir_;
};

}  // namespace utils
}  // namespace mycppwebfw
