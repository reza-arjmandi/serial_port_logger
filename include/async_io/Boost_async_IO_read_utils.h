#pragma once

class Boost_async_IO_read_utils {

public:

    template<typename IO_object_type, typename Read_handler_type>
    void async_read_line(
        std::shared_ptr<IO_object_type> IO_object_, 
        std::string & str, 
        Read_handler_type&& handler)
    {
        str.clear();
        boost::asio::async_read_until(
            *IO_object_, boost::asio::dynamic_buffer(str), '\n', handler);
    }

};