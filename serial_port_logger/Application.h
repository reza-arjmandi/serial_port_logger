#pragma once

#include "serial_port_logger/Serial_port_config_file_parser.h"
#include "serial_port_logger/Serial_port_logger.h"
#include "serial_port_logger/Serial_port_logger_dependency_injector.h"

class Application {

public:

    Application(const fs::path& config_file);
    int run();

private:

    Serial_port_config_file_parser _config_file_parser;
    Serial_port_logger_dependency_injector _dependency_injector;
    std::vector<Serial_port_logger> _serial_port_loggers;
    
};