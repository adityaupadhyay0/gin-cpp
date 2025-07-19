#include "mycppwebfw/core/connection_manager.h"

namespace mycppwebfw {
namespace core {

ConnectionManager::ConnectionManager() {}

void ConnectionManager::start(std::shared_ptr<Connection> c) {
    connections_.insert(c);
    c->start();
}

void ConnectionManager::stop(std::shared_ptr<Connection> c) {
    connections_.erase(c);
    c->stop();
}

void ConnectionManager::stop_all() {
    for (auto c : connections_) {
        c->stop();
    }
    connections_.clear();
}

} // namespace core
} // namespace mycppwebfw
