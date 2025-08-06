#pragma once

#include "middleware/cors.h"
#include "utils/cors_policy.h"

namespace mycppwebfw
{
namespace middleware
{

struct Cors::Impl
{
    Impl(const utils::CorsPolicy& policy) : policy_(policy)
    {
    }
    utils::CorsPolicy policy_;
};

}  // namespace middleware
}  // namespace mycppwebfw
