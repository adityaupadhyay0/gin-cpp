#pragma once

#include <memory>
#include "mycppwebfw/middleware/middleware.h"
#include "mycppwebfw/utils/session_store.h"

namespace mycppwebfw
{
namespace middleware
{

Middleware
create_session_middleware(std::shared_ptr<utils::SessionStore> store);

}  // namespace middleware
}  // namespace mycppwebfw
