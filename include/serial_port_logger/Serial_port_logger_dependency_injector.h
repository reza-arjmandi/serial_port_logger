#pragma once

#include "async_io/Serial_port_session.h"
#include "async_io/Serial_port_connection.h"
#include "async_io/Serial_port_session_factory.h"
#include "async_io/Boost_serial_port_factory.h"
#include "async_io/Boost_async_IO_read_utils.h"
#include "async_io/Boost_serial_port_options.h"
#include "async_io/Async_procrastinator.h"
#include "async_io/Boost_timer_factory.h"

class Serial_port_logger_dependency_injector {

public:

    using Async_IO_read_utils_type  = Boost_async_IO_read_utils;
    using Serial_port_factory_type = Boost_serial_port_factory;
    using Async_procrastinator_type = 
        Async_procrastinator<Boost_timer_factory>;
    using Seconds_type = boost::posix_time::seconds;
    using Error_code_type = boost::system::error_code;
    using Serial_port_type = boost::asio::serial_port;
    using IO_context_type = boost::asio::io_context;
    
    Serial_port_factory_type& get_serial_port_factory()
    {
        return _serial_port_factory_type;
    }

    Async_IO_read_utils_type& get_async_IO_read_utils()
    {
        return _async_IO_read_utils;
    }

    Async_procrastinator_type& get_async_procrastinator()
    {
        return _async_procrastinator;
    }

    boost::asio::io_context& get_io_context()
    {
        return _io_context_;
    }

private:

    boost::asio::io_context _io_context_ {1};
    Serial_port_factory_type _serial_port_factory_type {_io_context_};
    Async_IO_read_utils_type _async_IO_read_utils;
    std::shared_ptr<Boost_timer_factory> _timer_factory {
        std::make_shared<Boost_timer_factory>(_io_context_)};
    Async_procrastinator_type _async_procrastinator {_timer_factory};


};