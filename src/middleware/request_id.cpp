#include "mycppwebfw/middleware/request_id.h"
#include "mycppwebfw/utils/request_id_generator.h"

namespace mycppwebfw {
namespace middleware {

void RequestId::operator()(Context& ctx, Next next) {
    auto& req = ctx.req();
    auto& res = ctx.res();
    auto request_id_header = req.get_header("X-Request-ID");
    if (!request_id_header.empty()) {
        ctx.set("request_id", request_id_header);
    } else {
        auto request_id = utils::RequestIdGenerator::generate();
        ctx.set("request_id", request_id);
        res.headers.push_back({"X-Request-ID", request_id});
    }
    next();
}

std::shared_ptr<IMiddleware> create_request_id_middleware() {
    return std::make_shared<RequestId>();
}

} // namespace middleware
} // namespace mycppwebfw
