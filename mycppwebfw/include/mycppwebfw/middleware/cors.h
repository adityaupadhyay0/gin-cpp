#pragma once

#include <memory>
#include "middleware.h"
#include "utils/cors_policy.h"

namespace mycppwebfw
{
namespace middleware
{

class Cors : public Middleware
{
public:
    Cors(const utils::CorsPolicy& policy);
    ~Cors();

    void operator()(const Request& req, Response& res,
                    std::function<void()> next) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace middleware
}  // namespace mycppwebfw
