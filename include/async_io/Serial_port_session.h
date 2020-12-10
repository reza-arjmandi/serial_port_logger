#pragma once

template<
typename Ostream_type,
typename IO_read_utils_type,
typename Error_observer_type,
typename IO_object_type,
typename Sync_time_updater_type>
class Serial_port_session {

public:

    using Output_stream_type = Ostream_type;
    using Syncronization_time_updater_type = Sync_time_updater_type; 

    Serial_port_session(
        Ostream_type& ostream,
        IO_read_utils_type& IO_read_utils,
        std::shared_ptr<IO_object_type> IO_object,
        Error_observer_type& observer,
        Syncronization_time_updater_type& sync_time_updater,
        bool enable_add_sync_tags):
        _ostream {ostream},
        _IO_read_utils {IO_read_utils},
        _IO_object {IO_object},
        _error_observer {observer},
        _sync_time_updater {sync_time_updater},
        _enable_add_sync_tags {enable_add_sync_tags}
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
                if(_enable_add_sync_tags) {
                    _ostream 
                        << _sync_time_updater.get_time_tag() 
                        << std::string(",") 
                        << _buffer_str;
                }
                else {
                    _ostream << _buffer_str;
                }
                async_read();
            });
    }

    IO_read_utils_type& _IO_read_utils;
    Error_observer_type& _error_observer;
    Ostream_type& _ostream;
    std::shared_ptr<IO_object_type> _IO_object;
    std::string _buffer_str;
    bool _start {false};
    Syncronization_time_updater_type& _sync_time_updater;
    bool _enable_add_sync_tags {false};

};