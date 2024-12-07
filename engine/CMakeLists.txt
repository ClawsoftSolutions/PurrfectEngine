file(GLOB_RECURSE SOURCES "src/**.cpp" "include/**.hpp")

add_library(PurrfectEngine STATIC ${SOURCES})

target_include_directories(PurrfectEngine
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(PurrfectEngine
  PUBLIC purrr EnTT glm
)