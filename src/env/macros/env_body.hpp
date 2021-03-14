#ifndef ENV_BODY_HPP
#define ENV_BODY_HPP


// structs

#define ACCESS_BEGIN(_access) SEMI; PACK(_access) : SEMI
#define ACCESS_END(_access) SEMI; PACK(_access) : SEMI

ENV_TEST_CASE("struct macros")
{
    SUBCASE("anon")
    {
        strct test
        {
            ACCESS_BEGIN(private);
            ACCESS_END(public);
            SEMI;
        };
    }
}


// application formula body

#define APP_FML(_name, _application) (_name SPREAD(_application);)
#define FML_BODY(_formula) (return SPREAD(_formula);)

ENV_TEST_CASE("application formula body")
{
    strct test
    {
        inline constexpr static int return_1()
        {
            ret 1;
        }

        inline constexpr static int return_1_formula() { ret SPREAD(APP_FML(return_1, (()))); }

        inline constexpr static int return_1_body() { SPREAD(FML_BODY((return_1()))); }
    };

    REQUIRES(test::return_1() == 1);
    REQUIRES(test::return_1_formula() == 1);
    REQUIRES(test::return_1_body() == 1);
}


#endif // ENV_BODY_HPP
