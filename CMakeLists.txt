# Pulse Core Library
add_library(pulse-core STATIC)
set_target_properties(pulse-core PROPERTIES AUTOMOC ON)

# Source files
target_sources(pulse-core PRIVATE
    pulse-core/Core.cpp
    pulse-core/Logger.cpp
    pulse-config/Config.cpp
    pulse-ipc/DBusInterface.cpp
    pulse-plugins/PluginManager.cpp
)

# Include directories
target_include_directories(pulse-core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pulse-core
    ${CMAKE_CURRENT_SOURCE_DIR}/pulse-config
    ${CMAKE_CURRENT_SOURCE_DIR}/pulse-ipc
    ${CMAKE_CURRENT_SOURCE_DIR}/pulse-plugins
)

# Link libraries
target_link_libraries(pulse-core PUBLIC
    Qt6::Core
    Qt6::DBus
)

# Install headers
install(DIRECTORY pulse-core pulse-config pulse-ipc pulse-plugins
    DESTINATION include/pulse/core
    FILES_MATCHING PATTERN "*.h"
)

# Test executable
add_executable(pulse-core-test main.cpp)
target_link_libraries(pulse-core-test PRIVATE pulse-core)

# For testing
add_custom_target(test-core
    COMMAND ./pulse-core-test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS pulse-core-test
)
