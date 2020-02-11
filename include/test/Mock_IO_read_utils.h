#pragma once

template<typename IO_object_type, typename Read_handler_type>
class Mock_IO_read_utils {

public: 

    MOCK_METHOD(void, async_read_line, (
        std::shared_ptr<IO_object_type>, 
        std::string &, 
        Read_handler_type&& handler));

};