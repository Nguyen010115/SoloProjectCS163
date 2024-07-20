cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SoloProject")

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)
include_directories(render)
include_directories(resources)
add_subdirectory(raylib)

add_executable(${PROJECT_NAME}
    src/main.cpp
    render/menu_render.cpp
 "render/hash_table_render.cpp" "render/button.cpp")

target_link_libraries(${PROJECT_NAME} raylib)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/images
    ${CMAKE_BINARY_DIR}/resources/images
)

# Ensure the copy_resources target runs before the executable
add_dependencies(${PROJECT_NAME} copy_resources)