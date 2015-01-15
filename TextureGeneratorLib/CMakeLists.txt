include_directories(${Qt5Core_INCLUDE_DIRS})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(texturegenerator generator.hpp generator.cpp)
target_link_libraries(texturegenerator Qt5::Core)