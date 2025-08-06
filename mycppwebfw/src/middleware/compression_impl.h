#pragma once

#include <memory>
#include "utils/compressor.h"

namespace mycppwebfw {
namespace middleware {

struct Compression::Impl {
    Impl(std::unique_ptr<utils::Compressor> compressor)
        : compressor_(std::move(compressor)) {}

    std::unique_ptr<utils::Compressor> compressor_;
};

} // namespace middleware
} // namespace mycppwebfw
