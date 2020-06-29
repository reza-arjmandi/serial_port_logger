#include "pch.h"

#include "serial_port_logger/Syncronization_time_updater.h"

#include "test/Random_generator.h"
#include "test/Mock_timer.h"
#include "test/Mock_timer_factory.h"

using namespace testing;

class Syncronization_time_updater_tests : public Test {

public:

    void SetUp() override
    {
        EXPECT_CALL(*_timer_factory, create()).WillOnce(Return(_timer));

        EXPECT_CALL(*_timer, expires_from_now(_update_duration)).Times(1);
        EXPECT_CALL(*_timer, async_wait(
            Matcher<std::function<void(const Mock_error_code&)>>(_))).WillOnce(
                SaveArg<0>(&_handler));

        _syncronization_time_updater = 
            std::make_unique<Syncronization_time_updater_type>(
                _timer_factory, _update_duration);
    }

    using Syncronization_time_updater_type = 
        Syncronization_time_updater<Mock_timer_factory, int>;

    std::unique_ptr<Syncronization_time_updater_type> 
        _syncronization_time_updater;
    std::shared_ptr<Mock_timer_factory> _timer_factory {
        std::make_shared<Mock_timer_factory>()};
    std::shared_ptr<Mock_timer> _timer {std::make_shared<Mock_timer>()};
    Random_generator _random_generator;
    int _update_duration {_random_generator.generate_integer(10, 1000000)};
    typename Mock_timer_factory::Timer_handler_type _handler;
    Mock_error_code _error_code;

};

TEST_F(Syncronization_time_updater_tests, construction)
{
}

TEST_F(Syncronization_time_updater_tests, 
If_timer_not_updated_\
Whenever_get_time_tag_is_called_it_must_return_same_value)
{
    auto number_of_call = _random_generator.generate_integer(10, 100);
    std::vector<std::size_t> tags; 

    for(auto i = 0; i < number_of_call; i++) {
        tags.push_back(_syncronization_time_updater->get_time_tag());
    }

    ASSERT_THAT(tags, Each(Eq(tags[0])));
}

TEST_F(Syncronization_time_updater_tests, 
When_timer_updated_\
Then_tag_must_be_incremented_of_duration_case1)
{
    auto tag0 = _syncronization_time_updater->get_time_tag();

    EXPECT_CALL(_error_code, operator_bool()).WillOnce(Return(false));

    EXPECT_CALL(*_timer, expires_from_now(_update_duration)).Times(1);
    EXPECT_CALL(*_timer, async_wait(
        Matcher<std::function<void(const Mock_error_code&)>>(_))).Times(1);
    _handler(_error_code);
    
    auto tag1 = _syncronization_time_updater->get_time_tag();

    ASSERT_THAT(tag1, Gt(tag0));
    ASSERT_THAT(tag1 - tag0, Eq(1));
}

TEST_F(Syncronization_time_updater_tests, 
When_timer_got_and_error_\
Then_tag_must_be_unchanged)
{
    auto tag0 = _syncronization_time_updater->get_time_tag();

    EXPECT_CALL(_error_code, operator_bool()).WillOnce(Return(true));
    _handler(_error_code);
    
    auto tag1 = _syncronization_time_updater->get_time_tag();

    ASSERT_THAT(tag1, Eq(tag0));
}

TEST_F(Syncronization_time_updater_tests, 
When_timer_updated_\
Then_tag_must_be_incremented_of_duration_case2)
{
    auto tag0 = _syncronization_time_updater->get_time_tag();

    EXPECT_CALL(_error_code, operator_bool()).WillOnce(Return(false));
    EXPECT_CALL(*_timer, expires_from_now(_update_duration)).Times(1);
    EXPECT_CALL(*_timer, async_wait(
        Matcher<std::function<void(const Mock_error_code&)>>(_))).Times(1);
    _handler(_error_code);

    auto tag1 = _syncronization_time_updater->get_time_tag();

    auto number_of_call = _random_generator.generate_integer(10, 100);
    for(auto i = 0; i < number_of_call; i++) {
        EXPECT_CALL(_error_code, operator_bool()).WillOnce(Return(false));
        EXPECT_CALL(*_timer, expires_from_now(_update_duration)).Times(1);
        EXPECT_CALL(*_timer, async_wait(
            Matcher<std::function<void(const Mock_error_code&)>>(_))).WillOnce(
                SaveArg<0>(&_handler));
        _handler(_error_code);
    }

    auto tag2 = _syncronization_time_updater->get_time_tag();

    ASSERT_THAT(tag1, Gt(tag0));
    ASSERT_THAT(tag1 - tag0, Eq(1));
    ASSERT_THAT(tag2 - tag1, Eq(number_of_call));
}