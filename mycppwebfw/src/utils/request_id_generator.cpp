#include "mycppwebfw/utils/request_id_generator.h"
#include <random>
#include <sstream>

namespace mycppwebfw
{
namespace utils
{

std::string RequestIdGenerator::generate()
{
    // A simple placeholder implementation.
    // This is not a valid UUIDv4, but it is unique enough for now.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++)
    {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++)
    {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++)
    {
        ss << dis(gen);
    };
    return ss.str();
}

}  // namespace utils
}  // namespace mycppwebfw
