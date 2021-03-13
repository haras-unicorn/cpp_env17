#ifndef FWA_CORE_CHECK_HPP
#define FWA_CORE_CHECK_HPP


// expr

#define EXPR_CHECK_OPT(_name, _tmp, _application, _extract, _extract_application, _type, _static) \
            tmp<SPREAD(_tmp), name = FWA_CORE::success_t> \
            strct CAT(_name, _gs) : public FWA_STD::false_type { }; \
             \
            tmp<SPREAD(_extract)> \
            strct CAT(_name, _gs)<SPREAD(_extract_application), SPREAD(_type)> : public FWA_STD::true_type { }; \
             \
            tmp<SPREAD(_tmp)> \
            typ(CAT(_name, _gt)) = name CAT(_name, _gs)<SPREAD(_application)>::type; \
             \
            tmp<SPREAD(_tmp)> \
            cmp_obj bool _static CAT(_name, _g){CAT(_name, _gs)<SPREAD(_application)>::value}

#define ELABORATE_EXPR_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            EXPR_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (EXPR_TYPE(__VA_ARGS__)), \
                SKIP)

#define EXPR_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_EXPR_CHECK(_name, _tmp, _application, _tmp, _application, __VA_ARGS__)

#define EXPR_CHECK_UNARY(_name, ...) \
            EXPR_CHECK(_name, (name T), (T), __VA_ARGS__)

#define EXPR_CHECK_BINARY(_name, ...) \
            EXPR_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define EXPR_CHECK_TERNARY(_name, ...) \
            EXPR_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define EXPR_CHECK_VARIADIC(_name, ...) \
            ELABORATE_EXPR_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

EXPR_CHECK_UNARY(test_addable_unary, declvalr<T>() + declvalr<T>());

EXPR_CHECK_BINARY(test_addable_binary, declvalr<TLhs>() + declvalr<TRhs>());

EXPR_CHECK_TERNARY(test_addable_ternary, declvalr<T1>() + declvalr<T2>() + declvalr<T3>());

#if FWA_CPP >= 17 // fold expression

EXPR_CHECK_VARIADIC(test_addable_variadic, sum_res(declvalr<TVar>()...));

#endif // FWA_CPP >= 17

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("sfinae struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::test_addable_unary_g<int>);
        REQUIRES_FALSE(test::test_addable_unary_g<void>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::test_addable_binary_g<int, int>);
        REQUIRES_FALSE(test::test_addable_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::test_addable_ternary_g<int, int, int>);
        REQUIRES_FALSE(test::test_addable_ternary_g<int, void, int>);
    }

#if FWA_CPP >= 17 // fold expression
    SUBCASE("variadic")
    {
        REQUIRES(test::test_addable_variadic_g<variadic_vt < int, int, int, int>>);
        REQUIRES_FALSE(test::test_addable_variadic_g<variadic_vt < int, void, int>>);
    }
#endif // FWA_CPP >= 17
}


// expr class

#define ELABORATE_EXPR_CLASS_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            EXPR_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (EXPR_TYPE(__VA_ARGS__)), \
                static)

#define EXPR_CLASS_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_EXPR_CLASS_CHECK( \
                _name, \
                _tmp, _application, \
                _tmp, _application, \
                __VA_ARGS__)


#define EXPR_CLASS_CHECK_UNARY(_name, ...) \
            EXPR_CLASS_CHECK(_name, (name T), (T), __VA_ARGS__)

#define EXPR_CLASS_CHECK_BINARY(_name, ...) \
            EXPR_CLASS_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define EXPR_CLASS_CHECK_TERNARY(_name, ...) \
            EXPR_CLASS_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define EXPR_CLASS_CHECK_VARIADIC(_name, ...) \
            ELABORATE_EXPR_CLASS_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

