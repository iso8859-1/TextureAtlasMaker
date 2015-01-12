cmake_minimum_required (VERSION 3.0)
project (TextureAtlasMaker)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_PREFIX_PATH "~/Documents/Development/Libraries/Qt/5.4/clang_64")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_PREFIX_PATH "D:/Programming/Tools/Qt/Qt5.3.2/5.3/msvc2013_64")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(NOT MSVC) # Check if we are using the Visual Studio compiler
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
ENDIF(NOT MSVC)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Qt library
find_package(Qt5Widgets)

add_subdirectory(TextureAtlasMaker)
add_subdirectory(test)