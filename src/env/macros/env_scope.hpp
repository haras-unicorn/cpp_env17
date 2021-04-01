#ifndef ENV_SCOPE_HPP
#define ENV_SCOPE_HPP


// TODO: turn into functions


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


// implicitly constexpr if it can be as of c++17
#define IFFY_IMPL(...) ([&]() noexcept { __VA_ARGS__ })()
#define IFFY(...)      IFFY_IMPL(__VA_ARGS__)

ENV_TEST_CASE("iffy")
{
    constexpr auto a = IFFY(return 1;);
    REQUIRE_EQ(a, 1);
}


// implicitly constexpr if it can be as of c++17
#define EXC_IFFY_IMPL(...) ([&]() { __VA_ARGS__ })()
#define EXC_IFFY(...)      EXC_IFFY_IMPL(__VA_ARGS__)

ENV_CLANG_SUPPRESS_PUSH("UnreachableCode");

ENV_TEST_CASE("iffy")
{
    try
    {
        EXC_IFFY(throw ENV_STD::logic_error{"Exception IFFY test failed."};);
    }
    catch (...)
    {
        REQUIRE(true);
    }
}

ENV_CLANG_SUPPRESS_POP;


// if

#define IF(_condition, _if_true, _if_false) \
    SCOPE(if _condition { SPREAD(_if_true) } else {SPREAD(_if_false)})

ENV_TEST_CASE("if scope")
{
    IF((true),
       (REQUIRE(true);),
       (REQUIRE(false);));

    IF((false),
       (REQUIRE(false);),
       (REQUIRE(true);));
}

#if ENV_CPP >= 17
    #define CMP_IF(_condition, _if_true, _if_false) \
        IF(constexpr(_condition), _if_true, _if_false)
#else // CPP >= 17
    #define CMP_IF(_condition, _if_true, _if_false) \
        IF(_condition, _if_true, _if_false)
#endif // CPP >= 17

ENV_TEST_CASE("constexpr if")
{
    CMP_IF((true),
           (REQUIRE(true);),
           (REQUIRE(false);));

    CMP_IF((false),
           (REQUIRE(false);),
           (REQUIRE(true);));
}


// when

#define ON(_condition, ...) SCOPE(if _condition{__VA_ARGS__})

ENV_TEST_CASE("on")
{
    ON((true), REQUIRE(true););
    ON((false), REQUIRE(false););
}

#if ENV_CPP >= 17
    #define CMP_ON(_condition, ...) ON(constexpr _condition, __VA_ARGS__)
#else // ENV_CPP >= 17
    #define CMP_ON(_condition, ...) ON(_condition, __VA_ARGS__)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("cmp on")
{
    CMP_ON((true), REQUIRE(true););
    CMP_ON((false), REQUIRE(false););
}


// tern

#define EXC_TERN(_condition, _if_true, _if_false) \
    (_condition ? SPREAD(_if_true) : SPREAD(_if_false))
#define TERN(_condition, _if_true, _if_false) \
    (_condition ? SPREAD(_if_true) : SPREAD(_if_false))

ENV_TEST_CASE("tern")
{
    REQUIRE_EQ(TERN((true), (1), (2)), 1);
    REQUIRE_EQ(EXC_TERN((true), (1), (2)), 1);
    REQUIRE_EQ(TERN((false), (1), (2)), 2);
    REQUIRE_EQ(EXC_TERN((false), (1), (2)), 2);
}

#if ENV_CPP >= 17
    #define EXC_CMP_TERN(_condition, _if_true, _if_false) \
        EXC_IFFY(CMP_IF(_condition, (return _if_true;), (return _if_false;));)
    #define CMP_TERN(_condition, _if_true, _if_false) \
        IFFY(CMP_IF(_condition, (return _if_true;), (return _if_false;));)
#else // ENV_CPP >= 17
    #define EXC_CMP_TERN(_condition, _if_true, _if_false) \
        EXC_TERN(_condition, _if_true, _if_false)
    #define CMP_TERN(_condition, _if_true, _if_false) \
        TERN(_condition, _if_true, _if_false)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("cmp tern")
{
    REQUIRE_EQ(CMP_TERN((true), (1), (2)), 1);
    REQUIRE_EQ(EXC_CMP_TERN((true), (1), (2)), 1);
    REQUIRE_EQ(CMP_TERN((false), (1), (2)), 2);
    REQUIRE_EQ(EXC_CMP_TERN((false), (1), (2)), 2);
}


// elvis

#define EXC_ELVIS(_lhs, _rhs)         \
    EXC_IFFY(const auto _elvis{_lhs}; \
             return EXC_TERN((_elvis), (_elvis), (_rhs));)
#define ELVIS(_lhs, _rhs)         \
    IFFY(const auto _elvis{_lhs}; \
         return TERN((_elvis), (_elvis), (_rhs));)

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
    #define EXC_CMP_ELVIS(_lhs, _rhs)         \
        EXC_IFFY(constexpr auto _elvis{_lhs}; \
                 return EXC_TERN((_elvis), (_elvis), (_rhs));)
    #define CMP_ELVIS(_lhs, _rhs)         \
        IFFY(constexpr auto _elvis{_lhs}; \
             return TERN((_elvis), (_elvis), (_rhs));)
#else
    #define EXC_CMP_ELVIS(_lhs, _rhs) EXC_ELVIS(_lhs, _rhs)
    #define CMP_ELVIS(_lhs, _rhs)     ELVIS(_lhs, _rhs)
#endif // ENV_CPP >= 17

ENV_TEST_CASE("cmp elvis")
{
    struct
    {
        constexpr auto operator()() const
        {
            return 1;
        }
    } static constexpr test{};
    REQUIRE_EQ(CMP_ELVIS(test(), 2), 1);
    REQUIRE_EQ(EXC_CMP_ELVIS(test(), 2), 1);
}


#endif // ENV_SCOPE_HPP
