
cmake_minimum_required (VERSION 3.8)
project ("test")
include_directories("./include")
link_directories("./lib")
add_executable (${PROJECT_NAME} "test.cpp")
target_link_libraries(${PROJECT_NAME} log4cpp Ws2_32)
