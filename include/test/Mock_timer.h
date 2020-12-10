#pragma once

#include "Mock_error_code.h"

class Mock_timer {

public:

    MOCK_METHOD(void, expires_from_now, (int));
    MOCK_METHOD(void, async_wait, (std::shared_ptr<std::function<void()>>));
    MOCK_METHOD(void, async_wait, (
        std::function<void(const Mock_error_code&)>));

};