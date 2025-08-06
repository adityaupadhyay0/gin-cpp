#include "mycppwebfw/middleware/error_handler.h"
#include <exception>
#include <functional>
#include <string>
#include "logging/error_logger.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "utils/error_renderer.h"

namespace mycppwebfw
{
namespace middleware
{

Middleware create_error_handler()
{
    Middleware mw;
    mw.priority = 100;  // High priority to run first
    mw.handler = [](http::Request& req, http::Response& res, Next next)
    {
        try
        {
            next();
        }
        catch (const std::exception& e)
        {
            logging::ErrorLogger::log(e);
            std::string accept_header = req.get_header("Accept");
            utils::ErrorRenderer::render(
                res, http::Response::StatusCode::internal_server_error,
                e.what(), accept_header);
        }
        catch (...)
        {
            logging::ErrorLogger::log("An unknown error occurred.");
            std::string accept_header = req.get_header("Accept");
            utils::ErrorRenderer::render(
                res, http::Response::StatusCode::internal_server_error,
                "Internal Server Error", accept_header);
        }
    };
    return mw;
}

}  // namespace middleware
}  // namespace mycppwebfw
