#pragma once

template<typename Timer_factory_type>
class Async_procrastinator {

public:

    Async_procrastinator(std::shared_ptr<Timer_factory_type> timer_factory)
    {
        _timer = timer_factory->create();
    }

    template<
    typename Handler_type,
    typename Duration_type>
    void postpone(Handler_type handler, Duration_type duration)
    {
        _timer->expires_from_now(duration);
        _timer->async_wait(handler);
    }

private:

    typename Timer_factory_type::Timer_type _timer;

};