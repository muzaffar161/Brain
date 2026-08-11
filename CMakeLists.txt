cmake_minimum_required(VERSION 3.16)
project(brain_hh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(brain_hh
  src/main.cpp
  src/neuron.cpp
  src/synapse.cpp
  src/network.cpp
  src/simulator.cpp
)

target_include_directories(brain_hh PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
  target_compile_options(brain_hh PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()
