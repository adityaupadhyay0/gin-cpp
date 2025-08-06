#include "file_server.h"
#include <fstream>
#include <sstream>

namespace mycppwebfw
{
namespace utils
{

FileServer::FileServer(const std::string& base_dir) : base_dir_(base_dir)
{
}

void FileServer::handle_request(http::Request& req, http::Response& res)
{
    std::string path = req.get_path();
    std::string prefix = "/" + base_dir_;
    if (path.rfind(prefix, 0) == 0)
    {
        path.erase(0, prefix.length());
    }
    std::string file_path = base_dir_ + path;

    // Basic security to prevent directory traversal
    if (file_path.find("..") != std::string::npos)
    {
        res.status = http::Response::StatusCode::bad_request;
        res.content = "Bad Request";
        return;
    }

    std::ifstream file(file_path, std::ios::in | std::ios::binary);
    if (!file)
    {
        res.status = http::Response::StatusCode::not_found;
        res.content = "Not Found";
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    res.content = buffer.str();
    res.status = http::Response::StatusCode::ok;
    // TODO: Set Content-Type header based on file extension
}

}  // namespace utils
}  // namespace mycppwebfw
