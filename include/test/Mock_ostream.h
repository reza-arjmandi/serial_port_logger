#pragma once

#include "test/Random_generator.h"

class Mock_ostream {

public:

    MOCK_METHOD(void, extract_operator, (const std::string&));

    void operator<<(const std::string& str)
    {
        extract_operator(str);
    }

};