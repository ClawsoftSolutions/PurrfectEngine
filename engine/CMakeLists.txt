file(GLOB_RECURSE SOURCES "src/**.cpp" "include/**.hpp")
add_library(PurrfectEngine STATIC ${SOURCES})
target_include_directories(PurrfectEngine PUBLIC include/)