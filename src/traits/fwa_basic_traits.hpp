#ifndef FWA_CORE_BASIC_TRAITS_HPP
#define FWA_CORE_BASIC_TRAITS_HPP


// specialization traits

ELABORATE_EXPR_CHECK
(
        is_specialization,
        (name Type, tmp < name...> class Template),
        (Type, Template),
        (tmp<name...> class Template, name... Types),
        (Template<Types...>, Template),
        SKIP_EXPR
);

FWA_CORE_TEST_CASE("specialization traits")
{
    REQUIRES(is_specialization_g<variadic_vt<int>, variadic_vt>);
    REQUIRES_FALSE(is_specialization_g<int, variadic_vt>);
}


// completeness

// stolen from: https://stackoverflow.com/questions/21119281/using-sfinae-to-check-if-the-type-is-complete-or-not

EXPR_CHECK_UNARY(is_complete, sizeof(T));

FWA_CORE_TEST_CASE("completeness")
{
    REQUIRES(is_complete_g < int >);
    REQUIRES_FALSE(is_complete_g<void>);
}


// enum concept

COND_CONCEPT(enum, (FWA_STD::is_enum_v<C>));



// conversion traits

COND_CHECK_BINARY(is_convertible, (FWA_STD::is_constructible_v < TRhs, TLhs >));

COND_CONCEPT_UNARY(convertible, (is_convertible_g<C, T>));

COND_CHECK_BINARY(is_imp_convertible, (FWA_STD::is_convertible_v < TLhs, TRhs >));

COND_CONCEPT_UNARY(imp_convertible, (is_imp_convertible_g<C, T>));

COND_CHECK_BINARY(is_exp_convertible, (is_convertible_g<TLhs, TRhs> && !is_imp_convertible_g<TLhs, TRhs>));

COND_CONCEPT_UNARY(exp_convertible, (is_exp_convertible_g<C, T>));

// Same as convertible - it's just more explicit about what it checks for.
EXPR_CHECK_BINARY(is_static_castable, (static_cast<TRhs>(declvalr<TLhs>())));

COND_CONCEPT_UNARY(static_castable, (is_static_castable_g<C, T>));

FWA_CORE_TEST_CASE("conversion traits")
{
    strct tmp_derived_t : public variadic_vt<int> { };
    strct tmp_explicit_t { explicit op variadic_vt<int>() { } };

    REQUIRES(is_imp_convertible_g < tmp_derived_t, variadic_vt<int>>);
    REQUIRES(!is_imp_convertible_g < tmp_explicit_t, variadic_vt<int>>);

    REQUIRES(is_exp_convertible_g < tmp_explicit_t, variadic_vt<int>>);
    REQUIRES(!is_exp_convertible_g < tmp_derived_t, variadic_vt<int>>);

    REQUIRES(is_convertible_g < tmp_derived_t, variadic_vt<int>>);
    REQUIRES(is_convertible_g < tmp_explicit_t, variadic_vt<int>>);
}


// template conversion traits

// TODO: explicit conversion check if possible

tmp<tmp<name...> class Template>
strct tmp_ts
{
    tmp<name... Types>
    callb static consume(Template<Types...> arg) -> Template<Types...> { ret arg; }

    EXPR_TMP_UNARY((consume(declvalr<T>())))
    typ(converted_gt) = decltype(consume(declvalr<T>()));
};


EXPR_CHECK
(
        is_imp_convertible_tmp,
        (name From, tmp < name...> class To), (From, To),
        (tmp_ts<To>::consume(declvalr<From>()))
);

ELABORATE_COND_CONCEPT
(
        imp_convertible_to_tmp,
        (tmp<class...> class To),
        (is_imp_convertible_tmp_g<C, To>)
);

FWA_CORE_TEST_CASE("template conversion traits")
{
    strct tmp_derived_t : public variadic_vt<int> { };
    strct tmp_explicit_t { explicit op variadic_vt<int>() { } };

    REQUIRES(is_imp_convertible_tmp_g < tmp_derived_t, variadic_vt >);
    REQUIRES(!is_imp_convertible_tmp_g < tmp_explicit_t, variadic_vt >);

    REQUIRE_EQT(tmp_ts<variadic_vt>::converted_gt < tmp_derived_t >, variadic_vt<int>);
}


// behaviour traits

// TODO: fix formatting
// I think its because the rest of the lines are a continuation from decltype.
// I tried putting nullptr first but it didn't work.

EXPR_CHECK_UNARY
(
        is_equatable,
        (
                declvalr<T>() == declvalr<T>(),
                        declvalr<T>() != declvalr<T>()
        )
);

EXPR_CHECK_BINARY
(
        are_equatable,
        (
                declvalr<TLhs>() == declvalr<TRhs>(),
                        declvalr<TLhs>() != declvalr<TRhs>(),

                        declvalr<TRhs>() == declvalr<TLhs>(),
                        declvalr<TRhs>() != declvalr<TLhs>()
        )
);

COND_CONCEPT(equatable, (is_equatable_g<C>));


EXPR_CHECK_UNARY
(
        is_comparable,
        (
                declvalr<T>() < declvalr<T>(),
                        declvalr<T>() > declvalr<T>(),
                        declvalr<T>() <= declvalr<T>(),
                        declvalr<T>() >= declvalr<T>()
        )
);

EXPR_CHECK_BINARY
(
        are_comparable,
        (
                declvalr<TLhs>() < declvalr<TRhs>(),
                        declvalr<TLhs>() > declvalr<TRhs>(),
                        declvalr<TLhs>() <= declvalr<TRhs>(),
                        declvalr<TLhs>() >= declvalr<TRhs>(),

                        declvalr<TRhs>() < declvalr<TLhs>(),
                        declvalr<TRhs>() > declvalr<TLhs>(),
                        declvalr<TRhs>() <= declvalr<TLhs>(),
                        declvalr<TRhs>() >= declvalr<TLhs>()
        )
);

COND_CONCEPT(comparable, (is_comparable_g<C>));


COND_CHECK_UNARY(is_data, (FWA_STD::is_object_v < T > && !FWA_STD::is_pointer_v < T > ));

COND_CONCEPT(data, (is_data_g<C>));


FWA_CORE_TEST_CASE("behaviour traits")
{
    SUBCASE("equatable")
    {
        REQUIRES(is_equatable_g<int>);
        REQUIRES_FALSE(is_equatable_g<void>);

        REQUIRES(are_equatable_g<int, double>);
        REQUIRES_FALSE(are_equatable_g<void, int>);
    }

    SUBCASE("comparable")
    {
        REQUIRES(is_comparable_g<int>);
        REQUIRES_FALSE(is_comparable_g<void>);

        REQUIRES(are_comparable_g<int, double>);
        REQUIRES_FALSE(are_comparable_g<void, int>);
    }
}


// iterable traits

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name T> typ(iterator_element_gt) = name FWA_STD::iterator_traits<T>::value_type;

tmp<name T> typ(iterable_element_gt) = iterator_element_gt<decltype(declvalr<T>().begin())>;

FWA_NAMESPACE_DETAIL_END


EXPR_CHECK_UNARY(is_iterator, (TYPE_EXPR(detail::iterator_element_gt<T>)));

COND_CONCEPT(iterator, (is_iterator_g<C>));


COND_CHECK_BINARY(is_iterator_with_element, (FWA_STD::is_same_v < detail::iterator_element_gt<TLhs>, TRhs >));

COND_CONCEPT_UNARY(iterator_with_element, (is_iterator_with_element_g<C, T>));


EXPR_CHECK_UNARY(is_iterable, (TYPE_EXPR(detail::iterable_element_gt<T>)));

COND_CONCEPT(iterable, (is_iterable_g<C>));


COND_CHECK_BINARY(is_iterable_with_element, (FWA_STD::is_same_v < detail::iterable_element_gt<TLhs>, TRhs >));

COND_CONCEPT_UNARY(iterable_with_element, (is_iterable_with_element_g<C, T>));


tmp<name T> typ(iterator_element_gt) = detail::iterator_element_gt<iterator_c<T>>;

tmp<name T> typ(iterable_element_gt) = detail::iterable_element_gt<iterable_c<T>>;


FWA_CORE_TEST_CASE("iterable traits")
{
    SUBCASE("iterator")
    {
        REQUIRES(is_iterator_g<FWA_STD::vector<int>::iterator>);
        REQUIRE_EQT(iterator_element_gt<FWA_STD::vector<int>::iterator>, int);
        REQUIRES(is_iterator_with_element_g<FWA_STD::vector<int>::iterator, int>);
    }

    SUBCASE("iterable")
    {
        REQUIRES(is_iterable_g<FWA_STD::vector < int>>);
        REQUIRE_EQT(iterable_element_gt<FWA_STD::vector < int>>, int);
        REQUIRES(is_iterable_with_element_g<FWA_STD::vector < int>, int >);
    }
}


#endif // FWA_CORE_BASIC_TRAITS_HPP
