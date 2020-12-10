#pragma once

#include "async_io/Serial_port_session.h"
#include "async_io/Serial_port_connection.h"
#include "async_io/Serial_port_session_factory.h"
#include "async_io/Boost_serial_port_factory.h"
#include "async_io/Boost_async_IO_read_utils.h"
#include "async_io/Boost_serial_port_options.h"
#include "async_io/Async_procrastinator.h"
#include "async_io/Boost_timer_factory.h"

#include "serial_port_logger/Syncronization_time_updater.h"
#include "serial_port_logger/File_ostream.h"

class Serial_port_logger_dependency_injector {

public:

    using Timer_factory_type = Boost_timer_factory;
    using Millo_seconds_type = boost::posix_time::milliseconds;
    using Syncronization_time_updater_type = 
        Syncronization_time_updater<
            Timer_factory_type, 
            Millo_seconds_type>; 
    using Async_IO_read_utils_type  = Boost_async_IO_read_utils;
    using Serial_port_factory_type = Boost_serial_port_factory;
    using Async_procrastinator_type = 
        Async_procrastinator<Timer_factory_type>;
    using Seconds_type = boost::posix_time::seconds;
    using Error_code_type = boost::system::error_code;
    using Serial_port_type = boost::asio::serial_port;
    using Session_type = Serial_port_session<
        File_ostream, 
        Async_IO_read_utils_type, 
        Serial_port_logger,
        Serial_port_type,
        Syncronization_time_updater_type>;
    using Session_factory_type = 
        Serial_port_session_factory<Session_type>;
    using IO_context_type = boost::asio::io_context;
    using Serial_port_connection_type = Serial_port_connection<
        Serial_port_factory_type, 
        Session_factory_type>;
    
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

    std::shared_ptr<Timer_factory_type> get_timer_factory() const
    {
        return _timer_factory;
    }

private:

    boost::asio::io_context _io_context_ {1};
    Serial_port_factory_type _serial_port_factory_type {_io_context_};
    Async_IO_read_utils_type _async_IO_read_utils;
    std::shared_ptr<Timer_factory_type> _timer_factory {
        std::make_shared<Timer_factory_type>(_io_context_)};
    Async_procrastinator_type _async_procrastinator {_timer_factory};


};