#ifndef ENV_RATIOS_HPP
#define ENV_RATIOS_HPP

FWA_NAMESPACE_DETAIL_BEGIN

typ(std_ratio_num_t) = decl(FWA_STD::ratio<1, 1>::num);

FWA_NAMESPACE_DETAIL_END

tmp<uintmax_t Nominator, uintmax_t Denominator = 1>
    typ(nratio_nns) =
        FWA_STD::ratio<
            scast<detail::std_ratio_num_t>(Nominator),
            scast<detail::std_ratio_num_t>(Denominator)>;

tmp<uintmax_t Nominator, uintmax_t Denominator>
    cmp_obj nratio_nns<Nominator, Denominator> nratio{};

tmp<intmax_t Nominator, intmax_t Denominator = 1>
    typ(ratio_nns) =
        FWA_STD::ratio<
            scast<detail::std_ratio_num_t>(Nominator),
            scast<detail::std_ratio_num_t>(Denominator)>;

tmp<intmax_t Nominator, intmax_t Denominator>
    cmp_obj ratio_nns<Nominator, Denominator> ratio{};

ENV_TEST_CASE("ratios")
{
    SUBCASE("natural")
    {
        REQUIRE_EQ(ratio_nns<-1, 2>::num, ratio_nns<-2, 4>::num);
        REQUIRE_EQ(decl(ratio<-1, 2>)::num, -1);
    }
    SUBCASE("whole")
    {
        REQUIRE_EQ(nratio_nns<1, 2>::num, nratio_nns<2, 4>::num);
        REQUIRE_EQ(decl(nratio<1, 2>)::num, 1);
    }
}

FWA_NAMESPACE_DETAIL_BEGIN

EXTRACT_CHECK(
    is_ratio,
    (std_ratio_num_t Nominator, std_ratio_num_t Denominator),
    (FWA_STD::ratio<Nominator, Denominator>));

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY(is_nratio, (detail::is_ratio_g<T> && T::num > 0 && T::den > 0));

COND_CONCEPT(nratio, (is_nratio_g<C>));

COND_CHECK_UNARY(is_ratio, (detail::is_ratio_g<T>));

COND_CONCEPT(ratio, (is_ratio_g<C>));

ENV_TEST_CASE("ratio checks")
{
    SUBCASE("natural")
    {
        REQUIRES(is_nratio_g<nratio_nns<1, 2>>);
        REQUIRES_FALSE(is_nratio_g<ratio_nns<-1, 2>>);
        // Weird, I know, but it is true, because it is the same as 1 / 2 which is natural.
        REQUIRES(is_nratio_g<ratio_nns<-1, -2>>);
        REQUIRES_FALSE(is_nratio_g<int>);
    }

    SUBCASE("whole")
    {
        REQUIRES(is_ratio_g<ratio_nns<1, 2>>);
        REQUIRES_FALSE(is_ratio_g<int>);
    }
}

#endif // ENV_RATIOS_HPP
