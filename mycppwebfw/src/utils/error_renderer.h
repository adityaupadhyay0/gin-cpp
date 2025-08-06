#pragma once

#include <string>
#include "mycppwebfw/http/response.h"

namespace mycppwebfw
{
namespace utils
{

class ErrorRenderer
{
public:
    static void render(http::Response& response,
                       http::Response::StatusCode status_code,
                       const std::string& error_message,
                       const std::string& content_type);

private:
    static void render_html(http::Response& response,
                            http::Response::StatusCode status_code,
                            const std::string& error_message);
    static void render_json(http::Response& response,
                            http::Response::StatusCode status_code,
                            const std::string& error_message);
    static void render_text(http::Response& response,
                            http::Response::StatusCode status_code,
                            const std::string& error_message);
};

}  // namespace utils
}  // namespace mycppwebfw
