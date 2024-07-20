# src/CMakeLists.txt

# Add the executable
add_executable(CMakeProject1 "main.cpp" "constant.h")

# Link Raylib
target_link_libraries(CMakeProject1 raylib)
