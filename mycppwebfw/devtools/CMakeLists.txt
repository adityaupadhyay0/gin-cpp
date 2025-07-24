add_library(devtools)

target_sources(devtools PUBLIC
    "route_inspector.cpp"
)

target_include_directories(devtools PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${asio_SOURCE_DIR}/asio/include"
)
