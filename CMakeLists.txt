cmake_minimum_required (VERSION 3.0)
project (TextureAtlasMaker)

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(CMAKE_PREFIX_PATH "~/Documents/Development/Libraries/Qt/5.4/clang_64")
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Qt library
find_package(Qt5Widgets)

add_subdirectory(TextureAtlasMaker)
add_subdirectory(test)