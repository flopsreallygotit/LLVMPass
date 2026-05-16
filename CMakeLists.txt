cmake_minimum_required(VERSION 3.20)

execute_process(COMMAND brew --prefix llvm
    OUTPUT_VARIABLE LLVM_BREW_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_C_COMPILER "${LLVM_BREW_PREFIX}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_BREW_PREFIX}/bin/clang++")

project(MyPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(LLVM REQUIRED CONFIG)
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})

add_library(MyPass MODULE
    src/MyPass.cpp
    src/DotWriter.cpp
    src/ValueIds.cpp
    src/Labels.cpp
    src/Instrumenter.cpp
)

target_include_directories(MyPass PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(MyPass PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMPILE_FLAGS "-fno-rtti"
)

if(APPLE)
    set_target_properties(MyPass PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()