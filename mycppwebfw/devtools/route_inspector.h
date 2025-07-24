#pragma once

#include <string>
#include <vector>
#include "mycppwebfw/routing/router.h"

namespace mycppwebfw
{
namespace devtools
{

struct RouteInfo
{
    std::string method;
    std::string path;
    std::string handler_name;  // Or some other representation
};

class RouteInspector
{
public:
    RouteInspector(routing::Router& router);
    std::vector<RouteInfo> list_routes();

private:
    routing::Router& router_;
};

}  // namespace devtools
}  // namespace mycppwebfw
