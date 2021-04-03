#ifndef ENV_CONCURRENCY_REQUIREMENTS_HPP
#define ENV_CONCURRENCY_REQUIREMENTS_HPP


// notes: https://en.cppreference.com/w/cpp/named_req/BasicLockable
EXPR_CHECK_UNARY(
        is_std_basic_lockable,
        /* These have a bit more effect constraints */
        declval<T>().lock(),
        declval<T>().unlock());

ENV_TEST_CASE("std basic lockable")
{
    STD_REQUIRE(is_std_basic_lockable_g<ENV_STD::mutex>);
    STD_REQUIRE(!is_std_basic_lockable_g<int>);
}


// https://en.cppreference.com/w/cpp/named_req/Lockable
EXPR_CHECK_UNARY(
        is_std_lockable,
        COND_EXPR(is_std_basic_lockable_gs<T>),

        /* Return value + effects requirement */
        declval<T>().try_lock());

ENV_TEST_CASE("std lockable")
{
    STD_REQUIRE(is_std_lockable_g<ENV_STD::mutex>);
    STD_REQUIRE(!is_std_lockable_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/TimedLockable
EXPR_CHECK_UNARY(
        is_std_timed_lockable,
        /* requirements + return value */

        declval<T>().try_lock_for(
                declval<ENV_STD::chrono::duration<ENV_STD::size_t>>()),

        declval<T>().try_lock_until(
                declval<ENV_STD::chrono::time_point<
                        ENV_STD::chrono::system_clock,
                        ENV_STD::chrono::duration<ENV_STD::size_t>>>()));

ENV_TEST_CASE("std timed lockable")
{
    STD_REQUIRE(is_std_timed_lockable_g<ENV_STD::timed_mutex>);
    STD_REQUIRE(!is_std_timed_lockable_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/Mutex
COND_CHECK_UNARY(
        is_std_mutex,
        /* there is a lot of constraints for the methods */
        is_std_lockable_gs<T>,

        is_std_default_constructible_gs<T>,
        is_std_destructible_gs<T>,

        /* the standard is a bit vague on these ones,
         * so I'm assuming they mean this */
        neg_vt<is_std_copy_assignable_gs<T>,
               is_std_copy_constructible_gs<T>,
               is_std_move_assignable_gs<T>,
               is_std_move_constructible_gs<T>>);

ENV_TEST_CASE("std mutex")
{
    STD_REQUIRE(is_std_mutex_g<ENV_STD::mutex>);
    STD_REQUIRE(!is_std_mutex_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/TimedMutex
COND_CHECK_UNARY(
        is_std_timed_mutex,
        is_std_mutex_gs<T>,

        /* lots of constraints on methods */
        is_std_timed_lockable_gs<T>);

ENV_TEST_CASE("std timed mutex")
{
    STD_REQUIRE(is_std_timed_mutex_g<ENV_STD::timed_mutex>);
    STD_REQUIRE(!is_std_timed_mutex_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/SharedMutex
EXPR_CHECK_UNARY(
        is_std_shared_mutex,
        COND_EXPR(is_std_mutex_gs<T>),

        /* lots of requirements for these methods +
         * all of them execute in a total order */
        declval<T>().lock_shared(),
        declval<T>().try_lock_shared(),
        declval<T>().unlock_shared());

ENV_TEST_CASE("std shared mutex")
{
    STD_REQUIRE(is_std_shared_mutex_g<ENV_STD::shared_mutex>);
    STD_REQUIRE(!is_std_shared_mutex_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/SharedTimedMutex
EXPR_CHECK_UNARY(
        is_std_shared_timed_mutex,
        COND_EXPR(is_std_shared_mutex_gs<T>,
                  is_std_timed_mutex_gs<T>),

        /* lots of requirements for these two */
        declval<T>().try_lock_shared_for(
                declval<
                        ENV_STD::chrono::duration<
                                ENV_STD::size_t>>()),

        declval<T>().try_lock_shared_until(
                declval<ENV_STD::chrono::time_point<
                        ENV_STD::chrono::system_clock,
                        ENV_STD::chrono::duration<ENV_STD::size_t>>>()));

ENV_TEST_CASE("std shared timed mutex")
{
    STD_REQUIRE(is_std_shared_timed_mutex_g<ENV_STD::shared_timed_mutex>);
    STD_REQUIRE(!is_std_shared_timed_mutex_g<int>);
}


#endif // ENV_CONCURRENCY_REQUIREMENTS_HPP
