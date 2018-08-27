#!/bin/sh
PATH=/home/paolini/ninja:$PATH; export PATH

export BOOST_ROOT="/home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install"

mkdir build-arm
cd build-arm
pwd
#rm -f CMakeCache.txt
#cmake -G Ninja -H../Pink_Panther/submodules/OpenSSL-CMake/. -B. -DHUNTER_STATUS_DEBUG=ON  -DCMAKE_TOOLCHAIN_FILE=/home/paolini/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DCMAKE_INSTALL_PREFIX=/usr/arm-linux-gnueabihf -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install

#cmake -G Ninja -H../submodules/lapack/. -B. -DCMAKE_TOOLCHAIN_FILE=/home/paolini/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DRASPBERRY_PI=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install

#rm CMakeCache.txt
#cmake -G Ninja -H../submodules/cpprestsdk/Release/. -B. -DHUNTER_STATUS_DEBUG=ON  -DCMAKE_TOOLCHAIN_FILE=/home/paolini/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DRASPBERRY_PI=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja install

rm -f CMakeCache.txt
cmake -G Ninja -H../ -B. -DHUNTER_STATUS_DEBUG=ON -DCMAKE_INSTALL_PREFIX=/usr/arm-linux-gnueabihf -DCMAKE_TOOLCHAIN_FILE=/home/paolini/Pink_Panther/cmake/toolchain-arm-linux-gnueabihf.cmake -DRASPBERRY_PI=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=0 && DESTDIR=./ ninja
