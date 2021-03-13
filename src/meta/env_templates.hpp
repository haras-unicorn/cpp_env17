#ifndef ENV_TEMPLATES_HPP
#define ENV_TEMPLATES_HPP

// skips

#define SKIP_COND true
#define SKIP_EXPR ENV::success
#define SKIP_TYPE ENV::success_t

ENV_TEST_CASE("template skips")
{
    REQUIRES(SKIP_COND);
    REQUIRE_EQT(decl(SKIP_EXPR), const success_t);
}

// conversions

#define EXPR_TYPE(...) decl(__VA_ARGS__, ENV::success, ENV::ENV::declval<success_t>())
#define EXPR_COND(...) ENV::is_success_g<ENV::success_vt<decl(__VA_ARGS__)>>

#define TYPE_EXPR(...) ENV::ENV::declval<__VA_ARGS__>()
#define TYPE_COND(...) ENV::is_success_g<ENV::success_vt<__VA_ARGS__>>

#define COND_EXPR(...) ENV::ENV::declval<ENV::require_nt<__VA_ARGS__>>()
#define COND_TYPE(...) ENV::require_nt<__VA_ARGS__>

#define INSTANT(...) name ENV::variadic_vt<__VA_ARGS__, ENV::success_t>::last_t

ENV_TEST_CASE("expression conversion")
{
    strct
    {
        cmp_fn exp() const noex->first_gvt<bool, EXPR_TYPE(true)> { ret EXPR_COND(true); }

        cmp_fn type() const noex->first_gvt<bool, EXPR_TYPE(TYPE_EXPR(bool))> { ret TYPE_COND(bool); }

        cmp_fn flag() const noex->first_gvt<bool, COND_TYPE(true)> { ret EXPR_COND(COND_EXPR(true)); }
    }
    cmp test{};

    REQUIRES(test.exp() == true);
    REQUIRES(test.type() == true);
    REQUIRES(test.flag() == true);
    REQUIRE_EQT(INSTANT(int, float, void), success_t);
}

// cond

#define COND_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) COND_TYPE(__VA_ARGS__) = ENV::success>
#define COND_TMP(_tmp, ...) COND_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define COND_TMP_UNARY(...) COND_TMP((name T), __VA_ARGS__)
#define COND_TMP_BINARY(...) COND_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define COND_TMP_TERNARY(...) COND_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define COND_TMP_VARIADIC(...) COND_TMP((name... TVar), __VA_ARGS__)

ENV_NAMESPACE_TEST_BEGIN

COND_TMP_UNARY(ENV_STD::is_same_v<T, void>)
cmp_fn void_unary() noex { ret true; }

COND_TMP_UNARY(!ENV_STD::is_same_v<T, void>)
cmp_fn void_unary() noex { ret false; }

COND_TMP_BINARY(ENV_STD::is_same_v<TLhs, void> &&ENV_STD::is_same_v<TRhs, void>)
cmp_fn void_binary() noex { ret true; }

COND_TMP_BINARY(!ENV_STD::is_same_v<TLhs, void> || !ENV_STD::is_same_v<TRhs, void>)
cmp_fn void_binary() noex { ret false; }

COND_TMP_TERNARY(ENV_STD::is_same_v<T1, void> &&ENV_STD::is_same_v<T2, void> &&ENV_STD::is_same_v<T3, void>)
cmp_fn voidernary() noex { ret true; }

COND_TMP_TERNARY(!ENV_STD::is_same_v<T1, void> || !ENV_STD::is_same_v<T2, void> || !ENV_STD::is_same_v<T3, void>)
cmp_fn voidernary() noex { ret false; }

COND_TMP_VARIADIC(ENV_STD::conjunction_v<ENV_STD::is_same<TVar, void>...>)
cmp_fn void_variadic() noex { ret true; }

COND_TMP_VARIADIC(ENV_STD::disjunction_v<ENV_STD::negation<ENV_STD::is_same<TVar, void>>...>)
cmp_fn void_variadic() noex { ret false; }

ENV_NAMESPACE_TEST_END

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
        REQUIRES(test::voidernary<void, void, void>());
        REQUIRES_FALSE(test::voidernary<void, void, int>());
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

