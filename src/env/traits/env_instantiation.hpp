#ifndef ENV_SPECIALIZATION_HPP
#define ENV_SPECIALIZATION_HPP


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

ELABORATE_COND_CONCEPT
(
        specialization,
        (tmp<name...> class Of),
        (is_specialization_g<C, Of>)
);

ENV_TEST_CASE("specialization traits")
{
    REQUIRES(is_specialization_g<variadic_vt<int>, variadic_vt>);
    REQUIRES_FALSE(is_specialization_g<int, variadic_vt>);
}


// completeness

// stolen from: https://stackoverflow.com/questions/21119281/using-sfinae-to-check-if-the-type-is-complete-or-not

EXPR_CHECK_UNARY(is_complete, sizeof(T));

ENV_TEST_CASE("completeness")
{
    REQUIRES(is_complete_g < int >);
    REQUIRES_FALSE(is_complete_g < void >);
}


#endif // ENV_SPECIALIZATION_HPP
