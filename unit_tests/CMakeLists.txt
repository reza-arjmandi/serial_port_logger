include(Util)

find_or_get(GMock)
find_or_get(GTest)
find_or_get(Threads)

set(cpp_files 
    main.cpp 
    Serial_port_connection_tests.cpp
    Serial_port_session_tests.cpp
    Async_procrastinator_tests.cpp
    Signals_handler_tests.cpp)

add_executable(serial_port_logger_unit_tests ${cpp_files})
target_include_directories(serial_port_logger_unit_tests PUBLIC
                            ${INCLUDE_DIRS}
                            ${GMOCK_INCLUDE_DIRS})
target_precompile_headers(serial_port_logger_unit_tests PUBLIC pch.h)
target_link_libraries(serial_port_logger_unit_tests 
    async_io
    Threads::Threads
    stdc++fs
    ${GMOCK_BOTH_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES})
set_property(TARGET serial_port_logger_unit_tests PROPERTY CXX_STANDARD 17)

if(UNIX)
    include(GNUInstallDirs)
    install(TARGETS serial_port_logger_unit_tests 
        TYPE RUNTIME)
endif()
