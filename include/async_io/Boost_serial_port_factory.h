#pragma once
#include "async_io/Boost_serial_port_options.h"

class Boost_serial_port_factory {

public:

    using Serial_port_type = std::shared_ptr<boost::asio::serial_port>;
    using Serial_port_options_type = Boost_serial_port_options;

    Boost_serial_port_factory(boost::asio::io_context& io_context_):
        _io_context {io_context_}
    {
    }
    
    Serial_port_type create()
    {
        return std::make_shared<boost::asio::serial_port>(_io_context);
    }

private:

    boost::asio::io_context& _io_context;

};