#pragma once

#include "serial_port_logger/Serial_port_logger_dependency_injector.h"
#include "serial_port_logger/File_ostream.h"
#include "serial_port_logger/Log_type.h"

class Serial_port_logger {

public:

    using Session_type = Serial_port_session<
        File_ostream, 
        Serial_port_logger_dependency_injector::Async_IO_read_utils_type, 
        Serial_port_logger,
        Serial_port_logger_dependency_injector::Serial_port_type>;
    using Session_factory_type = 
        Serial_port_session_factory<Session_type>;
    using Serial_port_connection_type = Serial_port_connection<
        Serial_port_logger_dependency_injector::Serial_port_factory_type, 
        Session_factory_type>;
    using Error_type = Serial_port_logger_dependency_injector::Error_code_type;
    using Seconds_type = Serial_port_logger_dependency_injector::Seconds_type;
    using Async_procrastinator_type = 
        Serial_port_logger_dependency_injector::Async_procrastinator_type;

    Serial_port_logger(
        const Log_type& log_type,
        const Serial_port_config& serial_port_config,
        Serial_port_logger_dependency_injector& dependency_injector)
        : _log_file {serial_port_config.log_file}
         , _session_factory {
             dependency_injector.get_async_IO_read_utils(), _log_file, *this}
         , _serial_port_connection {
             serial_port_config.driver, 
             dependency_injector.get_serial_port_factory(), 
             serial_port_config.options, 
             _session_factory}
         , _async_procrastinator {
            dependency_injector.get_async_procrastinator()}
    {
        boost::system::error_code error_code;
        if(log_type == Log_type::LOG_DATA_TO_FILE){
            _serial_port_connection.start_async(error_code);
            if(error_code){
                reconnect_serial_connection();
            }
        }

        if(log_type == Log_type::IS_CONEECTED){
            _serial_port_connection.is_connected(error_code);
            if(!error_code){
                std::cout 
                    << serial_port_config.name 
                    << " is connected successfully" 
                    << std::endl;
            }
            else{
                std::cout 
                    << serial_port_config.name 
                    << " is not connected" 
                    << std::endl;
            }
        }
    }

    void stop()
    {
        Error_type error_code;
        _serial_port_connection.stop(error_code);
        _log_file.close();
    }

    void notify_error(const Error_type& ec)
    {
        reconnect_serial_connection();
    }

private:

    void reconnect_serial_connection()
    {
        Error_type error_code;
        _serial_port_connection.stop(error_code);
        _serial_port_connection.start_async(error_code);
        if(error_code){
            _async_procrastinator.postpone(
                    [&](const auto& ec)
                    {
                        reconnect_serial_connection();
                    }, _timer_duration);
        }
    }

    Async_procrastinator_type& _async_procrastinator;
    File_ostream _log_file;
    Session_factory_type _session_factory;
    Serial_port_connection_type _serial_port_connection; 
    Seconds_type _timer_duration {10};

};