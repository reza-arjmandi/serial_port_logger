#pragma once

#include "test/Random_generator.h"

class Mock_ostream {

public:

    MOCK_METHOD(void, extract_operator, (const std::string&));
    MOCK_METHOD(void, extract_operator, (const std::size_t&));

    Mock_ostream& operator<<(const std::string& str)
    {
        extract_operator(str);
        return *this;
    }

    Mock_ostream& operator<<(const std::size_t& value)
    {
        extract_operator(value);
        return *this;
    }

};