#ifndef ENV_CONSTRAIN_HPP
#define ENV_CONSTRAIN_HPP

// floor

tmp<name T> cmp_fn floor(arithmetic_c<T> of) noex
{
    if_cmp(is_floating_g<T>) ret ENV_STD::floor(of);
    else ret of;
}

ENV_TEST_CASE("floor")
{
    REQUIRE_EQ(floor(2.3), doctest::Approx{2});
    REQUIRE_EQ(floor(2), 2);
    REQUIRE_EQ(floor(1_s), 1);
}

// ceil

tmp<name T> cmp_fn ceil(arithmetic_c<T> of) noex
{
    if_cmp(is_floating_g<T>) ret ENV_STD::ceil(of);
    else ret of;
}

ENV_TEST_CASE("ceil")
{
    REQUIRE_EQ(ceil(2.3), doctest::Approx{3});
    REQUIRE_EQ(ceil(2), 2);
    REQUIRE_EQ(ceil(1_s), 1);
}

// round

tmp<name T> cmp_fn round(arithmetic_c<T> of) noex
{
    if_cmp(is_floating_g<T>) ret ENV_STD::round(of);
    else ret of;
}

ENV_TEST_CASE("round")
{
    REQUIRE_EQ(round(2.3), doctest::Approx{2});
    REQUIRE_EQ(round(2.7), doctest::Approx{3});
    REQUIRE_EQ(round(2), 2);
    REQUIRE_EQ(round(1_s), 1);
}

// step

tmp<name T> cmp_fn step(arithmetic_c<T> of) noex { ret 0 < of; }

ENV_TEST_CASE("step")
{
    REQUIRE_EQ(scast<int>(step(1)), 1);
    REQUIRE_EQ(scast<int>(step(2)), 1);
    REQUIRE_EQ(scast<int>(step(-2)), 0);
    REQUIRE_EQ(scast<int>(step(0_s)), 0);
}

// ramp

tmp<name T> cmp_fn ramp(arithmetic_c<T> of) noex
{
    if_cmp(is_signed_g<T>) ret step(of) * of;
    else ret of;
}

ENV_TEST_CASE("step")
{
    REQUIRE_EQ(scast<int>(ramp(1)), 1);
    REQUIRE_EQ(scast<int>(ramp(2)), 2);
    REQUIRE_EQ(scast<int>(ramp(-2)), 0);
    REQUIRE_EQ(scast<int>(ramp(0_s)), 0);
}

// sign

tmp<name T> cmp_fn sign(arithmetic_c<T> of) noex
{
    if_cmp(is_signed_g<T>) ret(0 < of) - (of < 0);
    else ret 0 < of;
}

ENV_TEST_CASE("sign")
{
    REQUIRE_EQ(scast<int>(sign(1)), 1);
    REQUIRE_EQ(scast<int>(sign(2)), 1);
    REQUIRE_EQ(scast<int>(sign(-2)), -1);
    REQUIRE_EQ(scast<int>(sign(0_s)), 0);
}

// abs

tmp<name T> cmp_fn abs(arithmetic_c<T> of) noex
{
    if_cmp(ENV_STD::is_enum_v<T>) ret abs(underlying_cast(of));
    else if_cmp(is_signed_g<T>) ret ENV_STD::abs(of);
    else ret of;
}

ENV_TEST_CASE("abs")
{
    REQUIRE_EQ(abs(2.3), doctest::Approx{2.3});
    REQUIRE_EQ(abs(static_cast<unsigned int>(2)), 2);
    REQUIRE_EQ(abs(-2.3), doctest::Approx{2.3});
    REQUIRE_EQ(abs(2_s), 2);
}

#endif // ENV_CONSTRAIN_HPP
