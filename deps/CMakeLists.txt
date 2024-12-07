add_subdirectory(purrr/)
add_subdirectory(entt/)
add_subdirectory(glm/)

set(BUILD_TESTING OFF)
set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
add_subdirectory(libsndfile/)

set(ALSOFT_TESTS OFF)
set(ALSOFT_INSTALL OFF)
set(ALSOFT_INSTALL_CONFIG OFF)
set(ALSOFT_INSTALL_HRTF_DATA OFF)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF)
set(ALSOFT_INSTALL_EXAMPLES OFF)
set(ALSOFT_INSTALL_UTILS OFF)
set(ALSOFT_UPDATE_BUILD_VERSION OFF)
set(LIBTYPE STATIC)
add_subdirectory(openal-soft/)

# minimp3

add_library(minimp3 INTERFACE)
target_include_directories(minimp3 INTERFACE minimp3/)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)