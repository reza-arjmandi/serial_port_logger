#pragma once

template <
class Signal_controller_factory_type, 
class Logger_type,
class IO_context_type> 
class Signals_handler {

public:

    Signals_handler(
        IO_context_type& io_context,
        std::shared_ptr<Signal_controller_factory_type> 
            signal_controller_factory,
        std::vector<Logger_type>& loggers,
        const std::vector<int>& signals) :
            _io_context {io_context},
            _loggers {loggers}
        {
            _signal_controller = signal_controller_factory->create(signals);
        }

    void async_wait()
    {
        _signal_controller->async_wait([&](const auto& err, auto sig) {
                for(auto& logger : _loggers) {
                    logger.stop();
                }
                _io_context.stop();
            });
    }

private:

    IO_context_type& _io_context;
    typename Signal_controller_factory_type::Signal_controller_type 
        _signal_controller;
    std::vector<Logger_type>& _loggers;
};