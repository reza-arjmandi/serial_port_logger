#pragma once

struct Boost_serial_port_options {

    using Baud_rate_type = boost::asio::serial_port_base::baud_rate;
    using Flow_control_type = boost::asio::serial_port_base::flow_control;
    using Parity_type = boost::asio::serial_port_base::parity;
    using Stop_bits_type = boost::asio::serial_port_base::stop_bits;
    using Character_size_type = boost::asio::serial_port_base::character_size;

    Baud_rate_type baud_rate;
    Flow_control_type flow_control;
    Parity_type parity;
    Stop_bits_type stop_bits;
    Character_size_type character_size;
    
};