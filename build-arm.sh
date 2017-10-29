#!/bin/sh

mkdir ../build_panther_arm
cd ../build_panther_arm
pwd
#rm CMakeCache.txt
#cmake -G Ninja -H../Pink_Panther/submodules/OpenSSL-CMake/. -B. -DHUNTER_STATUS_DEBUG=ON  -DCMAKE_TOOLCHAIN_FILE=/home/green/software/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DCMAKE_INSTALL_PREFIX=/usr/arm-linux-gnueabihf -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install
rm CMakeCache.txt
#cmake -G Ninja -H../Pink_Panther/submodules/cpprestsdk/Release/. -B. -DHUNTER_STATUS_DEBUG=ON  -DCMAKE_TOOLCHAIN_FILE=/home/green/software/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DRASPBERRY_PI=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install
rm CMakeCache.txt
cmake -G Ninja -H../Pink_Panther/. -B.  -DCMAKE_INSTALL_PREFIX=/usr/arm-linux-gnueabihf -DCMAKE_TOOLCHAIN_FILE=/home/green/software/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja
