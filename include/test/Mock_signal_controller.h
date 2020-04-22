#pragma once

template<typename Handler_type>
class Mock_signal_controller{

public:

    MOCK_METHOD(void, async_wait, (Handler_type&&));

};