#pragma once

#include <set>
#include <memory>
namespace mycppwebfw {
namespace core {

template <typename ConnectionType>
class Connection;

template <typename ConnectionType>
class ConnectionManager {
public:
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    ConnectionManager();

    void start(std::shared_ptr<ConnectionType> c);
    void stop(std::shared_ptr<ConnectionType> c);
    void stop_all();

private:
    std::set<std::shared_ptr<ConnectionType>> connections_;
};

} // namespace core
} // namespace mycppwebfw

namespace mycppwebfw {
namespace core {

template <typename ConnectionType>
ConnectionManager<ConnectionType>::ConnectionManager() {}

template <typename ConnectionType>
void ConnectionManager<ConnectionType>::start(std::shared_ptr<ConnectionType> c) {
    connections_.insert(c);
    c->start();
}

template <typename ConnectionType>
void ConnectionManager<ConnectionType>::stop(std::shared_ptr<ConnectionType> c) {
    connections_.erase(c);
    c->stop();
}

template <typename ConnectionType>
void ConnectionManager<ConnectionType>::stop_all() {
    for (auto c : connections_) {
        c->stop();
    }
    connections_.clear();
}

} // namespace core
} // namespace mycppwebfw
