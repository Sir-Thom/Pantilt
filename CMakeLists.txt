cmake_minimum_required(VERSION 3.25)
project(pantilt)

set(CMAKE_CXX_STANDARD 14)

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

#add_library(pantilt
#        SHARED
#        pantilt.cpp
#        pantilt.h
#        # Other source files
#)
#
#
## Specify include directories
#target_include_directories(pantilt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
## Link against the WiringPi library
#target_link_libraries(pantilt wiringPi)
add_executable(pantilt
        src/pantilt.cpp
        src/impl/pantilt.h
        # Other source files
)
# Link against the WiringPi library
target_link_libraries(pantilt wiringPi)
