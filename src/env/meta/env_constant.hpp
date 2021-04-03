#ifndef ENV_CONSTANT_HPP
#define ENV_CONSTANT_HPP


// tag

template<name T>
strct tag_gt
{
    typ(tag) = ENV_HANA::tag_of_t<T>;

    typ(type) = T;
};

ENV_TEST_CASE("tag constant")
{
}


// kind

template<template<name...> class TKind>
strct kind_kt
{
    template<name TArgs>
    typ(kind) = TKind<TArgs>;
};

template<name TKind>
strct kind_gt
{
    template<name TArgs>
    typ(kind) = name TKind::tmp kind<TArgs>;
};

ENV_TEST_CASE("kind constant")
{
    REQUIRE_EQT(kind_kt<ENV_STD::void_t>::kind<int, float>, void);
    REQUIRE_EQT(kind_kt<ENV_STD::is_const>::kind<const int>::type,
                ENV_STD::true_type);

    REQUIRE_EQT(kind_gt<kind_kt<ENV_STD::is_const>>::kind<const int>::type,
                ENV_STD::true_type);
}


// type

template<name TType>
strct type_gt
{
    template<name... TArgs>
    typ(kind) = TType;

    typ(type) = TType;
};

ENV_TEST_CASE("type constant")
{
    REQUIRE_EQT(type_gt<int>::type, int);
    REQUIRE_EQT(type_gt<double>::type, double);
    REQUIRE_EQT(type_gt<void>::type, void);
}


// value

template<name TType, TType Value>
strct value_gnt : public ENV_STD::integral_constant<TType, Value>
{
    template<name... TArgs>
    typ(kind) = TType;
};

template<name TType>
typ(value_gt) = value_gnt<name TType::value_type, TType::value>;

#define val(_value)                                                 \
    ENV::value_gnt<                                                 \
            ENV_STD::remove_cv_t<                                   \
                    ENV_STD::remove_reference_t<decltype(_value)>>, \
            _value>

#define val_con(_value) \
    val(_value) { }


#if ENV_CPP >= 17

template<deduc Value>
strct value_nt : public value_gnt<decltype(Value), Value>{};

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
