# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test REQUIRED)
add_executable(testtextureatlasmaker simpletest.cpp)
#add required tests here
target_link_libraries(testtextureatlasmaker Qt5::Test)