cls expr_check_class_t
{
public:
    EXPR_CLASS_CHECK_UNARY(test_addable_unary, declvalr<T>() + declvalr<T>());

    EXPR_CLASS_CHECK_BINARY(test_addable_binary, declvalr<TLhs>() + declvalr<TRhs>());

    EXPR_CLASS_CHECK_TERNARY(test_addable_ternary, declvalr<T1>() + declvalr<T2>() + declvalr<T3>());

#if FWA_CPP >= 17 // fold expression

    EXPR_CLASS_CHECK_VARIADIC(test_addable_variadic, sum_res(declvalr<TVar>()...));

#endif // FWA_CPP >= 17
};

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("sfinae in class struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::expr_check_class_t::test_addable_unary_g<int>);
        REQUIRES_FALSE(test::expr_check_class_t::test_addable_unary_g<void>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::expr_check_class_t::test_addable_binary_g<int, int>);
        REQUIRES_FALSE(test::expr_check_class_t::test_addable_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::expr_check_class_t::test_addable_ternary_g<int, int, int>);
        REQUIRES_FALSE(test::expr_check_class_t::test_addable_ternary_g<int, void, int>);
    }

#if FWA_CPP >= 17 // fold expression

    SUBCASE("variadic")
    {
        REQUIRES(test::expr_check_class_t::test_addable_variadic_g<variadic_vt < int, int, int, int>>);
        REQUIRES_FALSE(test::expr_check_class_t::test_addable_variadic_g<variadic_vt < int, void, int>>);
    }

#endif // FWA_CPP >= 17
}


// cond

#define COND_CHECK_OPT(_name, _tmp, _application, _extract, _extract_application, _condition, _static) \
            EXPR_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (COND_TYPE(_condition)), \
                _static)


#define ELABORATE_COND_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            COND_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (__VA_ARGS__), \
                SKIP)

#define COND_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_COND_CHECK( \
                _name, \
                _tmp, _application, \
                _tmp, _application, \
                __VA_ARGS__)

#define COND_CHECK_UNARY(_name, ...) \
            COND_CHECK(_name, (name T), (T), __VA_ARGS__)

#define COND_CHECK_BINARY(_name, ...) \
            COND_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define COND_CHECK_TERNARY(_name, ...) \
            COND_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define COND_CHECK_VARIADIC(_name, ...) \
            ELABORATE_COND_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

COND_CHECK_UNARY
(test_const_unary, FWA_STD::is_const_v < T >);

COND_CHECK_BINARY
(test_const_binary, FWA_STD::is_const_v < TLhs > && FWA_STD::is_const_v < TRhs >);

COND_CHECK_TERNARY
(test_const_ternary, FWA_STD::is_const_v < T1 > && FWA_STD::is_const_v < T2 > && FWA_STD::is_const_v < T3 >);

COND_CHECK_VARIADIC
(test_const_variadic, FWA_STD::conjunction_v < FWA_STD::is_const < TVar >...>);

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("require struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::test_const_unary_g<const int>);
        REQUIRES_FALSE(test::test_const_unary_g<int>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::test_const_binary_g<const int, const int>);
        REQUIRES_FALSE(test::test_const_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::test_const_ternary_g<const int, const int, const int>);
        REQUIRES_FALSE(test::test_const_ternary_g<int, void, const int>);
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::test_const_variadic_g<variadic_vt<const int, const int, const int, const int>>);
        REQUIRES_FALSE(test::test_const_variadic_g<variadic_vt<int, void, const int>>);
    }
}


// cond class

#define ELABORATE_COND_CLASS_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            COND_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (__VA_ARGS__), \
                static)

#define COND_CLASS_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_COND_CLASS_CHECK( \
                _name, \
                _tmp, _application, \
                _tmp, _application, \
                __VA_ARGS__)

#define COND_CLASS_CHECK_UNARY(_name, ...) \
            COND_CLASS_CHECK(_name, (name T), (T), __VA_ARGS__)

#define COND_CLASS_CHECK_BINARY(_name, ...) \
            COND_CLASS_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define COND_CLASS_CHECK_TERNARY(_name, ...) \
            COND_CLASS_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define COND_CLASS_CHECK_VARIADIC(_name, ...) \
            ELABORATE_COND_CLASS_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

