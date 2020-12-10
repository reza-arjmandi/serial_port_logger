#include "pch.h"

#include "test/Mock_timer.h"
#include "test/Mock_timer_factory.h"

#include "async_io/Async_procrastinator.h"

using namespace testing;

class Async_procrastinator_tests : public Test {

public:

    using Async_procrastinator_type = Async_procrastinator<Mock_timer_factory>;

    void SetUp() override
    {
        EXPECT_CALL(*_timer_factory, create()).WillOnce(Return(_timer));
        _async_procrastinator = 
            std::make_unique<Async_procrastinator_type>(_timer_factory);
    }

    std::unique_ptr<Async_procrastinator_type> _async_procrastinator;
    std::shared_ptr<Mock_timer_factory> _timer_factory {
        std::make_shared<Mock_timer_factory>()};
    std::shared_ptr<Mock_timer> _timer {std::make_shared<Mock_timer>()};

};

TEST_F(Async_procrastinator_tests, 
When_postpone_is_called_then_an_async_timer_must_be_created_and_initiated)
{
    auto duration = 10;
    std::shared_ptr<std::function<void()>> handler = 
        std::make_shared<std::function<void()>>([](){});
    EXPECT_CALL(*_timer, expires_from_now(duration)).Times(1);
    EXPECT_CALL(*_timer, async_wait(
        Matcher<std::shared_ptr<std::function<void()>>>(Eq(handler)))).Times(
            1);

    _async_procrastinator->postpone(handler, 10);
}