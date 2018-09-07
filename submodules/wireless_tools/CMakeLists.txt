project (wireless_test_app)
cmake_minimum_required(VERSION 2.8)

ADD_LIBRARY(wiffy_module
	iwlib.c
)

ADD_EXECUTABLE(wifi_test wifi_test.cpp)
target_link_libraries(wifi_test wiffy_module)

