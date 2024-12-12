file(GLOB_RECURSE SOURCES "src/**.cpp" "include/**.hpp")
add_executable(sandbox ${SOURCES})
target_link_libraries(sandbox PRIVATE PurrfectEngine)
target_include_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(shaders)

set(GLSL_SHADER_FILES vertex.vert fragment.frag)

foreach(FILE ${GLSL_SHADER_FILES})
  get_filename_component(FILE_WE ${FILE} NAME_WE)
  add_custom_command(TARGET shaders
                     COMMAND glslangValidator -V -o ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${FILE}
                     MAIN_DEPENDENCY ${FILE}
                     COMMENT "Compiling GLSL ${FILE} to SPIR-V"
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     VERBATIM)
endforeach(FILE)

add_dependencies(sandbox shaders)