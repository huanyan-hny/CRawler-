cmake_minimum_required(VERSION 3.7)
project(CRawlerPlusPlus_git)

#set (CMAKE_CXX_FLAGS "--std=c++1z -fconcepts ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS "--std=c++1z ${CMAKE_CXX_FLAGS}")
#set(SOURCE_FILES test/test_network.cpp)
set(SOURCE_FILES main.cpp src/Downloader.cpp
        src/Network.cpp
        src/Spider.cpp
        src/Json.cpp
        src/Task.cpp
        src/Generic_Scheduler.cpp
        src/Item.cpp
        src/ItemPipeline.cpp
        src/HTML.cpp)
add_executable(CRawlerPlusPlus ${SOURCE_FILES})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(CRawlerPlusPlus Threads::Threads)

find_package(Boost 1.63.0 COMPONENTS system filesystem REQUIRED)
find_package(curl 7.53.1 COMPONENTS system filesystem REQUIRED)
add_subdirectory(cpr)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include/CRawlerPlusPlus)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CPR_INCLUDE_DIRS})
include_directories(${curl_INCLUDE_DIRS})
target_link_libraries(CRawlerPlusPlus ${CPR_LIBRARIES})
target_link_libraries(CRawlerPlusPlus ${Boost_LIBRARIES})
target_link_libraries(CRawlerPlusPlus ${curl_LIBRARIES})