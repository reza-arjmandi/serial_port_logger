include(Util)

if(CROSS_COMPILE_FOR_RPI)
    set(SERIAL_PORT_LOGGER_DIR)
else()
    set(SERIAL_PORT_LOGGER_DIR ${PROJECT_BINARY_DIR}/serial_port_logger)
endif()

configure_file(config.h.in config.h)

find_or_get(GMock)
find_or_get(GTest)
find_or_get(Threads)
find_or_get(Boost 
    COMPONENTS 
    system 
    date_time
    filesystem)

set(_include_dirs 
    ${INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS})

set(serial_port_logger_acceptance_tests_cpp_files 
    main.cpp
    serial_port_logger_tests.cpp)
add_executable(serial_port_logger_acceptance_tests ${serial_port_logger_acceptance_tests_cpp_files})
target_include_directories(serial_port_logger_acceptance_tests 
    PUBLIC 
    ${_include_dirs}
    ${GMOCK_INCLUDE_DIRS} 
    ${PROJECT_BINARY_DIR}/acceptance_tests)
target_precompile_headers(serial_port_logger_acceptance_tests PUBLIC pch.h)
target_link_libraries(serial_port_logger_acceptance_tests 
    Threads::Threads
    Boost::system
    Boost::date_time
    Boost::filesystem
    stdc++fs
    ${GMOCK_BOTH_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES})
set_property(TARGET serial_port_logger_acceptance_tests PROPERTY CXX_STANDARD 17)

if(UNIX)
    get_target_property(BOOST_DATE_TIME_LIB Boost::date_time LOCATION)
    get_target_property(BOOST_FILE_SYSTEM_LIB Boost::filesystem LOCATION)
    include(GNUInstallDirs)
    install(TARGETS serial_port_logger_acceptance_tests TYPE RUNTIME)
    install(FILES 
        ${BOOST_DATE_TIME_LIB} 
        ${BOOST_FILE_SYSTEM_LIB} 
        TYPE LIB)
endif()

# include(GoogleTest)
# gtest_discover_tests(serial_port_logger_acceptance_tests 
#     TEST_PREFIX ACCEPTANCE_TEST_)
