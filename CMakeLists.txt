cmake_minimum_required(VERSION 3.20.0)
project(CHAT_CLIENT VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(CHAT_CLIENT main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CPack)
