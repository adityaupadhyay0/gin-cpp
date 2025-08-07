# Get all source files in the current directory and its subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/token_bucket.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/rate_limiter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/cors_policy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/cors.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/logging/formatter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/logging/sinks.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/compressor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/compression.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/request_id_generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/request_id.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/request_id_generator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/request_id.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/api_key_registry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/jwt_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/auth.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/api_key_registry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/jwt_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware/auth.cpp"
)

# Add the sources to the parent target
target_sources(mycppwebfw INTERFACE ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/core/connection.tpp"
)
