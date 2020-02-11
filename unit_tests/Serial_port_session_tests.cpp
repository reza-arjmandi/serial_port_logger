#include "pch.h"

#include "async_io/Serial_port_session.h"
#include "async_io/Serial_port_session_exception.h"

#include "test/Mock_IO_object.h"
#include "test/Mock_ostream.h"
#include "test/Mock_IO_read_utils.h"
#include "test/Mock_error_code.h"
#include "test/Random_generator.h"
#include "test/Mock_error_observer.h"

using namespace testing;

class Serial_port_session_tests : public Test {

public:

    using Read_handler_type = 
        std::function<void(const Mock_error_code&, std::size_t size)>;
    using IO_read_utils_type = 
        Mock_IO_read_utils<Mock_IO_object, Read_handler_type>;
    using Session_type = Serial_port_session<
        Mock_ostream, 
        IO_read_utils_type,
        Mock_error_observer,
        Mock_IO_object>;

    std::shared_ptr<Mock_IO_object> _IO_object {
        std::make_shared<Mock_IO_object>()};
    IO_read_utils_type _IO_read_utils;
    Mock_error_code _error_code;
    std::size_t _size {1000};
    Mock_ostream _ostream;
    std::string _error_message {"error_message"};
    Random_generator _random_generator;
    Mock_error_observer _error_observer;

    Session_type _Serial_port_session {
        _ostream, 
        _IO_read_utils, 
        _IO_object, 
        _error_observer};

};

TEST_F(Serial_port_session_tests, 
When_start_is_called_\
Then_async_read_some_must_be_called)
{
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).Times(1);
    
    _Serial_port_session.start();
}

TEST_F(Serial_port_session_tests, 
When_start_is_called_\
Then_a_chain_of_async_read_must_be_created)
{
    Read_handler_type read_handler;
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).WillOnce(
        SaveArg<2>(&read_handler));

    _Serial_port_session.start();
    
    Expectation check_err = EXPECT_CALL(_error_code, operator_bool()).WillOnce(
        Return(false));
    EXPECT_CALL(_ostream, extract_operator(_)).Times(1).After(check_err);
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).Times(1);
    read_handler(_error_code, _size);
}

TEST_F(Serial_port_session_tests, 
When_stop_is_called_\
Then_chain_of_async_read_must_be_broken)
{
    Read_handler_type read_handler;
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).WillOnce(
        SaveArg<2>(&read_handler));
    _Serial_port_session.start();
    _Serial_port_session.stop();
    
    Expectation check_err = EXPECT_CALL(_error_code, operator_bool()).WillOnce(
        Return(false));
    EXPECT_CALL(_ostream, extract_operator(_)).Times(1).After(check_err);
    read_handler(_error_code, _size);
}

TEST_F(Serial_port_session_tests, 
When_start_is_called_twice_\
Then_nothing_must_be_happen)
{
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).Times(1);
    _Serial_port_session.start();
    _Serial_port_session.start();
}

TEST_F(Serial_port_session_tests, 
When_start_is_called_if_an_error_occurred_while_reading\
Then_an_exception_must_be_thrown)
{
    Read_handler_type read_handler;
    EXPECT_CALL(_IO_read_utils, async_read_line(_IO_object, _, _)).WillOnce(
        SaveArg<2>(&read_handler));
    _Serial_port_session.start();
    
    Expectation check_err = EXPECT_CALL(_error_code, operator_bool()).WillOnce(
        Return(true));

    EXPECT_CALL(_error_observer, notify_error(_)).Times(1).After(
        check_err);

    read_handler(_error_code, _size);
}