include(Util)

set(Boost_ARCHITECTURE "ARM")

find_or_get(Boost COMPONENTS program_options system)
find_or_get(Threads)

set(cpp_files 
    main.cpp 
    Application.cpp)

add_executable(serial_port_logger 
    ${cpp_files})
target_include_directories(serial_port_logger PUBLIC
    ${INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    .)
target_link_libraries(serial_port_logger 
    async_io 
    Boost::program_options 
    Boost::system
    Threads::Threads
    stdc++fs)
target_precompile_headers(serial_port_logger PUBLIC pch.h)
set_property(TARGET serial_port_logger PROPERTY CXX_STANDARD 17)

if(UNIX)
    get_target_property(
        BOOST_PROGRAM_OPTIONS_LIB 
        Boost::program_options 
        LOCATION)

    get_target_property(
        BOOST_SYSTEM_LIB 
        Boost::system 
        LOCATION)

    include(GNUInstallDirs)
    install(TARGETS serial_port_logger TYPE RUNTIME)
    install(FILES 
        ${BOOST_PROGRAM_OPTIONS_LIB} 
        ${BOOST_SYSTEM_LIB} 
        TYPE LIB)
endif()
