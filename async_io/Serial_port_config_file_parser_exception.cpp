#include "serial_port_logger/Serial_port_config_file_parser_exception.h"

Serial_port_config_file_parser_exception::Serial_port_config_file_parser_exception(
    const std::string& message):
    _message {message}
{
}

const char* Serial_port_config_file_parser_exception::what() const noexcept
{
    return _message.c_str();
}