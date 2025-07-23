#include "mycppwebfw/core/connection_manager.h"

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
