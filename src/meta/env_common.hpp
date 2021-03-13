#ifndef ENV_COMMON_HPP
#define ENV_COMMON_HPP

tmp<name... T>
    typ(common_vt) = ENV_STD::common_type_t<ENV::unqualified_gt<T>...>;

ENV_TEST_CASE("common")
{
    SUBCASE("integral")
    {
        REQUIRE_EQT(common_vt<int &&, const double &>, double);
    }

    SUBCASE("base")
    {
        strct base_t{};
        strct derived_t : base_t{};

        REQUIRE_EQT(common_vt<const derived_t &, base_t &&>, base_t);
    }
}

#endif // ENV_COMMON_HPP
