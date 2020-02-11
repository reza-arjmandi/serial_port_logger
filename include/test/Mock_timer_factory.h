#pragma once

class Mock_timer_factory {

public:

    using Timer_type = std::shared_ptr<Mock_timer>;

    MOCK_METHOD(Timer_type, create, ());

};