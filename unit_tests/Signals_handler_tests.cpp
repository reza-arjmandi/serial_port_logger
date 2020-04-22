#include "pch.h"

#include "async_io/Signals_handler.h"

#include "test/Random_generator.h"
#include "test/Mock_signal_controller.h"
#include "test/Mock_signal_controller_factory.h"
#include "test/Mock_serial_port_logger.h"
#include "test/Mock_IO_context.h"

#define SIGNAL1 1
#define SIGNAL2 2
#define SIGNAL3 3

using namespace testing;

class Signals_handler_tests : public Test {

public:

    using Handler_type = std::function<void(int, int)>;
    using Signal_controller_type = Mock_signal_controller<Handler_type>;
    using Signal_controller_factory_type = 
        Mock_signal_controller_factory<Signal_controller_type>;

private:

    std::shared_ptr<Signal_controller_factory_type> _signal_controller_factory
        = std::make_shared<Signal_controller_factory_type>();
    Random_generator _random_generator;
    std::vector<Mock_serial_port_logger> _serial_port_loggers = 
        std::vector<Mock_serial_port_logger>(
            _random_generator.generate_integer(10, 100));
    std::vector<int> _signals = {SIGNAL1, SIGNAL2, SIGNAL3};
    std::shared_ptr<Signal_controller_type> _signal_controller = 
        std::make_shared<Signal_controller_type>();
    Handler_type _handler;
    Mock_IO_context _io_context;
    
};


TEST_F(Signals_handler_tests, 
When_it_is_constructed_\
Then_a_signal_controller_is_created)
{
    EXPECT_CALL(*_signal_controller_factory, create(_signals)).WillOnce(
        Return(_signal_controller));

    auto signals_handler = Signals_handler(
        _io_context,
        _signal_controller_factory, 
        _serial_port_loggers, 
        _signals);
}

TEST_F(Signals_handler_tests, 
When_start_is_called_\
Then_async_wait_of_signal_controller_is_called)
{
    EXPECT_CALL(*_signal_controller_factory, create(_signals)).WillOnce(
        Return(_signal_controller));

    auto signals_handler = Signals_handler(
        _io_context,
        _signal_controller_factory, 
        _serial_port_loggers, 
        _signals);

    EXPECT_CALL(*_signal_controller, async_wait(_)).Times(1);
    signals_handler.async_wait();
}

TEST_F(Signals_handler_tests, 
Given_start_is_called_\
When_SIGINT_or_SIGTERM_is_raised_\
Then_all_serial_port_loggers_must_be_stopped)
{
    EXPECT_CALL(*_signal_controller_factory, create(_signals)).WillOnce(
        Return(_signal_controller));

    auto signals_handler = Signals_handler(
        _io_context,
        _signal_controller_factory, 
        _serial_port_loggers, 
        _signals);

    EXPECT_CALL(*_signal_controller, async_wait(_)).WillOnce(
        SaveArg<0>(&_handler));
    signals_handler.async_wait();

    InSequence _sequence;

    for(auto& logger : _serial_port_loggers) {
        EXPECT_CALL(logger, stop()).Times(1);
    }
    EXPECT_CALL(_io_context, stop()).Times(1);

    _handler(1, 2);
}