#pragma once

#include "async_io/Serial_port_session.h"
#include "async_io/Boost_async_IO_read_utils.h"
#include "serial_port_logger/Serial_port_logger.h"

class Serial_port_logger;

template<typename Session_type_>
class Serial_port_session_factory {

public:

    using Session_type = std::shared_ptr<Session_type_>;

    Serial_port_session_factory(
        Boost_async_IO_read_utils& async_IO_read_utils, 
        typename Session_type_::Output_stream_type& ostream_,
        Serial_port_logger& serial_port_logger):
            _async_IO_read_utils {async_IO_read_utils},
            _ostream {ostream_},
            _serial_port_logger {serial_port_logger}
    {
    }

    Session_type create(std::shared_ptr<boost::asio::serial_port> io_object)
    {
        return std::make_shared<Session_type_>(
            _ostream, _async_IO_read_utils, io_object, _serial_port_logger);
    }

private:

    Boost_async_IO_read_utils& _async_IO_read_utils; 
    typename Session_type_::Output_stream_type& _ostream;
    Serial_port_logger& _serial_port_logger;

};