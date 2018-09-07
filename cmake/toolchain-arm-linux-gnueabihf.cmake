# CMake toolchain file for building ARM software on OI environment
SET (BOOST_ROOT /home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install)
SET (BOOST_INCLUDEDIR /home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install/include)
SET (BOOST_LIBRARYDIR /home/paolini/.hunter/_Base/e99ddb1/24add81/9a337fa/Install/lib)



# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
SET(CMAKE_STRIP /usr/bin/arm-linux-gnueabihf-strip)
SET(CMAKE_Fortran_COMPILER /usr/bin/arm-linux-gnueabihf-gfortran)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/arm-linux-gnueabihf)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

