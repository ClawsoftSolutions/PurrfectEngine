cmake_minimum_required(VERSION 3.14)

project(PurrfectEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(deps)

if(WIN32)
  add_definitions(-DPE_WINDOWS)
elseif(LINUX)
  add_definitions(-DPE_LINUX)
endif()

add_subdirectory(engine)
add_subdirectory(sandbox)
# Add more subdirectories like editor, home etc. as the engine grows