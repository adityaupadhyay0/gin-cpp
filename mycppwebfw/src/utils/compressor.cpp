#include "compressor.h"
#include <stdexcept>
#include <vector>
#include <cstring>
#include <zlib.h>

namespace mycppwebfw
{
namespace utils
{

std::string GzipCompressor::compress(const std::string& data)
{
    if (data.empty())
    {
        return "";
    }

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8,
                     Z_DEFAULT_STRATEGY) != Z_OK)
    {
        throw std::runtime_error("deflateInit2 failed while compressing.");
    }

    zs.next_in = (Bytef*)data.data();
    zs.avail_in = data.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    do
    {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out)
        {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END)
    {
        throw std::runtime_error("Exception during zlib compression: (" +
                                 std::to_string(ret) + ") " + zs.msg);
    }

    return outstring;
}

}  // namespace utils
}  // namespace mycppwebfw
