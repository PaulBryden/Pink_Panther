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

SET (BOOST_ROOT "/home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install")
SET (BOOST_INCLUDEDIR "/home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install/include")
SET (BOOST_LIBRARYDIR "/home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install/lib")

#SET(CMAKE_SYSROOT "/usr/arm-linux-gnueabihf")

#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#find_package(CppREST)
#add_library(CppREST STATIC "lib/libcpprestd.a")
#message(STATUS "CPPRESTLIBS")
#message(STATUS ${CppREST_LIBS})

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

ADD_LIBRARY(wiffy_module src/iwlib.c src/HttpGetNodeReaderModule.cpp src/inc/Modules/HttpGetNodeReaderModule.h src/inc/Modules/Interfaces/IScanModule.h src/inc/Modules/Interfaces/ILocationModule.h src/inc/Exceptions/UnsupportedOperationException.h src/inc/Modules/Interfaces/IModule.h src/WifiScanModulex86Test.cpp src/inc/Modules/WifiScanModulex86Test.h)
#include_directories(/home/paolini/Pink_Panther/submodules/Kalman/kalman/*.hpp)
include_directories(/home/paolini/Pink_Panther/submodules/Kalman)
include_directories(/home/paolini/Pink_Panther/submodules/cpprestsdk/Release/include)
ADD_LIBRARY(kalman /home/paolini/Pink_Panther/submodules/Kalman/kstatics.cpp src/HttpGetNodeReaderModule.cpp src/inc/Modules/HttpGetNodeReaderModule.h src/inc/Modules/Interfaces/IScanModule.h src/inc/Modules/Interfaces/ILocationModule.h src/inc/Exceptions/UnsupportedOperationException.h src/inc/Modules/Interfaces/IModule.h src/WifiScanModulex86Test.cpp src/inc/Modules/WifiScanModulex86Test.h)


#include_directories(${CMAKE_CURRENT_BINARY_DIR}/usr/local/include)
if(RASPBERRY_PI)
else()
include_directories(/home/paolini/Pink_Panther/build_panther/usr/local/include)
endif()
#include_directories(/home/green/software/build_panther/usr/local/include)
include_directories(SYSTEM ${CPP_REST_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(/home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install/include)
include_directories(/home/paolini/Pink_Panther/submodules/lapack/LAPACKE/include)

add_executable(trigApp src/Main.cpp src/inc/stdafx.h src/inc/Modules/RestModule.h src/RestModule.cpp src/Node.cpp src/inc/Data/Node.h src/NodeContainer.cpp src/inc/Data/NodeContainer.h src/inc/wireless.h src/WifiScanModule.cpp src/inc/Modules/WifiScanModule.h src/inc/Modules/FileNodeReaderModule.h src/FileNodeReaderModule.cpp src/TargetNode.cpp src/inc/Data/TargetNode.h src/inc/Interfaces/INode.h src/LocationModule.cpp src/inc/Modules/LocationModule.h src/inc/Data/Location.h src/KalmanFilterRSSI.cpp src/inc/KalmanFilterRSSI.h src/HttpGetNodeReaderModule.cpp src/inc/Modules/HttpGetNodeReaderModule.h src/inc/Modules/Interfaces/IScanModule.h src/inc/Modules/Interfaces/ILocationModule.h src/inc/Exceptions/UnsupportedOperationException.h src/inc/Modules/Interfaces/IModule.h src/WifiScanModulex86Test.cpp src/inc/Modules/WifiScanModulex86Test.h)
SET_TARGET_PROPERTIES(trigApp PROPERTIES LINK_FLAGS "-lgfortran")

target_link_libraries(trigApp ${CMAKE_CURRENT_BINARY_DIR}/../submodules/cpprestsdk/Release/build/Binaries/libcpprest-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/liblapacke-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/liblapack-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/libblas-arm.a
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
lapacke
lapack
refblas
gfortran
)
endif()

add_executable(MainProxy src/MainProxy.cpp src/inc/stdafx.h src/inc/Modules/RestModulePostGetProxy.h src/RestModulePostGetProxy.cpp src/Node.cpp src/inc/Data/Node.h src/NodeContainer.cpp src/inc/Data/NodeContainer.h src/inc/wireless.h src/WifiScanModule.cpp src/inc/Modules/WifiScanModule.h src/inc/Modules/FileNodeReaderModule.h src/FileNodeReaderModule.cpp src/TargetNode.cpp src/inc/Data/TargetNode.h src/inc/Interfaces/INode.h src/LocationModule.cpp src/inc/Modules/LocationModule.h src/inc/Data/Location.h src/KalmanFilterRSSI.cpp src/inc/KalmanFilterRSSI.h src/HttpGetNodeReaderModule.cpp src/inc/Modules/HttpGetNodeReaderModule.h src/inc/Modules/Interfaces/IScanModule.h src/inc/Modules/Interfaces/ILocationModule.h src/inc/Modules/Interfaces/INodeReaderModule.h src/inc/Exceptions/ModuleInitializationException.h src/inc/Exceptions/UnsupportedOperationException.h src/inc/Modules/Interfaces/IModule.h src/WifiScanModulex86Test.cpp src/inc/Modules/WifiScanModulex86Test.h)
SET_TARGET_PROPERTIES(MainProxy PROPERTIES LINK_FLAGS "-lgfortran")
MESSAGE(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(MainProxy ${CMAKE_CURRENT_BINARY_DIR}/../submodules/cpprestsdk/Release/build/Binaries/libcpprest-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/liblapacke-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/liblapack-arm.a ${CMAKE_CURRENT_BINARY_DIR}/../submodules/lapack/libblas-arm.a
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
