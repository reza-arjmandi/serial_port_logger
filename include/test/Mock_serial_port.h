#pragma once

#include "test/Mock_error_code.h"
#include "test/Mock_serial_port_options.h"

class Mock_serial_port {

public:

    Mock_serial_port(Mock_serial_port&&)
    {
    }

    Mock_serial_port(Mock_serial_port&)
    {
    }

    Mock_serial_port() = default;

	MOCK_METHOD(void, open, (const std::string& device, Mock_error_code& ec));
	MOCK_METHOD(void, close, (Mock_error_code& ec));
    MOCK_METHOD(bool, is_open, (), (const));
    MOCK_METHOD(void, set_option, (unsigned int, Mock_error_code& ec));
    MOCK_METHOD(void, set_option, (
        Mock_serial_port_options::Flow_control, Mock_error_code& ec));
    MOCK_METHOD(void, set_option, (
        Mock_serial_port_options::Parity, Mock_error_code& ec));
    MOCK_METHOD(void, set_option, (
        Mock_serial_port_options::Stop_bits, Mock_error_code& ec));

};