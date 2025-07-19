#pragma once

#include <set>
#include <memory>
#include "mycppwebfw/core/connection.h"

namespace mycppwebfw {
namespace core {

class ConnectionManager {
public:
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager();

    void start(std::shared_ptr<Connection> c);
    void stop(std::shared_ptr<Connection> c);
    void stop_all();

private:
    std::set<std::shared_ptr<Connection>> connections_;
};

} // namespace core
} // namespace mycppwebfw
