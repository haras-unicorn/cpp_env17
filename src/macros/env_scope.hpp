#ifndef ENV_SCOPE_HPP
#define ENV_SCOPE_HPP

// anon name

#define ANON_NAME CAT(_env_anon_, __LINE__)
#define SCOPE_SEMI typ(ANON_NAME) = void

ENV_NAMESPACE_TEST_BEGIN

let ANON_NAME{0};

SCOPE_SEMI;

ENV_NAMESPACE_TEST_END

// scopes

#define SCOPE_IMPL(...) \
    do                  \
    {                   \
        __VA_ARGS__     \
    } while (false)
#define SCOPE(...) SCOPE_IMPL(__VA_ARGS__)

ENV_TEST_CASE("scope")
{
    SCOPE(REQUIRE(true););
}

#define IFFY_IMPL(...) ([&]() noexcept { __VA_ARGS__ })() // implicitly constexpr if it can be as of c++17
#define IFFY(...) IFFY_IMPL(__VA_ARGS__)

ENV_TEST_CASE("iffy")
{
    constexpr auto a = IFFY(ret 1;);
    REQUIRE_EQ(a, 1);
}

// if

#define IF(_condition, _if_true, _if_false) SCOPE(if _condition { SPREAD(_if_true) } else {SPREAD(_if_false)})

ENV_TEST_CASE("if scope")
{
    IF(
        (true),
        (REQUIRE(true);),
        (REQUIRE(false);));

    IF(
        (false),
        (REQUIRE(false);),
        (REQUIRE(true);));
}

#if ENV_CPP >= 17
#define CMP_IF(_condition, _if_true, _if_false) IF(constexpr(_condition), _if_true, _if_false)
#else // CPP >= 17
#define CMP_IF(_condition, _if_true, _if_false) IF(_condition, _if_true, _if_false)
#endif // CPP >= 17

ENV_TEST_CASE("constexpr if")
{
    CMP_IF(
        (true),
        (REQUIRE(true);),
        (REQUIRE(false);));

    CMP_IF(
        (false),
        (REQUIRE(false);),
        (REQUIRE(true);));
}

// when

#define ON(_condition, ...) SCOPE(if _condition{__VA_ARGS__})

ENV_TEST_CASE("when")
{
    ON((true), REQUIRE(true););
    ON((false), REQUIRE(false););
}

#if ENV_CPP >= 17
#define CMP_ON(_condition, ...) ON(constexpr _condition, __VA_ARGS__)
#else // ENV_CPP >= 17
#define CMP_ON(_condition, ...) ON(_condition, __VA_ARGS__)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("when")
{
    CMP_ON((true), REQUIRE(true););
    CMP_ON((false), REQUIRE(false););
}

// tern

#define TERN(_condition, _if_true, _if_false) _condition ? SPREAD(_if_true) : SPREAD(_if_false)

ENV_TEST_CASE("cond")
{
    REQUIRE_EQ(TERN((true), (1), (2)), 1);
    REQUIRE_EQ(TERN((false), (1), (2)), 2);
}

#if ENV_CPP >= 17
#define CMP_TERN(_condition, _if_true, _if_false) IFFY(CMP_IF(_condition, (ret _if_true;), (ret _if_false;));)
#else // ENV_CPP >= 17
#define CMP_TERN(_condition, _if_true, _if_false) TERN(_condition, _if_true, _if_false)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("cond")
{
    REQUIRE_EQ(CMP_TERN((true), (1), (2)), 1);
    REQUIRE_EQ(CMP_TERN((false), (1), (2)), 2);
}

// elvis

#define ELVIS(_lhs, _rhs) IFFY(const auto ANON_NAME{_lhs}; return TERN((ANON_NAME), (ANON_NAME), (_rhs));)

ENV_TEST_CASE("elvis")
{
    struct
    {
        auto operator()()
        {
            auto static _test{1.0};
            return _test++;
        }
    } test{};

    REQUIRE_EQ(ELVIS(test(), 2), 1);
}

#if ENV_CPP >= 17
#define CMP_ELVIS(_lhs, _rhs) IFFY(constexpr auto ANON_NAME{_lhs}; return TERN((ANON_NAME), (ANON_NAME), (_rhs));)
#else
#define CMP_ELVIS(_lhs, _rhs) ELVIS(_lhs, _rhs)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("elvis")
{
    struct
    {
        constexpr auto operator()() const { return 1; }
    } static constexpr test{};
    REQUIRE_EQ(CMP_ELVIS(test(), 2), 1);
}

#endif // ENV_SCOPE_HPP
