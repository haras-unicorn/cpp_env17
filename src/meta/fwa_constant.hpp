#ifndef FWA_CORE_CONSTANT_HPP
#define FWA_CORE_CONSTANT_HPP


// type

tmp<name TType>
strct type_gt { typ(type) = TType; };

FWA_CORE_TEST_CASE("type constant")
{
    REQUIRE_EQT(type_gt<int>::type, int);
    REQUIRE_EQT(type_gt<double>::type, double);
    REQUIRE_EQT(type_gt<void>::type, void);
}


// value

tmp<name TType, TType Value>
strct value_gnt : public FWA_STD::integral_constant<TType, Value> { };

#define val(_value) FWA_CORE::value_gnt<FWA_STD::remove_cv_t<FWA_STD::remove_reference_t<decltype(_value)>>, _value>
#define val_con(_value) val(_value){}

#if FWA_CPP >= 17

tmp<deduc Value>
strct value_nt : public value_gnt<decltype(Value), Value> { };

#endif // FWA_CPP >= 17


FWA_CORE_TEST_CASE("value constant")
{
    SUBCASE("declare")
    {
        REQUIRE_EQT(val(1), value_gnt<int, 1>);
        REQUIRE_EQT(val(false), value_gnt<bool, false>);

#if FWA_CPP >= 17
        REQUIRES(value_nt<1>::value == 1);
        REQUIRES(value_nt<false>::value == false);
#endif // FWA_CPP >= 17
    }

    SUBCASE("construct")
    {
        REQUIRE_EQT(decltype(val_con(1)), value_gnt<int, 1>);
        REQUIRE_EQT(decltype(val_con(true)), value_gnt<bool, true>);
    }
}


#endif // FWA_CORE_CONSTANT_HPP
