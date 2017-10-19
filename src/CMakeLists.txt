cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("To Do List")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

enable_testing()


if (NOT DEFINED     BOOST_ROOT        AND
    NOT DEFINED ENV{BOOST_ROOT}       AND
    NOT DEFINED     BOOST_INCLUDEDIR  AND
    NOT DEFINED ENV{BOOST_INCLUDEDIR} AND
    NOT DEFINED     BOOST_LIBRARYDIR  AND
    NOT DEFINED ENV{BOOST_LIBRARYDIR})
        set(BOOST_ROOT "../lib/boost_1_59_0/")
    set(Boost_USE_STATIC_LIBS TRUE)
endif()
find_package(Boost 1.59 REQUIRED COMPONENTS system)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )

MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

add_executable(BoostExample BoostExample.cpp)
target_link_libraries(BoostExample ${Boost_LIBRARIES})


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
