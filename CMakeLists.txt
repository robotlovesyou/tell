cmake_minimum_required(VERSION 3.15)
project(tell)
include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/thirdparty)

add_library(tell compiler/MessageDescriptor.cpp compiler/MessageDescriptor.h compiler/FieldDescriptor.cpp compiler/FieldDescriptor.h include/thirdparty/fmt/format.cc)

add_executable(tellc main.cpp)
target_link_libraries(tellc tell)

add_subdirectory(tests)