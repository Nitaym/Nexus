cmake_minimum_required(VERSION 2.8)
project(Nexus)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories("./")
set(SOURCE_FILES
        Comm/Base/CData.h
        Comm/Base/CData.cpp
        Comm/Base/CDebug.h
        Comm/Base/CDebug.cpp
        Comm/Physical/CConsoleComm.h
        Comm/Physical/CConsoleComm.cpp
        Comm/Physical/CFileComm.h
        Comm/Physical/CFileComm.cpp
        Comm/Physical/CLoggerComm.h
        Comm/Physical/CLoggerComm.cpp
        Comm/Physical/CLoopbackComm.h
        Comm/Physical/CLoopbackComm.cpp
        Comm/Physical/CClientSocket.h
        Comm/Physical/CClientSocket.cpp
        Comm/Physical/CServerSocket.h
        Comm/Physical/CServerSocket.cpp
        Comm/Physical/CUart.h
        Comm/Physical/CUart.cpp
        Comm/Protocols/CAsyncReceiver.h
        Comm/Protocols/CAsyncReceiver.cpp
        Comm/Protocols/CAutoConnect.h
        Comm/Protocols/CAutoConnect.cpp
        Comm/Protocols/ICommLayer.h
        Comm/Protocols/ICommLayer.cpp
        Comm/Protocols/TComm.h
        Comm/Protocols/TComm.cpp
        General/General.h
        General/Macros.h
        General/Types.h
        General/Utils.cpp)

IF (WIN32)
    set_property(SOURCE ${SOURCE_FILES} APPEND PROPERTY COMPILE_DEFINITIONS WIN32)
ELSE()
# set stuff for other systems
ENDIF()


add_library(Nexus ${SOURCE_FILES})
