#pragma once

#include "test/Mock_serial_port.h"
#include "test/Mock_serial_port_options.h"

class Mock_serial_port_factory {

public:

    using Serial_port_type = std::shared_ptr<Mock_serial_port>;
    using Serial_port_options_type = Mock_serial_port_options;

	MOCK_METHOD(Serial_port_type, create, (), (const));

};