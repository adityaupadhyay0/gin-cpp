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
    size_t get_connection_count() const;

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

template <typename ConnectionType>
size_t ConnectionManager<ConnectionType>::get_connection_count() const {
    return connections_.size();
}

} // namespace core
} // namespace mycppwebfw