#define EXPR_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) EXPR_TYPE(__VA_ARGS__) = ENV::success>
#define EXPR_TMP(_tmp, ...) EXPR_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define EXPR_TMP_UNARY(...) EXPR_TMP((name T), __VA_ARGS__)
#define EXPR_TMP_BINARY(...) EXPR_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define EXPR_TMP_TERNARY(...) EXPR_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define EXPR_TMP_VARIADIC(...) EXPR_TMP((name... TVar), __VA_ARGS__)

ENV_NAMESPACE_TEST_BEGIN

EXPR_TMP_UNARY(ENV::declval<T>() + ENV::declval<T>())
cmp_fn test_addable_unary(int) noex { ret true; }

tmp<name> cmp_fn test_addable_unary(...) noex { ret false; }

EXPR_TMP_BINARY(ENV::declval<TLhs>() + ENV::declval<TRhs>())
cmp_fn test_addable_binary(int) noex { ret true; }

tmp<name, name> cmp_fn test_addable_binary(...) noex { ret false; }

EXPR_TMP_TERNARY(ENV::declval<T1>() + ENV::declval<T2>() + ENV::declval<T3>())
cmp_fn test_addable_ternary(int) noex { ret true; }

tmp<name, name, name> cmp_fn test_addable_ternary(...) noex { ret false; }

#if ENV_CPP >= 17 // fold expression

tmp<name... T> cmp_fn sum_res(T &&...args) noex->name variadic_vt<decl((args + ...)), void>::last_t;

EXPR_TMP_VARIADIC(sum_res(ENV::declval<TVar>()...))
cmp_fn test_addable_variadic(int) noex { ret true; }

tmp<name...> cmp_fn test_addable_variadic(...) noex { ret false; }

#endif // ENV_CPP >= 17

ENV_NAMESPACE_TEST_END

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

#define TYPE_TMP_OPT(_tmp, ...) tmp<SPREAD(_tmp) INSTANT(__VA_ARGS__) = ENV::success>
#define TYPE_TMP(_tmp, ...) TYPE_TMP_OPT((SPREAD(_tmp), ), __VA_ARGS__)
#define TYPE_TMP_UNARY(...) TYPE_TMP((name T), __VA_ARGS__)
#define TYPE_TMP_BINARY(...) TYPE_TMP((name TLhs, name TRhs), __VA_ARGS__)
#define TYPE_TMP_TERNARY(...) TYPE_TMP((name T1, name T2, name T3), __VA_ARGS__)
#define TYPE_TMP_VARIADIC(...) TYPE_TMP((name... TVar), __VA_ARGS__)

ENV_NAMESPACE_TEST_BEGIN

TYPE_TMP_UNARY(ENV_STD::minus<T>)
cmp_fn test_subtractable_unary(int) noex { ret true; }

tmp<name>
    cmp_fn test_subtractable_unary(...) noex { ret false; }

TYPE_TMP_BINARY(ENV_STD::minus<TLhs>, ENV_STD::minus<TRhs>)
cmp_fn test_subtractable_binary(int) noex { ret true; }

tmp<name, name>
    cmp_fn test_subtractable_binary(...) noex { ret false; }

TYPE_TMP_TERNARY(ENV_STD::minus<T1>, ENV_STD::minus<T2>, ENV_STD::minus<T3>)
cmp_fn test_subtractable_ternary(int) noex { ret true; }

tmp<name, name, name>
    cmp_fn test_subtractable_ternary(...) noex { ret false; }

TYPE_TMP_VARIADIC(ENV_STD::minus<TVar>...)
cmp_fn test_subtractable_variadic(int) noex { ret true; }

tmp<name...>
    cmp_fn test_subtractable_variadic(...) noex { ret false; }

ENV_NAMESPACE_TEST_END

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

    SUBCASE("variadic")
    {
        REQUIRES(test::test_addable_variadic<int, int, int>(0));
        REQUIRES(test::test_addable_variadic<int, int>(0));
        REQUIRES_FALSE(test::test_addable_variadic<int, nullptr_t>(0));
        REQUIRES_FALSE(test::test_addable_variadic<int, nullptr_t, double, nullptr_t>(0));
    }
}

#endif // ENV_TEMPLATES_HPP
