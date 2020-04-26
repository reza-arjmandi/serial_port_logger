#include "pch.h"

#include "async_io/Serial_port_connection.h"

#include "test/Random_generator.h"
#include "test/Mock_serial_port_factory.h"
#include "test/Mock_serial_port_session_factory.h"
#include "test/Mock_serial_port_session.h"
#include "test/Mock_error_code.h"

using namespace testing;

class Serial_port_connection_tests : public Test {

public:

    using Serial_port_connection_type = 
        Serial_port_connection<
            Mock_serial_port_factory, 
            Mock_serial_port_session_factory>;

    void SetUp() override
    {
        EXPECT_CALL(_serial_port_factory, create()).WillOnce(
            Return(_serial_port));

        _logger = std::make_unique<Serial_port_connection_type>(
            _device, 
            _serial_port_factory, 
            _options, 
            _session_factory);
    }

    Mock_serial_port_options::Flow_control generate_random_flow_control()
    {
        switch(_random_generator.generate_integer(0, 2)){
            case(0):
                return Mock_serial_port_options::Flow_control::none;
            case(1):
                return Mock_serial_port_options::Flow_control::software;
            case(2):
                return Mock_serial_port_options::Flow_control::hardware;
            default:
                return Mock_serial_port_options::Flow_control::none;
        }
    }

    Mock_serial_port_options::Parity generate_random_parity()
    {
        switch(_random_generator.generate_integer(0, 2)){
            case(0):
                return Mock_serial_port_options::Parity::none;
            case(1):
                return Mock_serial_port_options::Parity::odd;
            case(2):
                return Mock_serial_port_options::Parity::even;
            default:
                return Mock_serial_port_options::Parity::none;
        }
    }

    Mock_serial_port_options::Stop_bits generate_random_stop_bits()
    {
        switch(_random_generator.generate_integer(0, 2)){
            case(0):
                return Mock_serial_port_options::Stop_bits::one;
            case(1):
                return Mock_serial_port_options::Stop_bits::onepointfive;
            case(2):
                return Mock_serial_port_options::Stop_bits::two;
            default:
                return Mock_serial_port_options::Stop_bits::one;
        }
    }

    Mock_serial_port_options generate_random_serial_port_options()
    {
        Mock_serial_port_options options;
        options.baud_rate = _random_generator.generate_integer(10, 100000);
        options.flow_control = generate_random_flow_control();
        options.parity = generate_random_parity();
        options.stop_bits = generate_random_stop_bits();
        options.character_size = 
            _random_generator.generate_integer(10, 100000);
        return options;
    }

    std::string generate_random_device()
    {
        return _random_generator.generate_string(
            _random_generator.generate_integer(10, 50));
    }

    Random_generator _random_generator;

    Mock_serial_port_factory _serial_port_factory;

    std::shared_ptr<Mock_serial_port> _serial_port {
        std::make_shared<Mock_serial_port>()};

    Mock_serial_port_factory::Serial_port_options_type _options {
        generate_random_serial_port_options()};
    std::string _device {generate_random_device()};

    Mock_serial_port_session_factory _session_factory;

    std::shared_ptr<Mock_serial_port_session> _session {
        std::make_shared<Mock_serial_port_session>()};

    Mock_error_code _error_code;
    
    std::unique_ptr<Serial_port_connection_type> _logger;

};

