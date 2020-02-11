include(Util)

find_or_get(GMock)
find_or_get(GTest)
find_or_get(Threads)
find_or_get(Boost)

set(cpp_files 
    main.cpp 
    Serial_port_config_file_parser_tests.cpp)

add_executable(integration_tests ${cpp_files})
target_include_directories(integration_tests PUBLIC
                            ${INCLUDE_DIRS}
                            ${Boost_INCLUDE_DIRS}
                            ${GMOCK_INCLUDE_DIRS})
target_precompile_headers(integration_tests PUBLIC pch.h)
target_link_libraries(integration_tests 
    async_io
    Threads::Threads
    stdc++fs
    ${GMOCK_BOTH_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES})
set_property(TARGET integration_tests PROPERTY CXX_STANDARD 17)

if(UNIX)
    include(GNUInstallDirs)
    install(TARGETS integration_tests 
        TYPE RUNTIME)
endif()
