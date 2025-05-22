cmake_minimum_required(VERSION 3.13.4)
project(magic-cli)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

include(AddLLVM)

set(LLVM_LINK_COMPONENTS
  CodeGen
  CodeGenTypes
  Support
  Core
  Analysis
)

add_llvm_executable(
  magic-cli
  main.cpp
)
