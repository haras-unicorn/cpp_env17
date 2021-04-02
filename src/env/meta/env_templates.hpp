#ifndef ENV_TEMPLATES_HPP
#define ENV_TEMPLATES_HPP


// skips

#define SKIP_COND true
#define SKIP_EXPR ENV::req
#define SKIP_TYPE ENV::req_t

ENV_TEST_CASE("template skips")
{
    REQUIRES(SKIP_COND);
    REQUIRE_EQT(decl(SKIP_EXPR), const succ_t);
}


// conversions

// Please, if you touch these macros, leave a commit message or
// check that all compilers are passing.
// A lot of stuff depends on these macros and compilers vary a lot on this.
// These current macros are passing on GCC, Clang and MSVC

#define EXPR_TYPE(...) ENV::req_vt<decl(nonce(__VA_ARGS__))>
#define EXPR_COND(...) ENV::true_vt<decl(nonce(__VA_ARGS__))>

#define TYPE_EXPR(...) (ENV::req_v<__VA_ARGS__>)
#define TYPE_COND(...) ENV::true_vt<__VA_ARGS__>

#define COND_EXPR(...) (ENV::require_vt<__VA_ARGS__>{})
#define COND_TYPE(...) ENV::require_vt<__VA_ARGS__>

#define INSTANT(...) ENV::req_vt<__VA_ARGS__>

ENV_TEST_CASE("expression conversion")
{
    strct
    {
        cmp_fn exp() const noex->first_gvt<bool, EXPR_TYPE(true)>
        {
            ret EXPR_COND(true)::value;
        }
        cmp_fn type() const noex->first_gvt<bool, EXPR_TYPE(TYPE_EXPR(bool))>
        {
            ret TYPE_COND(bool)::value;
        }
        cmp_fn flag() const noex->first_gvt<bool, COND_TYPE(ENV_STD::true_type)>
        {
            ret EXPR_COND(COND_EXPR(ENV_STD::true_type))::value;
        }
    }
    cmp test{};

    REQUIRES(test.exp() == true);
    REQUIRES(test.type() == true);
    REQUIRES(test.flag() == true);
    REQUIRE_EQT(INSTANT(int, float, void), req_t);
}


// cond

#define COND_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) COND_TYPE(__VA_ARGS__) = ENV::req>
#define COND_TMP(_tmp, ...)     COND_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define COND_TMP_UNARY(...)     COND_TMP((name T), __VA_ARGS__)
#define COND_TMP_BINARY(...)    COND_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define COND_TMP_TERNARY(...)   COND_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define COND_TMP_VARIADIC(...)  COND_TMP((name... TVar), __VA_ARGS__)

ENV_TEST_BEGIN

COND_TMP_UNARY(same_vt<void, T>)
cmp_fn void_unary() noex
{
    ret true;
}

COND_TMP_UNARY(neg_vt<same_vt<void, T>>)
cmp_fn void_unary() noex
{
    ret false;
}

COND_TMP_BINARY(same_vt<void, TLhs, TRhs>)
cmp_fn void_binary() noex
{
    ret true;
}

COND_TMP_BINARY(neg_vt<same_vt<void, TLhs, TRhs>>)
cmp_fn void_binary() noex
{
    ret false;
}

COND_TMP_TERNARY(same_vt<void, T1, T2, T3>)
cmp_fn void_ternary() noex
{
    ret true;
}

COND_TMP_TERNARY(neg_vt<same_vt<void, T1, T2, T3>>)
cmp_fn void_ternary() noex
{
    ret false;
}

COND_TMP_VARIADIC(same_vt<void, TVar...>)
cmp_fn void_variadic() noex
{
    ret true;
}

COND_TMP_VARIADIC(neg_vt<same_vt<void, TVar...>>)
cmp_fn void_variadic() noex
{
    ret false;
}

ENV_TEST_END

ENV_TEST_CASE("require template")
{
    SUBCASE("unary")
    {
        REQUIRES(test::void_unary<void>());
        REQUIRES_FALSE(test::void_unary<int>());
    }

    SUBCASE("binary")
    {
        REQUIRES(test::void_binary<void, void>());
        REQUIRES_FALSE(test::void_binary<int, void>());
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::void_ternary<void, void, void>());
        REQUIRES_FALSE(test::void_ternary<void, void, int>());
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::void_variadic<void, void, void, void>());
        REQUIRES(test::void_variadic<void, void, void, void, void>());
        REQUIRES_FALSE(test::void_variadic<int>());
        REQUIRES_FALSE(test::void_variadic<int, void, double, void>());
    }
}


// expr

#define EXPR_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) EXPR_TYPE(__VA_ARGS__) = ENV::req>
#define EXPR_TMP(_tmp, ...)     EXPR_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define EXPR_TMP_UNARY(...)     EXPR_TMP((name T), __VA_ARGS__)
#define EXPR_TMP_BINARY(...)    EXPR_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define EXPR_TMP_TERNARY(...)   EXPR_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define EXPR_TMP_VARIADIC(...)  EXPR_TMP((name... TVar), __VA_ARGS__)

ENV_TEST_BEGIN

