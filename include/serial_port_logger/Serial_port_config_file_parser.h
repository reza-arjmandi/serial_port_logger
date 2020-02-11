#pragma once

#include "serial_port_logger/Serial_port_config.h"

class Serial_port_config_file_parser {

public:

    std::vector<Serial_port_config> get_devices_info(
        const fs::path& config_file);

private:

    Boost_serial_port_options::Baud_rate_type 
        get_baud_rate(const std::string& prefix) const;
    Boost_serial_port_options::Character_size_type 
        get_character_size(const std::string& prefix) const;

    boost::property_tree::ptree _container;

    Boost_serial_port_options::Flow_control_type str_to_flow_control(
        const std::string& str);
    Boost_serial_port_options::Parity_type str_to_parity(
        const std::string& str);
    Boost_serial_port_options::Stop_bits_type float_to_stop_bits(float value);

};