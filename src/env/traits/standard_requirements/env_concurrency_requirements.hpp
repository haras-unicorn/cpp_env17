#ifndef ENV_CONCURRENCY_REQUIREMENTS_HPP
#define ENV_CONCURRENCY_REQUIREMENTS_HPP


// notes: https://en.cppreference.com/w/cpp/named_req/BasicLockable
EXPR_CHECK_UNARY
(
        is_std_basic_lockable,
/* These have a bit more effect constraints */
        nonce(declval<T>().lock()),
        nonce(declval<T>().unlock())
);

ENV_TEST_CASE("std basic lockable")
{
    STD_REQUIRE(is_std_basic_lockable_g<std::mutex>);
    STD_REQUIRE(!is_std_basic_lockable_g<int>);
}



// https://en.cppreference.com/w/cpp/named_req/Lockable
EXPR_CHECK_UNARY
(
        is_std_lockable,
        COND_EXPR(is_std_basic_lockable_g<T>),
/* Return value + effects requirement */
        declval<T>().try_lock()
);

ENV_TEST_CASE("std lockable")
{
    STD_REQUIRE(is_std_lockable_g < ENV_STD::mutex >);
    STD_REQUIRE(!is_std_lockable_g < int >);
}


#endif // ENV_CONCURRENCY_REQUIREMENTS_HPP
