#ifndef ENV_LIFE_HPP
#define ENV_LIFE_HPP


// conversion traits

COND_CHECK_BINARY(is_convertible, (ENV_STD::is_constructible_v < TRhs, TLhs >));

COND_CONCEPT_UNARY(convertible, (is_convertible_g<C, T>));

COND_CHECK_BINARY(is_imp_convertible, (ENV_STD::is_convertible_v < TLhs, TRhs >));

COND_CONCEPT_UNARY(imp_convertible, (is_imp_convertible_g<C, T>));

COND_CHECK_BINARY(is_exp_convertible, (is_convertible_g<TLhs, TRhs> && !is_imp_convertible_g<TLhs, TRhs>));

COND_CONCEPT_UNARY(exp_convertible, (is_exp_convertible_g<C, T>));

// Same as convertible - it's just more explicit about what it checks for.
EXPR_CHECK_BINARY(is_static_castable, (static_cast<TRhs>(declvalr<TLhs>())));

COND_CONCEPT_UNARY(static_castable, (is_static_castable_g<C, T>));

ENV_TEST_CASE("conversion traits")
{
    strct tmp_derived_t : public variadic_vt<int> { };
    strct tmp_explicit_t { con op variadic_vt<int>() { ret { }; } };

    REQUIRES(is_imp_convertible_g<tmp_derived_t, variadic_vt<int>>);
    REQUIRES(!is_imp_convertible_g<tmp_explicit_t, variadic_vt<int>>);

    REQUIRES(is_exp_convertible_g<tmp_explicit_t, variadic_vt<int>>);
    REQUIRES(!is_exp_convertible_g<tmp_derived_t, variadic_vt<int>>);

    REQUIRES(is_convertible_g<tmp_derived_t, variadic_vt<int>>);
    REQUIRES(is_convertible_g<tmp_explicit_t, variadic_vt<int>>);
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
        imp_convertible_tmp,
        (tmp<name...> class To),
        (is_imp_convertible_tmp_g<C, To>)
);

ENV_TEST_CASE("template conversion traits")
{
    strct tmp_derived_t : public variadic_vt<int> { };
    strct tmp_explicit_t { con op variadic_vt<int>() { ret { }; } };

    REQUIRES(is_imp_convertible_tmp_g < tmp_derived_t, variadic_vt >);
    REQUIRES(!is_imp_convertible_tmp_g < tmp_explicit_t, variadic_vt >);

    REQUIRE_EQT(tmp_ts<variadic_vt>::converted_gt < tmp_derived_t >, variadic_vt<int>);
}


// writeable/placeable

COND_CHECK_BINARY(is_placeable_on, ENV_STD::is_constructible_v < subject_gt < TRhs >, TLhs >);

COND_CONCEPT_UNARY(placeable_on, is_placeable_on_g<C, T>);

COND_CHECK_BINARY(is_writeable_to, ENV_STD::is_assignable_v < subject_gt < TRhs > &, TLhs >);

COND_CONCEPT_UNARY(writeable_to, is_writeable_to_g<C, T>);

ENV_TEST_CASE("writeable/placeable")
{
    REQUIRES(is_placeable_on_g < int, int[]>);
    REQUIRES(is_writeable_to_g < int, int*>);
    REQUIRES(is_placeable_on_g<const int, int[3]>);
    REQUIRES(is_writeable_to_g<const int, int*>);
    REQUIRES(is_placeable_on_g<int, const int[]>);
    REQUIRES(is_placeable_on_g < int, ENV_STD::unique_ptr<const int>>);
    REQUIRES(is_placeable_on_g < int, ENV_STD::unique_ptr < int >>);
    REQUIRES_FALSE(is_placeable_on_g < void(*)(), void(*)() >);
    REQUIRES_FALSE(is_placeable_on_g < int *, ENV_STD::unique_ptr < int >>);
    REQUIRES_FALSE(is_writeable_to_g<int, const int*>);
    REQUIRES_FALSE(is_writeable_to_g < int, ENV_STD::unique_ptr<const int>>);
    REQUIRES_FALSE(is_writeable_to_g<const int, const int*>);
    REQUIRES_FALSE(is_writeable_to_g<const int, ENV_STD::unique_ptr<const int>>);
}


// stable

COND_CHECK_UNARY
(
        is_stable,
        ENV_STD::is_default_constructible_v < T > &&
        ENV_STD::is_copy_constructible_v < T > &&
        ENV_STD::is_copy_assignable_v < T > &&
        ENV_STD::is_destructible_v < T > &&
        ENV_STD::is_swappable_v < T >
);

COND_CONCEPT_UNARY(stable, is_stable_g<T>);

ENV_TEST_CASE("stable")
{
    REQUIRES(is_stable_g<int>);
    REQUIRES(is_stable_g<int*>);
    REQUIRES(is_stable_g<void (*)()>);
    REQUIRES(is_stable_g<ENV_STD::vector < int>>);
    REQUIRES(is_stable_g<ENV_STD::string>);
    REQUIRES(is_stable_g<ENV_STD::function < void()>>);
    REQUIRES_FALSE(is_stable_g<int[3]>);
    REQUIRES_FALSE(is_stable_g<ENV_STD::unique_ptr < int>>);
    REQUIRES_FALSE(is_stable_g<int&>);
    REQUIRES_FALSE(is_stable_g<void()>);
}


#endif // ENV_LIFE_HPP
