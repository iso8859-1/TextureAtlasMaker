include_directories("../TextureGeneratorLib")
include_directories(${Qt5Core_INCLUDE_DIRS})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(testtextureatlasmaker testgenerator.cpp)
target_link_libraries(testtextureatlasmaker Qt5::Core texturegenerator)