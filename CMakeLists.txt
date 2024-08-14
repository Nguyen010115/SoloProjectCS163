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
 "render/hash_table.cpp" "render/button.cpp" "render/hash_table.h" "render/animation.h"  "render/bee.cpp" "render/bee.h" "render/avltree.cpp" "render/avltree.h" "render/234Tree.h" "render/234Tree.cpp" "render/minheap.cpp" "render/minheap.h")

target_link_libraries(${PROJECT_NAME} raylib)

# Copy images directory
add_custom_target(copy_resources_images ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/images
    ${CMAKE_BINARY_DIR}/resources/images
)

# Copy fonts directory
add_custom_target(copy_resources_fonts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/fonts
    ${CMAKE_BINARY_DIR}/resources/fonts
)

# Ensure the images and fonts are copied before building the executable
add_dependencies(${PROJECT_NAME} copy_resources_images copy_resources_fonts)
