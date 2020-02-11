#pragma once

class Boost_timer_factory {

public:

    using Timer_type = std::shared_ptr<boost::asio::deadline_timer>;

    Boost_timer_factory(boost::asio::io_context& io_context_):
        _io_context {io_context_}
    {
    }

    Timer_type create()
    {
        return std::make_shared<boost::asio::deadline_timer>(_io_context);
    }

private:

    boost::asio::io_context& _io_context;

};