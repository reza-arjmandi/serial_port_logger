#pragma once

template<
typename Ostream_type,
typename IO_read_utils_type,
typename Error_observer_type,
typename IO_object_type>
class Serial_port_session {

public:

    using Output_stream_type = Ostream_type;

    Serial_port_session(
        Ostream_type& ostream,
        IO_read_utils_type& IO_read_utils,
        std::shared_ptr<IO_object_type> IO_object,
        Error_observer_type& observer):
        _ostream {ostream},
        _IO_read_utils {IO_read_utils},
        _IO_object {IO_object},
        _error_observer {observer}
    {
    }

    void start()
    {
        if(_start){
            return;
        }
        
        _start = true;
        async_read();
    }

    void stop()
    {
        _start = false;
    }

private:

    void async_read()
    {
        if(!_start){
            return;
        }

        _IO_read_utils.async_read_line(_IO_object, _buffer_str, 
        [&](const auto& ec, auto size)
            {
                if(ec){
                    _error_observer.notify_error(ec);
                    return;
                }
                _ostream << _buffer_str;
                async_read();
            });
    }

    IO_read_utils_type& _IO_read_utils;
    Error_observer_type& _error_observer;
    Ostream_type& _ostream;
    std::shared_ptr<IO_object_type> _IO_object;
    std::string _buffer_str;
    bool _start {false};

};