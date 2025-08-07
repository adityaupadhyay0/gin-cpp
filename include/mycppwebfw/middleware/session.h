#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include "mycppwebfw/utils/session_store.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

class Session : public IMiddleware {
public:
    Session(std::shared_ptr<utils::SessionStore> store);

    void operator()(Context& ctx, Next next) override;

private:
    std::shared_ptr<utils::SessionStore> store_;
};

std::shared_ptr<IMiddleware> create_session_middleware(
    std::shared_ptr<utils::SessionStore> store);

} // namespace middleware
} // namespace mycppwebfw