EXPR_TMP_UNARY(declval<T>() + declval<T>())
cmp_fn test_addable_unary(int) noex
{
    ret true;
}

template<name>
nonced cmp_fn_p test_addable_unary(...) noex
{
    ret false;
}

EXPR_TMP_BINARY(declval<TLhs>() + declval<TRhs>())
cmp_fn test_addable_binary(int) noex
{
    ret true;
}

template<name, name>
nonced cmp_fn_p test_addable_binary(...) noex
{
    ret false;
}

EXPR_TMP_TERNARY(declval<T1>() + declval<T2>() + declval<T3>())
cmp_fn test_addable_ternary(int) noex
{
    ret true;
}

template<name, name, name>
nonced cmp_fn_p test_addable_ternary(...) noex
{
    ret false;
}

#if ENV_CPP >= 17 // fold expression

template<name... T>
cmp_fn sum_res(T&&... args) noex
        ->name variadic_vt<decl((args + ...)), void>::last_t;

EXPR_TMP_VARIADIC(sum_res(declval<TVar>()...))
cmp_fn test_addable_variadic(int) noex
{
    ret true;
}

template<name...>
cmp_fn test_addable_variadic(...) noex
{
    ret false;
}

#endif // ENV_CPP >= 17

ENV_TEST_END

ENV_TEST_CASE("expr template")
{
    SUBCASE("unary")
    {
        REQUIRES(test::test_addable_unary<int>(0));
        REQUIRES_FALSE(test::test_addable_unary<nullptr_t>(0));
    }

    SUBCASE("binary")
    {
        REQUIRES(test::test_addable_binary<int, int>(0));
        REQUIRES_FALSE(test::test_addable_binary<int, nullptr_t>(0));
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::test_addable_ternary<int, int, int>(0));
        REQUIRES_FALSE(test::test_addable_ternary<nullptr_t, nullptr_t, int>(0));
    }

#if ENV_CPP >= 17 // fold expression

    SUBCASE("variadic")
    {
        REQUIRES(test::test_addable_variadic<int, int, int>(0));
        REQUIRES(test::test_addable_variadic<int, int>(0));
        REQUIRES_FALSE(test::test_addable_variadic<int, nullptr_t>(0));
        REQUIRES_FALSE(test::test_addable_variadic<int, nullptr_t, double, nullptr_t>(0));
    }

#endif // ENV_CPP >= 17
}


// type

#define TYPE_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) INSTANT(__VA_ARGS__) = ENV::req>
#define TYPE_TMP(_tmp, ...)     TYPE_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define TYPE_TMP_UNARY(...)     TYPE_TMP((name T), __VA_ARGS__)
#define TYPE_TMP_BINARY(...)    TYPE_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define TYPE_TMP_TERNARY(...)   TYPE_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define TYPE_TMP_VARIADIC(...)  TYPE_TMP((name... TVar), __VA_ARGS__)

ENV_TEST_BEGIN

EXPR_TMP_UNARY(declval<T>() - declval<T>())
strct enable_minus_gt{};

TYPE_TMP_UNARY(enable_minus_gt<T>)
cmp_fn test_subtractable_unary(int) noex
{
    ret true;
}

template<name>
nonced cmp_fn_p test_subtractable_unary(...) noex
{
    ret false;
}

TYPE_TMP_BINARY(enable_minus_gt<TLhs>, enable_minus_gt<TRhs>)
cmp_fn test_subtractable_binary(int) noex
{
    ret true;
}

template<name, name>
nonced cmp_fn_p test_subtractable_binary(...) noex
{
    ret false;
}

TYPE_TMP_TERNARY(enable_minus_gt<T1>, enable_minus_gt<T2>, enable_minus_gt<T3>)
cmp_fn test_subtractable_ternary(int) noex
{
    ret true;
}

template<name, name, name>
nonced cmp_fn_p test_subtractable_ternary(...) noex
{
    ret false;
}

TYPE_TMP_VARIADIC(enable_minus_gt<TVar>...)
cmp_fn test_subtractable_variadic(int) noex
{
    ret true;
}

template<name...>
nonced cmp_fn_p test_subtractable_variadic(...) noex
{
    ret false;
}

ENV_TEST_END

ENV_TEST_CASE("type template")
{
    SUBCASE("unary")
    {
        REQUIRES(test::test_subtractable_unary<int>(0));
        REQUIRES_FALSE(test::test_subtractable_unary<nullptr_t>(0));
    }

    SUBCASE("binary")
    {
        REQUIRES(test::test_subtractable_binary<int, int>(0));
        REQUIRES_FALSE(test::test_subtractable_binary<int, nullptr_t>(0));
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::test_subtractable_ternary<int, int, int>(0));
        REQUIRES_FALSE(test::test_subtractable_ternary<nullptr_t, nullptr_t, int>(0));
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::test_subtractable_variadic<int, int, int>(0));
        REQUIRES(test::test_subtractable_variadic<int, int>(0));
        REQUIRES_FALSE(test::test_subtractable_variadic<int, nullptr_t>(0));
        REQUIRES_FALSE(test::test_subtractable_variadic<int, nullptr_t, double, nullptr_t>(0));
    }
}


#endif // ENV_TEMPLATES_HPP
