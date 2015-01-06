find_package(Qt5Test REQUIRED)
add_executable(testtextureatlasmaker main.cpp)
#add required tests here
target_link_libraries(testtextureatlasmaker Qt5::Test)