#pragma once

#include "serial_port_logger/Serial_port_config_file_parser.h"
#include "serial_port_logger/Serial_port_logger.h"
#include "serial_port_logger/Serial_port_logger_dependency_injector.h"
#include "serial_port_logger/Log_type.h"

#include "async_io/Boost_signal_set_factory.h"
#include "async_io/Signals_handler.h"

class Application {

public:

    using Signals_handler_type = 
        Signals_handler<
            Boost_signal_set_factory, 
            Serial_port_logger, 
            Serial_port_logger_dependency_injector::IO_context_type>;

    Application(const Log_type& log_type, const fs::path& config_file);
    int run();

private:

    Serial_port_config_file_parser _config_file_parser;
    Serial_port_logger_dependency_injector _dependency_injector;
    std::vector<Serial_port_logger> _serial_port_loggers;
    std::shared_ptr<Boost_signal_set_factory> _signal_set_factory {
        std::make_shared<Boost_signal_set_factory>(
            _dependency_injector.get_io_context())};
    std::vector<int> _signals {std::vector<int>({SIGINT, SIGTERM})}; 
    
};