#ifndef FWA_CORE_COMMON_HPP
#define FWA_CORE_COMMON_HPP


tmp<name... T>
typ(common_vt) = FWA_STD::common_type_t<FWA_CORE::unqualified_gt<T>...>;


FWA_CORE_TEST_CASE("common")
{
    SUBCASE("integral")
    {
        REQUIRE_EQT(common_vt<int&&, const double&>, double );
    }

    SUBCASE("base")
    {
        strct base_t { };
        strct derived_t : base_t { };

        REQUIRE_EQT(common_vt<const derived_t&, base_t&&>, base_t );
    }
}


#endif // FWA_CORE_COMMON_HPP
