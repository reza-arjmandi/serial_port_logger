#pragma once

class Mock_timer {

public:

    MOCK_METHOD(void, expires_from_now, (int));
    MOCK_METHOD(void, async_wait, (std::shared_ptr<std::function<void()>>));
    MOCK_METHOD(void, async_wait, (std::function<void()>));

};