cmake_minimum_required(VERSION 3.7)
project(CRawlerPlusPlus_git)

find_package(Boost 1.63.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

add_executable(CRawlerPlusPlus ${SOURCE_FILES})
target_link_libraries(CRawlerPlusPlus ${Boost_LIBRARIES})