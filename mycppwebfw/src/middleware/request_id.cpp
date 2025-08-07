#include "mycppwebfw/middleware/request_id.h"
#include <thread>
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/request_id_generator.h"

namespace mycppwebfw
{
namespace middleware
{

// This is a simplified thread-local storage for the request ID.
// In a real application, this would be part of a more comprehensive context
// object.
thread_local std::string current_request_id;

Middleware create_request_id_middleware()
{
    Middleware mw;
    mw.priority = 90;  // High priority, but after error handler
    mw.handler = [](http::Request& req, http::Response& res, Next next)
    {
        auto it = req.get_header("X-Request-ID");
        if (!it.empty())
        {
            current_request_id = it;
        }
        else
        {
            current_request_id = utils::RequestIdGenerator::generate();
        }

        res.headers.push_back({"X-Request-ID", current_request_id});

        if (next)
        {
            next();
        }
    };
    return mw;
}

}  // namespace middleware
}  // namespace mycppwebfw
