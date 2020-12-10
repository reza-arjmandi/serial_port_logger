#pragma once

template<
typename Timer_factory_type, 
typename Duration_type>
class Syncronization_time_updater {

public:

    Syncronization_time_updater(
        std::shared_ptr<Timer_factory_type> timer_factory,
        const Duration_type& update_duration):
        _update_duration {update_duration}
    {
        _handler = 
        [&](const auto& error_code) {
            if(error_code) {
                return;
            }
            _tag++;
            set_timer();
        };

        _timer = timer_factory->create();
        set_timer();
    }

    std::size_t get_time_tag() const
    {
        return _tag;
    }

private:

    void set_timer()
    {
        _timer->expires_from_now(_update_duration);
        _timer->async_wait(_handler);
    }

    typename Timer_factory_type::Timer_type _timer;
    std::atomic<std::size_t> _tag = 0;
    Duration_type _update_duration;
    typename Timer_factory_type::Timer_handler_type _handler;

};