cls cond_check_class_t
{
public:
    COND_CLASS_CHECK_UNARY
    (test_const_unary, FWA_STD::is_const_v < T >);

    COND_CLASS_CHECK_BINARY
    (test_const_binary, FWA_STD::is_const_v < TLhs > && FWA_STD::is_const_v < TRhs >);

    COND_CLASS_CHECK_TERNARY
    (test_const_ternary, FWA_STD::is_const_v < T1 > && FWA_STD::is_const_v < T2 > && FWA_STD::is_const_v < T3 >);

    COND_CLASS_CHECK_VARIADIC
    (test_const_variadic, FWA_STD::conjunction_v < FWA_STD::is_const < TVar >...>);
};

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("require in class struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::cond_check_class_t::test_const_unary_g<const int>);
        REQUIRES_FALSE(test::cond_check_class_t::test_const_unary_g<int>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::cond_check_class_t::test_const_binary_g<const int, const int>);
        REQUIRES_FALSE(test::cond_check_class_t::test_const_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::cond_check_class_t::test_const_ternary_g<const int, const int, const int>);
        REQUIRES_FALSE(test::cond_check_class_t::test_const_ternary_g<int, void, const int>);
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::cond_check_class_t::test_const_variadic_g<variadic_vt<const int, const int>>);
        REQUIRES_FALSE(test::cond_check_class_t::test_const_variadic_g<variadic_vt<int, void, const int>>);
    }
}


// type

#define TYPE_CHECK_OPT(_name, _tmp, _application, _extract, _extract_application, _type, _static) \
            EXPR_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (INSTANT(SPREAD(_type))), \
                _static)


#define ELABORATE_TYPE_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            TYPE_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (__VA_ARGS__), \
                SKIP)

#define TYPE_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_TYPE_CHECK( \
                _name, \
                _tmp, _application, \
                _tmp, _application, \
                __VA_ARGS__)

#define TYPE_CHECK_UNARY(_name, ...) \
            TYPE_CHECK(_name, (name T), (T), __VA_ARGS__)

#define TYPE_CHECK_BINARY(_name, ...) \
            TYPE_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define TYPE_CHECK_TERNARY(_name, ...) \
            TYPE_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define TYPE_CHECK_VARIADIC(_name, ...) \
            ELABORATE_TYPE_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

EXPR_TMP_UNARY(declval<T>() - declval<T>())
strct enable_minus_gt { };

TYPE_CHECK_UNARY
(test_minus_unary, enable_minus_gt<T>);

TYPE_CHECK_BINARY
(test_minus_binary, enable_minus_gt<TLhs>, enable_minus_gt<TRhs>);

TYPE_CHECK_TERNARY
(test_minus_ternary, enable_minus_gt<T1>, enable_minus_gt<T2>, enable_minus_gt<T3>);

TYPE_CHECK_VARIADIC
(test_minus_variadic, enable_minus_gt<TVar>...);

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("require struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::test_minus_unary_g<const int>);
        REQUIRES_FALSE(test::test_minus_unary_g<void>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::test_minus_binary_g<const int, const int>);
        REQUIRES_FALSE(test::test_minus_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::test_minus_ternary_g<const int, const int, const int>);
        REQUIRES_FALSE(test::test_minus_ternary_g<int, void, const int>);
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::test_minus_variadic_g<variadic_vt<const int, const int, const int, const int>>);
        REQUIRES_FALSE(test::test_minus_variadic_g<variadic_vt<int, void, const int>>);
    }
}


// type class

#define ELABORATE_TYPE_CLASS_CHECK(_name, _tmp, _application, _extract, _extract_application, ...) \
            TYPE_CHECK_OPT( \
                _name,  \
                _tmp, _application, \
                _extract, _extract_application, \
                (__VA_ARGS__), \
                static)

#define TYPE_CLASS_CHECK(_name, _tmp, _application, ...) \
            ELABORATE_TYPE_CLASS_CHECK( \
                _name, \
                _tmp, _application, \
                _tmp, _application, \
                __VA_ARGS__)

#define TYPE_CLASS_CHECK_UNARY(_name, ...) \
            TYPE_CLASS_CHECK(_name, (name T), (T), __VA_ARGS__)

#define TYPE_CLASS_CHECK_BINARY(_name, ...) \
            TYPE_CLASS_CHECK(_name, (name TLhs, name TRhs), (TLhs, TRhs), __VA_ARGS__)

#define TYPE_CLASS_CHECK_TERNARY(_name, ...) \
            TYPE_CLASS_CHECK(_name, (name T1, name T2, name T3), (T1, T2, T3), __VA_ARGS__)

