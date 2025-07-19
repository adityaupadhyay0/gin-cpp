#include "mycppwebfw/http/response.h"
#include <string>

namespace mycppwebfw {
namespace http {

namespace status_strings {

const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string created =
  "HTTP/1.0 201 Created\r\n";
const std::string accepted =
  "HTTP/1.0 202 Accepted\r\n";
const std::string no_content =
  "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices =
  "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently =
  "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily =
  "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified =
  "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request =
  "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found =
  "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error =
  "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented =
  "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway =
  "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable =
  "HTTP/1.0 503 Service Unavailable\r\n";

asio::const_buffer to_buffer(Response::StatusCode status) {
    switch (status) {
        case Response::StatusCode::ok: return asio::buffer(ok);
        case Response::StatusCode::created: return asio::buffer(created);
        case Response::StatusCode::accepted: return asio::buffer(accepted);
        case Response::StatusCode::no_content: return asio::buffer(no_content);
        case Response::StatusCode::multiple_choices: return asio::buffer(multiple_choices);
        case Response::StatusCode::moved_permanently: return asio::buffer(moved_permanently);
        case Response::StatusCode::moved_temporarily: return asio::buffer(moved_temporarily);
        case Response::StatusCode::not_modified: return asio::buffer(not_modified);
        case Response::StatusCode::bad_request: return asio::buffer(bad_request);
        case Response::StatusCode::unauthorized: return asio::buffer(unauthorized);
        case Response::StatusCode::forbidden: return asio::buffer(forbidden);
        case Response::StatusCode::not_found: return asio::buffer(not_found);
        case Response::StatusCode::internal_server_error: return asio::buffer(internal_server_error);
        case Response::StatusCode::not_implemented: return asio::buffer(not_implemented);
        case Response::StatusCode::bad_gateway: return asio::buffer(bad_gateway);
        case Response::StatusCode::service_unavailable: return asio::buffer(service_unavailable);
        default: return asio::buffer(internal_server_error);
    }
}

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

std::vector<asio::const_buffer> Response::to_buffers() {
    std::vector<asio::const_buffer> buffers;
    buffers.push_back(status_strings::to_buffer(status));
    for (std::size_t i = 0; i < headers.size(); ++i) {
        Header& h = headers[i];
        buffers.push_back(asio::buffer(h.name));
        buffers.push_back(asio::buffer(misc_strings::name_value_separator));
        buffers.push_back(asio::buffer(h.value));
        buffers.push_back(asio::buffer(misc_strings::crlf));
    }
    buffers.push_back(asio::buffer(misc_strings::crlf));
    buffers.push_back(asio::buffer(content));
    return buffers;
}

Response Response::stock_response(Response::StatusCode status) {
    Response resp;
    resp.status = status;
    resp.content = ""; // You can add default content for error pages here
    resp.headers.resize(2);
    resp.headers[0].name = "Content-Length";
    resp.headers[0].value = std::to_string(resp.content.size());
    resp.headers[1].name = "Content-Type";
    resp.headers[1].value = "text/html";
    return resp;
}

} // namespace http
} // namespace mycppwebfw
