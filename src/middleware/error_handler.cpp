#include "mycppwebfw/middleware/error_handler.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/error_renderer.h"
#include "mycppwebfw/logging/error_logger.h"
#include <exception>

namespace mycppwebfw {
namespace middleware {

void ErrorHandler::operator()(Context& ctx, Next next) {
    try {
        next();
    } catch (const std::exception& e) {
        logging::ErrorLogger::log(e);
        auto& res = ctx.res();
        auto& req = ctx.req();
        std::string accept_header = req.get_header("Accept");
        utils::ErrorRenderer::render(
            res, http::Response::StatusCode::internal_server_error,
            e.what(), accept_header);
    } catch (...) {
        logging::ErrorLogger::log("An unknown error occurred.");
        auto& res = ctx.res();
        auto& req = ctx.req();
        std::string accept_header = req.get_header("Accept");
        utils::ErrorRenderer::render(
            res, http::Response::StatusCode::internal_server_error,
            "Internal Server Error", accept_header);
    }
}

std::shared_ptr<IMiddleware> create_error_handler() {
    return std::make_shared<ErrorHandler>();
}

} // namespace middleware
} // namespace mycppwebfw
