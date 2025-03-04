cmake_minimum_required(VERSION 3.14)

project(PurrfectEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
  add_definitions(-DPE_PLATFORM_WINDOWS)
elseif(LINUX)
  add_definitions(-DPE_PLATFORM_LINUX)
else()
  message(FATAL_ERROR "Unsupported platform!")
endif()

add_subdirectory(engine/)
add_subdirectory(sandbox/)