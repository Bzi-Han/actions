cmake_minimum_required(VERSION 3.0)

project(main)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} /MT /GR- /utf-8")
add_definitions(-DUNICODE -D_UNICODE)

include_directories(
    includes
    modules
    third_party
)

# build quickjs static library
add_subdirectory(third_party/quickjs-cmake)

# build test program
add_executable(main src/main.cc)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(main quickjs)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(main quickjs dl pthread)
else()
    target_link_libraries(main quickjs)
endif()