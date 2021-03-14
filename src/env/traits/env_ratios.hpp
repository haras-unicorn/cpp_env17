#ifndef ENV_RATIOS_HPP
#define ENV_RATIOS_HPP


ENV_DETAIL_BEGIN

typ(std_ratio_num_t) = decl(ENV_STD::ratio<1, 1>::num);

ENV_DETAIL_END

tmp<uintmax_t Nominator, uintmax_t Denominator = 1>
typ(nratio_nns) = ENV_STD::ratio<Nominator, Denominator>;

tmp<uintmax_t Nominator, uintmax_t Denominator>
cmp_obj nratio_nns<Nominator, Denominator> nratio{ };

tmp<intmax_t Nominator, intmax_t Denominator = 1>
typ(ratio_nns) = ENV_STD::ratio<Nominator, Denominator>;

tmp<intmax_t Nominator, intmax_t Denominator>
cmp_obj ratio_nns<Nominator, Denominator> ratio{ };

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

COND_CHECK_UNARY(is_nratio, (T::num > 0 && T::den > 0));

COND_CONCEPT(nratio, (is_nratio_g<C>));

COND_CHECK_UNARY(is_ratio, (T::num || T::den));

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
