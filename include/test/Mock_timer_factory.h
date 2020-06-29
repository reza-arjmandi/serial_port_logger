#pragma once

#include "test/Mock_error_code.h"

class Mock_timer_factory {

public:

    using Timer_type = std::shared_ptr<Mock_timer>;
    using Timer_handler_type = std::function<void(const Mock_error_code&)>;

    MOCK_METHOD(Timer_type, create, ());

};