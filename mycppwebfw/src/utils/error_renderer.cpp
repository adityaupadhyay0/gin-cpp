#include "error_renderer.h"
#include <iostream>
#include <string>
#include "mycppwebfw/http/header.h"

namespace mycppwebfw
{
namespace utils
{

void ErrorRenderer::render(http::Response& response,
                           http::Response::StatusCode status_code,
                           const std::string& error_message,
                           const std::string& content_type)
{
    if (content_type.find("application/json") != std::string::npos)
    {
        render_json(response, status_code, error_message);
    }
    else if (content_type.find("text/html") != std::string::npos)
    {
        render_html(response, status_code, error_message);
    }
    else
    {
        render_text(response, status_code, error_message);
    }
}

void ErrorRenderer::render_html(http::Response& response,
                                http::Response::StatusCode status_code,
                                const std::string& error_message)
{
    response.status = status_code;
    response.headers.emplace_back(http::Header{"Content-Type", "text/html"});
    response.content = "<html><head><title>Error</title></head><body><h1>" +
                       std::to_string(static_cast<int>(status_code)) +
                       "</h1><p>" + error_message + "</p></body></html>";
}

void ErrorRenderer::render_json(http::Response& response,
                                http::Response::StatusCode status_code,
                                const std::string& error_message)
{
    response.status = status_code;
    response.headers.emplace_back(
        http::Header{"Content-Type", "application/json"});
    response.content =
        "{\"status\": " + std::to_string(static_cast<int>(status_code)) +
        ", \"error\": \"" + error_message + "\"}";
}

void ErrorRenderer::render_text(http::Response& response,
                                http::Response::StatusCode status_code,
                                const std::string& error_message)
{
    response.status = status_code;
    response.headers.emplace_back(http::Header{"Content-Type", "text/plain"});
    response.content =
        std::to_string(static_cast<int>(status_code)) + " " + error_message;
}

}  // namespace utils
}  // namespace mycppwebfw
