#pragma once

class Mock_error_observer {
    
public:

    MOCK_METHOD(void, notify_error, (const Mock_error_code&));

};