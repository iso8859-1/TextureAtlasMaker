# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(textureatlasmaker main.cpp mainwindow.hpp mainwindow.cpp)
qt5_use_modules(textureatlasmaker Widgets)
target_link_libraries(textureatlasmaker ${QT_LIBRARIES})

IF(MSVC) # Check if we are using the Visual Studio compiler
	target_link_libraries(textureatlasmaker Qt5::WinMain)
    set_target_properties(textureatlasmaker PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
ENDIF(MSVC)