#define TYPE_CLASS_CHECK_VARIADIC(_name, ...) \
            ELABORATE_TYPE_CLASS_CHECK( \
                _name, \
                (name T), (T), \
                (name... TVar), (FWA_CORE::variadic_vt<TVar...>), \
                __VA_ARGS__)


FWA_NAMESPACE_TEST_BEGIN

cls type_check_class_t
{
public:
    TYPE_CLASS_CHECK_UNARY
    (test_minus_unary, enable_minus_gt<T>);

    TYPE_CLASS_CHECK_BINARY
    (test_minus_binary, enable_minus_gt<TLhs>, enable_minus_gt<TRhs>);

    TYPE_CLASS_CHECK_TERNARY
    (test_minus_ternary, enable_minus_gt<T1>, enable_minus_gt<T2>, enable_minus_gt<T3>);

    TYPE_CLASS_CHECK_VARIADIC
    (test_minus_variadic, enable_minus_gt<TVar>...);
};

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("require in class struct")
{
    SUBCASE("unary")
    {
        REQUIRES(test::type_check_class_t::test_minus_unary_g<const int>);
        REQUIRES_FALSE(test::type_check_class_t::test_minus_unary_g<void>);
    }

    SUBCASE("binary")
    {
        REQUIRES(test::type_check_class_t::test_minus_binary_g<const int, const int>);
        REQUIRES_FALSE(test::type_check_class_t::test_minus_binary_g<int, void>);
    }

    SUBCASE("ternary")
    {
        REQUIRES(test::type_check_class_t::test_minus_ternary_g<const int, const int, const int>);
        REQUIRES_FALSE(test::type_check_class_t::test_minus_ternary_g<int, void, const int>);
    }

    SUBCASE("variadic")
    {
        REQUIRES(test::type_check_class_t::test_minus_variadic_g<variadic_vt<const int, const int>>);
        REQUIRES_FALSE(test::type_check_class_t::test_minus_variadic_g<variadic_vt<int, void, const int>>);
    }
}


// extract

#define EXTRACT_CHECK_OPT(_name, _extract, _application, _static) \
            EXPR_CHECK_OPT( \
                _name,  \
                (name T), (T), \
                _extract, _application, \
                (FWA_CORE::success_t), \
                _static)

#define EXTRACT_CHECK(_name, _extract, _application) EXTRACT_CHECK_OPT(_name, _extract, _application, SKIP)
#define SIMPLE_EXTRACT_CHECK(_name, ...) EXTRACT_CHECK(_name, (), (__VA_ARGS__))

FWA_NAMESPACE_TEST_BEGIN

tmp<name T>
strct tmp_struct { };

EXTRACT_CHECK(is_tmp_struct, (name T), (tmp_struct<T>));

SIMPLE_EXTRACT_CHECK(is_int, int);

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("struct check")
{
    REQUIRES(test::is_tmp_struct_g<test::tmp_struct<int >>);
    REQUIRES_FALSE(test::is_tmp_struct_g<int>);

    SUBCASE("simple")
    {
        REQUIRES(test::is_int_g<int>);
        REQUIRES_FALSE(test::is_int_g<void>);
    }
}


// extract class

#define EXTRACT_CLASS_CHECK(_name, _extract, _application) EXTRACT_CHECK_OPT(_name, _extract, _application, static)
#define SIMPLE_EXTRACT_CLASS_CHECK(_name, ...) EXTRACT_CLASS_CHECK(_name, (), (__VA_ARGS__))

FWA_NAMESPACE_TEST_BEGIN

cls struct_check_class
{
public:
    EXTRACT_CLASS_CHECK(is_tmp_struct, (name T), (tmp_struct<T>));
    SIMPLE_EXTRACT_CLASS_CHECK(is_int, int);
};

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("in class struct check")
{
    REQUIRES(test::struct_check_class::is_tmp_struct_g<test::tmp_struct<int >>);
    REQUIRES_FALSE(test::struct_check_class::is_tmp_struct_g<int>);

    SUBCASE("simple")
    {
        REQUIRES(test::struct_check_class::is_int_g<int>);
        REQUIRES_FALSE(test::struct_check_class::is_int_g<void>);
    }
}


#endif // FWA_CORE_CHECK_HPP
