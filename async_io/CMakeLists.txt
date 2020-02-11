include(Util)

find_or_get(Boost)

set(cpp_files
    Serial_port_session_exception.cpp
    Serial_port_config_file_parser.cpp
    Serial_port_config_file_parser_exception.cpp)

add_library(async_io STATIC ${cpp_files})
target_include_directories(async_io PUBLIC
                            ${INCLUDE_DIRS}
                            ${INCLUDE_DIRS}/async_io
                            ${Boost_INCLUDE_DIRS})
target_precompile_headers(async_io PUBLIC pch.h)
set_property(TARGET async_io PROPERTY CXX_STANDARD 17)

