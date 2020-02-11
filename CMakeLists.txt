cmake_minimum_required(VERSION 3.16)
project(logger)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fconcepts")
set(CROSS_COMPILE_FOR_RPI False)

if(CROSS_COMPILE_FOR_RPI)
    set(CMAKE_SIZEOF_VOID_P 4)
    set(CMAKE_CXX_COMPILER_ARCHITECTURE_ID "ARM")
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_COMPILER "gcc")

    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    set(cross_compile_toolchain_path 
        $ENV{HOME}/.local/x-tools/armv8-rpi3-linux-gnueabihf)
    set(CMAKE_SYSROOT 
        ${cross_compile_toolchain_path}/armv8-rpi3-linux-gnueabihf/sysroot)
    set(CMAKE_C_COMPILER
        ${cross_compile_toolchain_path}/bin/armv8-rpi3-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER
        ${cross_compile_toolchain_path}/bin/armv8-rpi3-linux-gnueabihf-g++)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

add_subdirectory(async_io)
add_subdirectory(serial_port_logger)
add_subdirectory(unit_tests)
add_subdirectory(acceptance_tests)
add_subdirectory(integration_tests)

if(CROSS_COMPILE_FOR_RPI)
    set(CPACK_PACKAGE_NAME "serial_port_logger-rpi3")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
else()
    set(CPACK_PACKAGE_NAME "serial_port_logger")
endif()

set(CPACK_PACKAGE_DESCRIPTION "serial_port_logger is a single threaded console\
 application that can log multiple serial port devices simultaneously.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It can log multiple serial port\
 devices.")

if(UNIX)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Reza Arjmandi")
endif()

include(CPack)