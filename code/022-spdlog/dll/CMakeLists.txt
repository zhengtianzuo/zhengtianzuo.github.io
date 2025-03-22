
cmake_minimum_required (VERSION 3.8)
project ("cpplib")
add_definitions(-D cpplib_exports)
include_directories("../include")
link_directories("../lib")
add_library (${PROJECT_NAME} SHARED "cpplib.cpp")
target_link_libraries(${PROJECT_NAME} spdlog)
