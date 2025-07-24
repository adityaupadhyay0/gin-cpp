#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <regex>

namespace mycppwebfw {
namespace routing {

class ParameterParser {
public:
    template<typename T>
    static T parse(const std::string& s);

    static bool validate(const std::string& s, const std::string& pattern);
};

template<>
inline int ParameterParser::parse<int>(const std::string& s) {
    return std::stoi(s);
}

template<>
inline float ParameterParser::parse<float>(const std::string& s) {
    return std::stof(s);
}

template<>
inline double ParameterParser::parse<double>(const std::string& s) {
    return std::stod(s);
}

template<>
inline std::string ParameterParser::parse<std::string>(const std::string& s) {
    return s;
}

inline bool ParameterParser::validate(const std::string& s, const std::string& pattern) {
    std::regex re(pattern);
    return std::regex_match(s, re);
}

} // namespace routing
} // namespace mycppwebfw
