#pragma once

class Boost_signal_set_factory {

public:

    using Signal_controller_type = std::shared_ptr<boost::asio::signal_set>;

    Boost_signal_set_factory(boost::asio::io_context& io_context_) :
        _io_context {io_context_}
    {
    }

    Signal_controller_type create(const std::vector<int>& signals)
    {
        auto result = std::make_shared<boost::asio::signal_set>(_io_context);
        for(const auto& signal : signals) {
            result->add(signal);
        }
        return result;
    }

private:

    boost::asio::io_context& _io_context;

};