#ifndef FWA_CORE_BODY_HPP
#define FWA_CORE_BODY_HPP


// structs

#define ANON_STRUCT struct [[maybe_unused]] {}

#define FORMAT ANON_STRUCT

FWA_CLANG_SUPPRESS_PUSH("bugprone-macro-parentheses")

#define ACCESS_BEGIN(_access) ANON_STRUCT; PACK(_access): ANON_STRUCT
#define ACCESS_END(_access) ANON_STRUCT; PACK(_access): ANON_STRUCT

FWA_CLANG_SUPPRESS_POP

#define CLASS_SEMI ANON_STRUCT

FWA_CORE_TEST_CASE("struct macros")
{
    SUBCASE("anon")
    {
        strct test
        {
            ACCESS_BEGIN(private);
            ACCESS_END(public);
            CLASS_SEMI;
        };
    }
}


// application formula body

#define APP_FML(_name, _application) (_name SPREAD(_application);)
#define FML_BODY(_formula) (return SPREAD(_formula);)

FWA_CORE_TEST_CASE("application formula body")
{
    strct test
    {
        inline constexpr static int return_1() { ret 1; }

        inline constexpr static int return_1_formula() { ret SPREAD(APP_FML(return_1, (()))); }

        inline constexpr static int return_1_body() { SPREAD(FML_BODY((return_1()))); }
    };

    REQUIRES(test::return_1() == 1);
    REQUIRES(test::return_1_formula() == 1);
    REQUIRES(test::return_1_body() == 1);
}


#endif // FWA_CORE_BODY_HPP
