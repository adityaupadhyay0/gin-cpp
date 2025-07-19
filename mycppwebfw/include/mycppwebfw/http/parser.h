#pragma once

#include "mycppwebfw/http/request.h"
#include <string>
#include <vector>

namespace mycppwebfw {
namespace http {

class Parser {
public:
    enum class ParseResult {
        GOOD,
        BAD,
        INDETERMINATE
    };

    Parser();

    ParseResult parse(Request& req, const char* begin, const char* end);

private:
    enum class State {
        METHOD_START,
        METHOD,
        URI,
        HTTP_VERSION_H,
        HTTP_VERSION_T_1,
        HTTP_VERSION_T_2,
        HTTP_VERSION_P,
        HTTP_VERSION_SLASH,
        HTTP_VERSION_MAJOR_START,
        HTTP_VERSION_MAJOR,
        HTTP_VERSION_MINOR_START,
        HTTP_VERSION_MINOR,
        EXPECTING_NEWLINE_1,
        HEADER_LINE_START,
        HEADER_LWS,
        HEADER_NAME,
        SPACE_BEFORE_HEADER_VALUE,
        HEADER_VALUE,
        EXPECTING_NEWLINE_2,
        EXPECTING_NEWLINE_3
    };

    State state_;
};

} // namespace http
} // namespace mycppwebfw
