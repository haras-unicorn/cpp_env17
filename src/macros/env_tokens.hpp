#ifndef ENV_TOKENS_HPP
#define ENV_TOKENS_HPP

// basic

#define SKIP
#define EMPTY ()
#define COMMA ,
#define OPEN (
#define CLOSE )

#define SINGLE(_) _
#define PACK(...) __VA_ARGS__
#define ERASE(_)
#define CONSUME(...)

ENV_TEST_CASE("basic")
{
    struct
    {
        void g() { }
    } test{ };
    test.g EMPTY;
    [[maybe_unused]] int a = OPEN 3 CLOSE COMMA b = 2;
    static_cast<void>(b);

    ERASE(something)
    CONSUME(other, thing)

    SKIP
}

// strings

#define CAT_IMPL(_lhs, _rhs) _lhs##_rhs
#define CAT(_lhs, _rhs) CAT_IMPL(_lhs, _rhs)

#define INTER_IMPL(_lhs, _middle, _rhs) _lhs##_middle##_rhs
#define INTER(_lhs, _middle, _rhs) INTER_IMPL(_lhs, _middle, _rhs)

#define STRING_IMPL(...) #__VA_ARGS__
#define STRING_VAR(...) STRING_IMPL(__VA_ARGS__)
#define STRING(...) STRING_VAR(__VA_ARGS__)

ENV_TEST_CASE("strings")
{
    const auto string = ENV_STD::string{STRING(INTER(first_, second_, CAT(third_, fourth)), fifth)};

    REQUIRE_EQ(string, "first_second_third_fourth, fifth");
}

// spread

#define SPREAD_IMPL(_tuple) SINGLE(PACK _tuple)
#define SPREAD(_tuple) SPREAD_IMPL(_tuple)

ENV_TEST_CASE("spread")
{
    [[maybe_unused]] int a[]{SPREAD((1, 2, 3))};
}

#endif // ENV_TOKENS_HPP