TEST_F(Serial_port_connection_tests, 
When_serial_port_constructed_\
Then_options_must_be_set_to_serial_port)
{
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_\
Then_a_session_must_be_created)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    Expectation check_err2 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    Expectation check_err3 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(false));
    
    Expectation set_option3 = EXPECT_CALL(*_serial_port, set_option(
        _options.parity, _)).Times(1).After(check_err1);
    Expectation check_err4 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option3).WillOnce(Return(false));

    Expectation set_option4 = EXPECT_CALL(*_serial_port, set_option(
        _options.stop_bits, _)).Times(1).After(check_err1);
    Expectation check_err5 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option4).WillOnce(Return(false));

    Expectation set_option5 = EXPECT_CALL(*_serial_port, set_option(
        _options.character_size, _)).Times(1).After(check_err1);
    Expectation check_err6 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option5).WillOnce(Return(false));

    EXPECT_CALL(_session_factory, create(_serial_port)).After(
        check_err6, check_err5, check_err4, check_err3, check_err2).WillOnce(
        Return(_session));
    EXPECT_CALL(*_session, start()).Times(1);

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_if_set_baud_rate_option_cause_an_error_code\
Then_function_must_be_return)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_if_set_flow_control_option_cause_an_error_code\
Then_function_must_be_return)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_if_set_parity_option_cause_an_error_code\
Then_function_must_be_return)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(false));

    Expectation set_option3 = EXPECT_CALL(*_serial_port, set_option(
        _options.parity, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option3).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_if_set_stop_bits_cause_an_error_code\
Then_function_must_be_return)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(false));

    Expectation set_option3 = EXPECT_CALL(*_serial_port, set_option(
        _options.parity, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option3).WillOnce(Return(false));

    Expectation set_option4 = EXPECT_CALL(*_serial_port, set_option(
        _options.stop_bits, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option4).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_if_set_character_size_cause_an_error_code\
Then_function_must_be_return)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1).After(is_open1);
    
    Expectation check_err1 = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    
    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(false));

    Expectation set_option3 = EXPECT_CALL(*_serial_port, set_option(
        _options.parity, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option3).WillOnce(Return(false));

    Expectation set_option4 = EXPECT_CALL(*_serial_port, set_option(
        _options.stop_bits, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option4).WillOnce(Return(false));

    Expectation set_option5 = EXPECT_CALL(*_serial_port, set_option(
        _options.character_size, _)).Times(1).After(check_err1);
    EXPECT_CALL(_error_code, operator_bool()).After(
        set_option5).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_And_serial_port_is_not_opened_\
Then_a_nothig_must_be_happened)
{
    Expectation is_open1 = EXPECT_CALL(*_serial_port, is_open()).WillOnce(
        Return(false));
    EXPECT_CALL(*_serial_port, open(_device, _)).Times(1).After(is_open1);
    EXPECT_CALL(_error_code, operator_bool()).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_start_async_is_called_If_serial_port_is_opened_\
Then_nothing_must_be_happened)
{
    EXPECT_CALL(*_serial_port, is_open()).WillOnce(Return(true));

    _logger->start_async(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_stop_is_called_\
Then_serial_port_must_be_closed)
{
    EXPECT_CALL(*_serial_port, is_open()).WillOnce(Return(true));
    EXPECT_CALL(*_serial_port, close(_)).Times(1);

    _logger->stop(_error_code);
}

TEST_F(Serial_port_connection_tests,
When_stop_is_called_\
Then_session_must_be_stopped_and_serial_port_closed)
{
    Expectation is_open1 = EXPECT_CALL(
        *_serial_port, is_open()).WillOnce(Return(false));

    EXPECT_CALL(*_serial_port, open(_device, _)).Times(1).After(is_open1);

    Expectation check_err1 = EXPECT_CALL(
        _error_code, operator_bool()).WillOnce(Return(false));

    Expectation set_option1 = EXPECT_CALL(*_serial_port, set_option(
        _options.baud_rate, _)).Times(1).After(check_err1);
    Expectation check_err2 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option1).WillOnce(Return(false));

    Expectation set_option2 = EXPECT_CALL(*_serial_port, set_option(
        _options.flow_control, _)).Times(1).After(check_err1);
    Expectation check_err3 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option2).WillOnce(Return(false));
    
    Expectation set_option3 = EXPECT_CALL(*_serial_port, set_option(
        _options.parity, _)).Times(1).After(check_err1);
    Expectation check_err4 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option3).WillOnce(Return(false));

    Expectation set_option4 = EXPECT_CALL(*_serial_port, set_option(
        _options.stop_bits, _)).Times(1).After(check_err1);
    Expectation check_err5 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option4).WillOnce(Return(false));

    Expectation set_option5 = EXPECT_CALL(*_serial_port, set_option(
        _options.character_size, _)).Times(1).After(check_err1);
    Expectation check_err6 = EXPECT_CALL(_error_code, operator_bool()).After(
        set_option5).WillOnce(Return(false));

    EXPECT_CALL(_session_factory, create(_serial_port)).After(
        check_err6, check_err5, check_err4, check_err3, check_err2).WillOnce(
        Return(_session));

    EXPECT_CALL(*_session, start()).Times(1);

    _logger->start_async(_error_code);

    Expectation stop_session = EXPECT_CALL(*_session, stop()).Times(1);
    Expectation is_open2 = EXPECT_CALL(
        *_serial_port, is_open()).After(stop_session).WillOnce(Return(true));
    EXPECT_CALL(*_serial_port, close(_)).Times(1).After(is_open2);

    _logger->stop(_error_code);
}


TEST_F(Serial_port_connection_tests, 
When_is_connected_is_called_\
Then_serial_port_connection_must_be_checked_\
case_1)
{
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1);
    Expectation check_err = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(false));
    EXPECT_CALL(*_serial_port, close(_)).Times(1).After(check_err);
    
    _logger->is_connected(_error_code);
}

TEST_F(Serial_port_connection_tests, 
When_is_connected_is_called_\
Then_serial_port_connection_must_be_checked_\
case_2)
{
    Expectation open = EXPECT_CALL(*_serial_port, open(
        _device, _)).Times(1);
    Expectation check_err = EXPECT_CALL(_error_code, operator_bool()).After(
        open).WillOnce(Return(true));
    
    _logger->is_connected(_error_code);
}