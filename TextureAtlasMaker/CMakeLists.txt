# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(textureatlasmaker main.cpp)
qt5_use_modules(textureatlasmaker Widgets)
target_link_libraries(textureatlasmaker ${QT_LIBRARIES})