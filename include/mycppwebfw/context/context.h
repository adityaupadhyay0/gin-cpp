#pragma once

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace mycppwebfw {

class Context {
public:
    Context(http::Request& req, http::Response& res) : req_(req), res_(res) {}

    http::Request& req() { return req_; }
    http::Response& res() { return res_; }

    template<typename T>
    void set(const std::string& key, T value) {
        data_[key] = std::make_shared<T>(std::move(value));
    }

    template<typename T>
    T* get(const std::string& key) {
        auto it = data_.find(key);
        if (it != data_.end()) {
            return std::static_pointer_cast<T>(it->second).get();
        }
        return nullptr;
    }

private:
    http::Request& req_;
    http::Response& res_;
    std::unordered_map<std::string, std::shared_ptr<void>> data_;
};

} // namespace mycppwebfw
