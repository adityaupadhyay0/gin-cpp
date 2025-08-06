#include "cors_policy.h"

namespace mycppwebfw
{
namespace utils
{

bool CorsPolicy::is_origin_allowed(const std::string& origin) const
{
    if (allowed_origins.count("*"))
    {
        return true;
    }
    return allowed_origins.count(origin) > 0;
}

}  // namespace utils
}  // namespace mycppwebfw
