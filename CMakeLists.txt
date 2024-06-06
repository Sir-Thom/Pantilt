cmake_minimum_required(VERSION 3.25)
project(pantilt)

set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(src)
include_directories(src/impl)
include_directories(deps/WiringPi)
include_directories(deps/WiringPi/wiringPi)
include_directories(deps/WiringPi/wiringPiD)

# Add a custom target to build WiringPi
add_custom_target(build_wiringpi ALL
        COMMAND ${CMAKE_COMMAND} -E env sh ${CMAKE_SOURCE_DIR}/deps/WiringPi/build
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/WiringPi
        COMMENT "Building WiringPi library"
)

# Define the library
add_library(pantilt SHARED
        src/pantilt.cpp
        src/impl/pantilt.h
        # Add other source files if any
)

# Specify include directories for the library
target_include_directories(pantilt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(pantilt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/impl)

# Link against the WiringPi library
target_link_libraries(pantilt wiringPi)

# Optionally, you can install the library
install(TARGETS pantilt LIBRARY DESTINATION lib)
install(FILES src/impl/pantilt.h DESTINATION include)
