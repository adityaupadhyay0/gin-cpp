#pragma once

#include <string>
#include <vector>

namespace mycppwebfw
{
namespace utils
{

class Compressor
{
public:
    virtual ~Compressor() = default;
    virtual std::string compress(const std::string& data) = 0;
    virtual std::string get_encoding() const = 0;
};

class GzipCompressor : public Compressor
{
public:
    std::string compress(const std::string& data) override;
    std::string get_encoding() const override
    {
        return "gzip";
    }
};

}  // namespace utils
}  // namespace mycppwebfw
