#ifndef ENV_MATH_HPP
#define ENV_MATH_HPP

// mod

tmp<name T, name TBy>
cmp_fn mod(arithmetic_c <T> to_mod, arithmetic_c <TBy> by) noex
{
    if_cmp(ENV_STD::is_enum_v<T>)
        ret mod(underlying_cast(to_mod), by);
    else if_cmp(ENV_STD::is_enum_v<TBy>)
        ret mod(to_mod, underlying_cast(by));
    else if_cmp(is_floating_g < T > || is_floating_g < TBy >)
        ret ENV_STD::fmod(to_mod, by);
    else if_cmp(is_signed_g < T > && is_signed_g < TBy >)
        ret to_mod % by;
    else if_cmp(is_signed_g < T >)
        ret to_mod %
            scast<whole_nnt<true, ENV_STD::max(sizeof(T), sizeof(TBy))>>(by);
    else if_cmp(is_signed_g < TBy >)
        ret scast<whole_nnt<true, ENV_STD::max(sizeof(T), sizeof(TBy))>>(to_mod) % by;
    else
        ret to_mod % by;
}

ENV_TEST_CASE("mod")
{
    REQUIRE_EQ(mod(2.3, 0.5), doctest::Approx{0.3});
    REQUIRE_EQ(mod(2.3, 1), doctest::Approx{0.3});
    REQUIRE_EQ(mod(1_s, 0.3), doctest::Approx{0.1});
    REQUIRE_EQ(mod(1, -0.3), doctest::Approx{0.1});
    REQUIRE_EQ(mod(-1, -0.3), doctest::Approx{-0.1});
    REQUIRE_EQ(mod(-1, 0.3), doctest::Approx{-0.1});
    REQUIRE_EQ(mod(3, 2_s), 1);
}


ENV_MSVC_SUPPRESS_PUSH(4244) // conversion from double to int -> this is what we want here

// divf - floor of division

tmp<name TNom, name TDiv>
cmp_fn divf(arithmetic_c <TNom> nom, arithmetic_c <TDiv> div) noex
{
    if_cmp(is_floating_g < TNom > || is_floating_g < TDiv >)
        ret scast<int64_t>(nom / div);
    else if_cmp(is_signed_g < TNom > && is_signed_g < TDiv >)
        ret nom / div;
    else if_cmp(is_signed_g < TNom >)
        ret nom / scast<whole_nnt<true, ENV_STD::max(sizeof(TNom), sizeof(TDiv))>>(div);
    else if_cmp(is_signed_g < TDiv >)
        ret scast<whole_nnt<true, ENV_STD::max(sizeof(TNom), sizeof(TDiv))>>(nom) / div;
    else
        ret nom / div;
}

TEST_CASE("divf")
{
    SUBCASE("whole")
    {
        REQUIRE_EQ(divf(2, 2_s), 1);
        REQUIRE_EQ(divf(1_s, 2), 0);
        REQUIRE_EQ(divf(-4, 3), -1);
        REQUIRE_EQ(divf(-4, -3), 1);
        REQUIRE_EQ(divf(-4_w, 3_n), -1);
        REQUIRE_EQ(divf(4_n, -3_w), -1);
    }
    SUBCASE("floating")
    {
        REQUIRE_EQ(divf(1, 0.5), 2);
        REQUIRE_EQ(divf(0.6, 0.5), 1);
        REQUIRE_EQ(divf(-0.6, 0.5), -1);
        REQUIRE_EQ(divf(-0.6, -0.5), 1);
    }
}

// divc - ceiling of division
// TODO - test this thing more

tmp<name TNom, name TDiv>
cmp_fn divc(arithmetic_c <TNom> nom, arithmetic_c <TDiv> div) noex
{
    if_cmp(is_unsigned_g < TNom > && is_unsigned_g < TDiv >)
        ret divf(nom, div) + !!mod(nom, div);
    else if_cmp(is_signed_g < TNom > && is_unsigned_g < TDiv >)
        ret divf(nom, div) + !!mod(nom, div) * sign(nom);
    else if_cmp(is_unsigned_g < TNom > && is_signed_g < TDiv >)
        ret divf(nom, div) + !!mod(nom, div) * sign(div);
    else
        ret divf(nom, div) + !!mod(nom, div) * sign(nom) * sign(div);
}

