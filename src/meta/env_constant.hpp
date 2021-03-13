#ifndef ENV_CONSTANT_HPP
#define ENV_CONSTANT_HPP

// type

tmp<name TType>
strct type_gt { typ(type) = TType; };

ENV_TEST_CASE("type constant")
{
    REQUIRE_EQT(type_gt<int>::type, int);
    REQUIRE_EQT(type_gt<double>::type, double);
    REQUIRE_EQT(type_gt<void>::type, void);
}

// value

tmp<name TType, TType Value>
strct value_gnt : public ENV_STD::integral_constant<TType, Value> { };

#define val(_value) ENV::value_gnt<ENV_STD::remove_cv_t<ENV_STD::remove_reference_t<decltype(_value)>>, _value>
#define val_con(_value) val(_value) {}

#if ENV_CPP >= 17

tmp<deduc Value>
strct value_nt : public value_gnt<decltype(Value), Value> { };

#endif // ENV_CPP >= 17

ENV_TEST_CASE("value constant")
{
    SUBCASE("declare")
    {
        REQUIRE_EQT(val(1), value_gnt<int, 1>);
        REQUIRE_EQT(val(false), value_gnt<bool, false>);

#if ENV_CPP >= 17
        REQUIRES(value_nt<1>::value == 1);
        REQUIRES(value_nt<false>::value == false);
#endif // ENV_CPP >= 17
    }

    SUBCASE("construct")
    {
        REQUIRE_EQT(decltype(val_con(1)), value_gnt<int, 1>);
        REQUIRE_EQT(decltype(val_con(true)), value_gnt<bool, true>);
    }
}

#endif // ENV_CONSTANT_HPP
