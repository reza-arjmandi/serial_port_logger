#include "async_io/Serial_port_session_exception.h"

Serial_port_session_exception::Serial_port_session_exception(
    const std::string& device,
    const std::string& message):
    _device(device),
    _message(message)
{
}

const char* Serial_port_session_exception::what() const noexcept
{
    return _message.c_str();
}

std::string Serial_port_session_exception::device() const
{
    return _device;
}