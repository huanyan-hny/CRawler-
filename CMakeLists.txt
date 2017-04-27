
cmake_minimum_required(VERSION 3.7)

SET(CMAKE_C_COMPILER "/usr/bin/gcc-6")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++-6")

project(CRawlerPlusPlus_git)
set (CMAKE_CXX_FLAGS "--std=c++1z -fconcepts ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_STANDARD 14)
#set(SOURCE_FILES test/test_network.cpp)
set(SOURCE_FILES main.cpp
        src/Item.cpp
        src/ItemPipeline.cpp
        src/Spider.cpp
        src/Generic_Scheduler.cpp
        src/Task.cpp
        src/Network.cpp
        src/Downloader.cpp
        src/Parser.cpp
        src/Json.cpp
        )
add_executable(CRawlerPlusPlus ${SOURCE_FILES})

IF (CURL_FOUND)
    message(${CMAKE_ROOT})
ENDIF (CURL_FOUND)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system filesystem REQUIRED)
add_subdirectory(cpr)
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include/CRawlerPlusPlus")
include_directories("${CMAKE_SOURCE_DIR}/demo/IMDB")
include_directories("${CMAKE_SOURCE_DIR}/demo/GrubHub")



include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CPR_INCLUDE_DIRS})
include_directories(${curl_INCLUDE_DIRS})
target_link_libraries(CRawlerPlusPlus ${CPR_LIBRARIES})
target_link_libraries(CRawlerPlusPlus ${Boost_LIBRARIES})
target_link_libraries(CRawlerPlusPlus ${curl_LIBRARIES})
target_link_libraries(CRawlerPlusPlus Threads::Threads)