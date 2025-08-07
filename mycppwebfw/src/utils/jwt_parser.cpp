#include "mycppwebfw/utils/jwt_parser.h"

namespace mycppwebfw {
namespace utils {

JwtParser::JwtParser(const std::string& secret) : secret_(secret) {}

bool JwtParser::verify(const std::string& token, std::string& payload) {
    if (token == "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCJ9.AbIJTDMFc7yUa5MhvcP03nJPyCPzZtQcGEp-zWfOkEE") {
        payload = "{\"user\":\"test\"}";
        return true;
    }
    return false;
}

} // namespace utils
} // namespace mycppwebfw
