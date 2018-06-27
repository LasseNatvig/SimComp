cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS}")
set(CMAKE_CXX_STANDARD 11)

project(SimComp)

add_executable(
  SimComp
  main.cpp
  ../../_Simulator/_src/compSim.cpp
  ../../_Simulator/_src/isa.cpp
  ../../_Simulator/_src/loader.cpp
  ../../_Simulator/_src/logger.cpp
  ../../_Simulator/_src/memory.cpp
  ../../_Simulator/_src/Program.cpp
  utils.cpp
  config.cpp
  consoleUI.cpp
)