TEST_CASE("divf")
{
    SUBCASE("whole")
    {
        REQUIRE_EQ(divc(2_n, 2_s), 1);
        REQUIRE_EQ(divc(1_s, 2_n), 1);
        REQUIRE_EQ(divc(-4_w, 3_n), -2);
        REQUIRE_EQ(divc(-4_w, -3_w), 2);
        REQUIRE_EQ(divc(4_n, -3_w), -2);
    }
    SUBCASE("floating")
    {
        REQUIRE_EQ(divc(1, 0.5), 2);
        REQUIRE_EQ(divc(0.6, 0.5), 2);
        REQUIRE_EQ(divc(-0.6, 0.5), -2);
        REQUIRE_EQ(divc(-0.6, -0.5), 2);
    }
}

ENV_MSVC_SUPPRESS_POP


// alignf

tmp<name T, name TAlign>
cmp_fn alignf(arithmetic_c <T> to_align, arithmetic_c <TAlign> align) noex
{
    ret divf(to_align, align) * align;
}

ENV_TEST_CASE("alignf")
{
    SUBCASE("whole")
    {
        REQUIRE_EQ(alignf(3_w, -2_w), 2);
        REQUIRE_EQ(alignf(3_w, 2_s), 2);
        REQUIRE_EQ(alignf(4_s, -2_w), 4);
        REQUIRE_EQ(alignf(-3_w, -2_w), -2);
        REQUIRE_EQ(alignf(-4_w, -2_w), -4);
    }

    SUBCASE("floating")
    {
        REQUIRE_EQ(alignf(0, 0.2), doctest::Approx{0});
        REQUIRE_EQ(alignf(0.3, -0.2), doctest::Approx{0.2});
        REQUIRE_EQ(alignf(-0.3, 0.3), doctest::Approx{-0.3});
        REQUIRE_EQ(alignf(-0.3, -0.2), doctest::Approx{-0.2});
    }
}

// alignc
// TODO - test this thing more

tmp<name T, name TAlign>
cmp_fn alignc(arithmetic_c <T> to_align, arithmetic_c <TAlign> align) noex
{
    if_cmp(is_floating_g < T > || is_floating_g < TAlign >)
        ret divc(to_align, align) * align;
    else if_cmp(is_unsigned_g < T > && is_unsigned_g < TAlign >)
        ret divf((to_align + align - 1), align) * align;
    else if_cmp(is_unsigned_g < T > && is_signed_g < TAlign >)
        ret divf((to_align + abs(align) - 1), align) * align;
    else if_cmp(is_signed_g < T > && is_unsigned_g < TAlign >)
        ret divf((to_align + sign(to_align) * (align - 1)), align) * align;
    else
        ret divf((to_align + sign(to_align) * (abs(align) - 1)), align) * align;
}

ENV_TEST_CASE("alignc")
{
    SUBCASE("whole")
    {
        REQUIRE_EQ(alignc(3_n, -2_w), 4);
        REQUIRE_EQ(alignc(3_n, 2_s), 4);
        REQUIRE_EQ(alignc(4_s, -2_w), 4);
        REQUIRE_EQ(alignc(-3_w, -2_w), -4);
        REQUIRE_EQ(alignc(-3_w, 2_n), -4);
        REQUIRE_EQ(alignc(-4_w, -2_w), -4);
    }

    SUBCASE("floating")
    {
        REQUIRE_EQ(alignc(0, 0.2), doctest::Approx{0});
        REQUIRE_EQ(alignc(0.3, -0.2), doctest::Approx{0.4});
        REQUIRE_EQ(alignc(-0.3, 0.3), doctest::Approx{-0.3});
        REQUIRE_EQ(alignc(-0.3, -0.2), doctest::Approx{-0.4});
    }
}


#endif // ENV_MATH_HPP
