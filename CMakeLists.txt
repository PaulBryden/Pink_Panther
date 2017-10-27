cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.10.9.tar.gz"
    SHA1 "53b198e364dc7bc8360fc545f798563229bd7e20"
)

project(Foo C CXX)
#set_target_properties(cppREST PROPERTIES LINKER_LANGUAGE Cxx) 
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_MODULE_PATH "${PROJECT_ROOT}/cmake")
hunter_add_package(Boost COMPONENTS random system thread filesystem chrono atomic date_time regex)
find_package(Boost CONFIG REQUIRED random system thread filesystem chrono atomic date_time regex)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED )

#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#find_package(CppREST)
#add_library(CppREST STATIC "lib/libcpprestd.a")
#message(STATUS "CPPRESTLIBS")
#message(STATUS ${CppREST_LIBS})

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

ADD_LIBRARY(wiffy_module src/iwlib.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build_panther/usr/local/include)
include_directories(SYSTEM ${CPP_REST_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(helloworldspeaker src/main.cpp src/messagetypes.h src/stdafx.h src/MyServer.h src/MyServer.cpp src/Node.cpp src/inc/Node.h src/Nodes.cpp src/inc/Nodes.h src/inc/wireless.h src/RestModule.cpp src/inc/RestModule.h src/WifiScanModule.cpp src/inc/WifiScanModule.h)
target_link_libraries(helloworldspeaker ${CMAKE_SOURCE_DIR}/../build_panther/usr/local/lib/libcpprestd.a
  Boost::boost
  Boost::random
  Boost::system
  Boost::thread
  Boost::filesystem
  Boost::chrono
  Boost::atomic
  Boost::date_time
  Boost::regex
  ${OPENSSL_LIBRARIES}
  wiffy_module
)

#add_executable(helloworldlistener src/listen.cpp)
#target_link_libraries(helloworldlistener ${CppREST_LIBS})
