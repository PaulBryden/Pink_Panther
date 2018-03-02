cmake_minimum_required(VERSION 3.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.137.tar.gz"
    SHA1 "e99ddb16d3b92b7ea9ac4b526faf9415db1c5c5b"
)

project(Foo C CXX)
#set_target_properties(cppREST PROPERTIES LINKER_LANGUAGE Cxx) 
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt")
#SET_TARGET_PROPERTIES(helloworldspeaker PROPERTIES LINK_FLAGS -lgfortran)
#set(CMAKE_MODULE_PATH "${PROJECT_ROOT}/cmake")
#hunter_add_package(LAPACK)
#find_package(LAPACK CONFIG REQUIRED)
#find_package(LAPACKE)
hunter_add_package(Boost COMPONENTS random system thread filesystem atomic date_time regex timer chrono)
find_package(Boost CONFIG REQUIRED random system thread filesystem atomic date_time regex timer chrono)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)


#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#find_package(CppREST)
#add_library(CppREST STATIC "lib/libcpprestd.a")
#message(STATUS "CPPRESTLIBS")
#message(STATUS ${CppREST_LIBS})

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

ADD_LIBRARY(wiffy_module src/iwlib.c)
include_directories(../Pink_Panther/submodules/Kalman/kalman/*.hpp)
ADD_LIBRARY(kalman ../Pink_Panther/submodules/Kalman/kstatics.cpp)


include_directories(${CMAKE_CURRENT_BINARY_DIR}/usr/local/include)
if(RASPBERRY_PI)
else()
include_directories(/home/green/software/build_panther/usr/local/include)
endif()
#include_directories(/home/green/software/build_panther/usr/local/include)
include_directories(SYSTEM ${CPP_REST_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(trigApp src/Main.cpp src/inc/messagetypes.h src/inc/stdafx.h src/inc/RestModule.h src/RestModule.cpp src/Node.cpp src/inc/Node.h src/Node_Container.cpp src/inc/Node_Container.h src/inc/wireless.h src/WifiScanModule.cpp src/inc/WifiScanModule.h src/inc/Repository.h src/inc/Repository.inc.h src/inc/FileIO.h src/FileIO.cpp src/Target_Node.cpp src/inc/Target_Node.h src/inc/INode.h src/locationModule.cpp src/inc/locationModule.h src/inc/Location.h src/KamalFilterRSSI.cpp src/inc/KamalFilterRSSI.h)
SET_TARGET_PROPERTIES(trigApp PROPERTIES LINK_FLAGS "-lgfortran")

target_link_libraries(trigApp ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/libcpprestd.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/liblapacke.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/liblapack.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/libblas.a
  Boost::boost
  Boost::random
  Boost::timer
  Boost::system
  Boost::chrono
  Boost::filesystem
  Boost::thread
  Boost::atomic
  Boost::date_time
  Boost::regex
  ${OPENSSL_LIBRARIES}
  wiffy_module
  kalman
)

if(RASPBERRY_PI)
target_link_libraries(trigApp
/usr/arm-linux-gnueabihf/lib/libgfortran.so.3
)
else()
target_link_libraries(trigApp
lapack
gfortran
)
endif()

add_executable(MainProxy src/MainProxy.cpp src/inc/messagetypes.h src/inc/stdafx.h src/inc/RestModuleProxy.h src/RestModuleProxy.cpp src/Node.cpp src/inc/Node.h src/Node_Container.cpp src/inc/Node_Container.h src/inc/wireless.h src/WifiScanModule.cpp src/inc/WifiScanModule.h src/inc/Repository.h src/inc/Repository.inc.h src/inc/FileIO.h src/FileIO.cpp src/Target_Node.cpp src/inc/Target_Node.h src/inc/INode.h src/locationModule.cpp src/inc/locationModule.h src/inc/Location.h src/KamalFilterRSSI.cpp src/inc/KamalFilterRSSI.h)
SET_TARGET_PROPERTIES(MainProxy PROPERTIES LINK_FLAGS "-lgfortran")
MESSAGE(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(MainProxy ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/libcpprestd.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/liblapacke.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/liblapack.a ${CMAKE_CURRENT_BINARY_DIR}/usr/local/lib/libblas.a
  Boost::boost
  Boost::random
  Boost::timer
  Boost::system
  Boost::chrono
  Boost::filesystem
  Boost::thread
  Boost::atomic
  Boost::date_time
  Boost::regex
  ${OPENSSL_LIBRARIES}
  wiffy_module
  kalman
)

if(RASPBERRY_PI)
target_link_libraries(MainProxy
/usr/arm-linux-gnueabihf/lib/libgfortran.so.3
)
else()
target_link_libraries(MainProxy
lapack
gfortran
)
endif()
#add_executable(helloworldlistener src/listen.cpp)
#target_link_libraries(helloworldlistener ${CppREST_LIBS})
