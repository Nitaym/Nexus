cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
include_directories("../../")
set(SOURCE_FILES
        test_comm.cpp)

IF (WIN32)
    set_property(SOURCE ${SOURCE_FILES} APPEND PROPERTY COMPILE_DEFINITIONS WIN32)
ELSE()
    # set stuff for other systems
ENDIF()

link_directories("../../")

add_executable(test_comm ${SOURCE_FILES})
target_link_libraries(test_comm Nexus pthread)
