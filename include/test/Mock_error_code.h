#pragma once

class Mock_error_code {

public:

    operator bool() const
    {
        return operator_bool();
    }

    MOCK_METHOD(bool, operator_bool, (), (const));
    MOCK_METHOD(std::string, message, (), (const));

};