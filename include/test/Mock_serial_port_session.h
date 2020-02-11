#pragma once

#include "test/Mock_serial_port.h"

class Mock_serial_port_session {

public:

    MOCK_METHOD(void, start, ());
    MOCK_METHOD(void, stop, ());

};