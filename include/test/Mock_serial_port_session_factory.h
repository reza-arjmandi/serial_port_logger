#pragma once
#include "test/Mock_serial_port_session.h"
#include "test/Mock_serial_port.h"
#include "test/Mock_ostream.h"
#include "test/Mock_IO_read_utils.h"
#include "test/Mock_error_observer.h"
#include "test/Mock_IO_object.h"
#include "test/Mock_read_handler.h"

// class Mock_serial_port_session_factory_with_ctor {

// public:

//     using Session_type = std::shared_ptr<Mock_serial_port_session>;
//     using IO_read_utils_type = 
//         Mock_IO_read_utils<Mock_IO_object, Mock_read_handler>;

//     Mock_serial_port_session_factory_with_ctor(
//         Mock_ostream& ostream_, 
//         IO_read_utils_type& IO_read_utils,
//         Mock_error_observer& error_observer)
//     {
//         ctor(ostream_, IO_read_utils, error_observer);
//     }

// 	MOCK_METHOD(Session_type, create, 
//         (std::shared_ptr<Mock_serial_port>), (const));
// 	MOCK_METHOD(void, ctor, 
//         (Mock_ostream&, IO_read_utils_type&, Mock_error_observer&));

// };

class Mock_serial_port_session_factory {

public:

    using Session_type = std::shared_ptr<Mock_serial_port_session>;

	MOCK_METHOD(Session_type, create, 
        (std::shared_ptr<Mock_serial_port>), (const));

};