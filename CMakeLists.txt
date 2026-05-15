cmake_minimum_required(VERSION 3.20)
project(MyPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(MyPass MODULE MyPass.cpp)

set_target_properties(MyPass PROPERTIES COMPILE_FLAGS "-fno-rtti")

if(APPLE)
    set_target_properties(MyPass PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()