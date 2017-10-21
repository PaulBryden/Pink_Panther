#!/bin/sh

mkdir ../build_panther
cd ../build_panther
pwd
rm CMakeCache.txt
cmake -G Ninja -H../Pink_Panther/submodules/cpprestsdk/Release/. -B. -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install
rm CMakeCache.txt
cmake -G Ninja -H../Pink_Panther/. -B. -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja
