# Get all source files in the current directory and its subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

# Add the sources to the parent target
target_sources(mycppwebfw INTERFACE ${SOURCES}
    "${CMAKE_CURRENT_SOURCE_DIR}/core/connection.tpp"
)
