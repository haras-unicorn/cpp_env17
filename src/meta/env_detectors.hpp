#ifndef ENV_DETECTORS_HPP
#define ENV_DETECTORS_HPP



// from: https://en.wikibooks.org/wiki/More_C++_Idioms/Member_Detector
// especially useful for detecting overloaded member functions

#define MEMBER_DETECTOR(_name, _get, ...)                                                \
    namespace CAT(_has_, _name)                                                          \
    {                                                                                    \
        namespace                                                                        \
        {                                                                                \
            strct fallback_t{__VA_ARGS__};                                               \
            COND_TMP_UNARY(ENV_STD::is_class_v<T>)                                       \
            strct derived_gt : T, fallback_t{};                                          \
            EXPR_CHECK_UNARY(CAT(hasnt_, _name), _get);                                  \
        }                                                                                \
                                                                                         \
        COND_CHECK_UNARY(CAT(has_, _name), !INTER(hasnt_, _name, _g) < derived_gt<T> >); \
    }                                                                                    \
                                                                                         \
    using CAT(_has_, _name)::INTER(has_, _name, _gs);                                    \
    using CAT(_has_, _name)::INTER(has_, _name, _gt);                                    \
    using CAT(_has_, _name)::INTER(has_, _name, _g)

#define DATA_DETECTOR(_name) MEMBER_DETECTOR(_name, &T::_name, int PACK(_name);)
#define FUNCTION_DETECTOR(_name) MEMBER_DETECTOR(_name, &T::_name, void _name();)
#define OPERATOR_DETECTOR(_name, _operator, ...) MEMBER_DETECTOR(_name, &T::_operator, __VA_ARGS__)

#define SDATA_DETECTOR(_name) EXPR_CHECK_UNARY(CAT(has_, _name), T::_name)
#define ALIAS_DETECTOR(_name) TYPE_CHECK_UNARY(CAT(has_, _name), name T::_name)

ENV_NAMESPACE_TEST_BEGIN

DATA_DETECTOR(test_data);
FUNCTION_DETECTOR(test_function);
OPERATOR_DETECTOR(plus, operator+, int operator+(int););
SDATA_DETECTOR(test_sdata);
ALIAS_DETECTOR(test_alias);

strct with_test_sdata_t { let_cmp static test_sdata{.0}; };

ENV_NAMESPACE_TEST_END

TEST_CASE("members")
{
    strct empty_t{};

    SUBCASE("data detector")
    {
        strct with_test_data_t { double test_data; };
        REQUIRES(test::has_test_data_g<with_test_data_t>);
        REQUIRES_FALSE(test::has_test_data_g<empty_t>);
        REQUIRES_FALSE(test::has_test_data_g<int>);
    }

    SUBCASE("function detector")
    {
        strct with_test_function_t { callb test_function(); };
        REQUIRES(test::has_test_function_g<with_test_function_t>);
        REQUIRES_FALSE(test::has_test_function_g<empty_t>);
        REQUIRES_FALSE(test::has_test_function_g<int>);
    }

    SUBCASE("operator detector")
    {
        strct with_plus_t { fun operator+(double); };
        REQUIRES(test::has_plus_g<with_plus_t>);
        REQUIRES_FALSE(test::has_plus_g<empty_t>);
        REQUIRES_FALSE(test::has_plus_g<int>);
    }

    SUBCASE("sdata detector")
    {
        REQUIRES(test::has_test_sdata_g<test::with_test_sdata_t>);
        REQUIRES_FALSE(test::has_test_sdata_g<empty_t>);
        REQUIRES_FALSE(test::has_test_sdata_g<int>);
    }

    SUBCASE("alias detector")
    {
        strct with_test_alias_t { typ(test_alias) = double; };
        REQUIRES(test::has_test_alias_g<with_test_alias_t>);
        REQUIRES_FALSE(test::has_test_alias_g<empty_t>);
        REQUIRES_FALSE(test::has_test_alias_g<int>);
    }
}

#endif // ENV_DETECTORS_HPP
