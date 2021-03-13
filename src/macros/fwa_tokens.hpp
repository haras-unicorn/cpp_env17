#ifndef FWA_CORE_TOKENS_HPP
#define FWA_CORE_TOKENS_HPP


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

FWA_CORE_TEST_CASE("basic")
{
    SKIP

    struct { void g() { } } test{ };
    test.g EMPTY COMMA OPEN 3 CLOSE;

    ERASE(something)
    CONSUME(other, thing)
}


// strings

#define CAT_IMPL(_lhs, _rhs) _lhs ## _rhs
#define CAT(_lhs, _rhs) CAT_IMPL(_lhs, _rhs)

#define INTER_IMPL(_lhs, _middle, _rhs) _lhs ## _middle ## _rhs
#define INTER(_lhs, _middle, _rhs) INTER_IMPL(_lhs, _middle, _rhs)

#define STRING_IMPL(_) #_
#define STRING_VAR(_) STRING_IMPL(_)
#define STRING(...) STRING_VAR(PACK(__VA_ARGS__))

FWA_CORE_TEST_CASE ("strings")
{
    const auto string = FWA_STD::string{STRING(INTER(first_, second_, CAT(third_, fourth)))};

    REQUIRE_EQ(string, "first_second_third_fourth");
}


// spread

#define SPREAD_IMPL(_tuple) SINGLE(PACK _tuple)
#define SPREAD(_tuple) SPREAD_IMPL(_tuple)

FWA_CORE_TEST_CASE ("spread")
{
    [[maybe_unused]] int a[]{SPREAD((1, 2, 3))};
}


#endif // FWA_CORE_TOKENS_HPP
