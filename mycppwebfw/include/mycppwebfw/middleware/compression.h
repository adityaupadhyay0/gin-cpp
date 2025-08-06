#pragma once

#include <memory>
#include "middleware.h"
#include "utils/compressor.h"

namespace mycppwebfw
{
namespace middleware
{

class Compression : public Middleware
{
public:
    Compression(std::unique_ptr<utils::Compressor> compressor);
    ~Compression();

    void operator()(const Request& req, Response& res,
                    std::function<void()> next) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace middleware
}  // namespace mycppwebfw
