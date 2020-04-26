#pragma once

#include "async_io/Boost_serial_port_options.h"

struct Serial_port_config {

    using Serial_port_options_type = Boost_serial_port_options;

    Serial_port_options_type options;
    fs::path log_file;
    fs::path driver;
    std::string name;

};