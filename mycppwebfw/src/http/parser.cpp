#include "mycppwebfw/http/parser.h"
#include "mycppwebfw/http/request.h"

namespace mycppwebfw {
namespace http {

Parser::Parser() : state_(State::METHOD_START) {}

Parser::ParseResult Parser::parse(Request& req, const char* begin, const char* end) {
    while (begin != end) {
        char input = *begin++;
        switch (state_) {
            case State::METHOD_START:
                if (!isalpha(input)) {
                    return ParseResult::BAD;
                }
                req.method.push_back(input);
                state_ = State::METHOD;
                break;
            case State::METHOD:
                if (input == ' ') {
                    state_ = State::URI;
                } else if (!isalpha(input)) {
                    return ParseResult::BAD;
                } else {
                    req.method.push_back(input);
                }
                break;
            case State::URI:
                if (input == ' ') {
                    state_ = State::HTTP_VERSION_H;
                } else if (iscntrl(input)) {
                    return ParseResult::BAD;
                } else {
                    req.uri.push_back(input);
                }
                break;
            case State::HTTP_VERSION_H:
                if (input == 'H') {
                    state_ = State::HTTP_VERSION_T_1;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_T_1:
                if (input == 'T') {
                    state_ = State::HTTP_VERSION_T_2;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_T_2:
                if (input == 'T') {
                    state_ = State::HTTP_VERSION_P;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_P:
                if (input == 'P') {
                    state_ = State::HTTP_VERSION_SLASH;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_SLASH:
                if (input == '/') {
                    state_ = State::HTTP_VERSION_MAJOR_START;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_MAJOR_START:
                if (isdigit(input)) {
                    req.http_version_major = input - '0';
                    state_ = State::HTTP_VERSION_MAJOR;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_MAJOR:
                if (input == '.') {
                    state_ = State::HTTP_VERSION_MINOR_START;
                } else if (isdigit(input)) {
                    req.http_version_major = req.http_version_major * 10 + (input - '0');
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_MINOR_START:
                if (isdigit(input)) {
                    req.http_version_minor = input - '0';
                    state_ = State::HTTP_VERSION_MINOR;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HTTP_VERSION_MINOR:
                if (input == '\r') {
                    state_ = State::EXPECTING_NEWLINE_1;
                } else if (isdigit(input)) {
                    req.http_version_minor = req.http_version_minor * 10 + (input - '0');
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::EXPECTING_NEWLINE_1:
                if (input == '\n') {
                    state_ = State::HEADER_LINE_START;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HEADER_LINE_START:
                if (input == '\r') {
                    state_ = State::EXPECTING_NEWLINE_3;
                } else if (!req.headers.empty() && (input == ' ' || input == '\t')) {
                    state_ = State::HEADER_LWS;
                } else if (!iscntrl(input) && input != ':' && input != ' ') {
                    req.headers.push_back(Header());
                    req.headers.back().name.push_back(input);
                    state_ = State::HEADER_NAME;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HEADER_LWS:
                if (input == '\r') {
                    state_ = State::EXPECTING_NEWLINE_2;
                } else if (input == ' ' || input == '\t') {
                } else if (!iscntrl(input)) {
                    req.headers.back().value.push_back(input);
                    state_ = State::HEADER_VALUE;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HEADER_NAME:
                if (input == ':') {
                    state_ = State::SPACE_BEFORE_HEADER_VALUE;
                } else if (!iscntrl(input) && input != ' ') {
                    req.headers.back().name.push_back(input);
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::SPACE_BEFORE_HEADER_VALUE:
                if (input == ' ') {
                    state_ = State::HEADER_VALUE;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::HEADER_VALUE:
                if (input == '\r') {
                    state_ = State::EXPECTING_NEWLINE_2;
                } else if (iscntrl(input)) {
                    return ParseResult::BAD;
                } else {
                    req.headers.back().value.push_back(input);
                }
                break;
            case State::EXPECTING_NEWLINE_2:
                if (input == '\n') {
                    state_ = State::HEADER_LINE_START;
                } else {
                    return ParseResult::BAD;
                }
                break;
            case State::EXPECTING_NEWLINE_3:
                return (input == '\n') ? ParseResult::GOOD : ParseResult::BAD;
        }
    }
    return ParseResult::INDETERMINATE;
}

} // namespace http
} // namespace mycppwebfw
