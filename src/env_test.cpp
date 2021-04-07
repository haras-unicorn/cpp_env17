#include <env/env.hpp>

using namespace env::syntax;


ENV_TEST(env, deps)
{
    constexpr auto constant_one = 1_c;
    EXPECT_EQ((_ + 1)(constant_one), 2);

    constexpr auto constant_hello = BOOST_HANA_STRING("hello");
    EXPECT_EQ(
            constant_hello,
            (::env::meta::string_c<'h', 'e', 'l', 'l', 'o'>) );

    auto runtime_hello = "hello"s;
    EXPECT_EQ(runtime_hello, ::std::string{"hello"});

    auto my_json = R"({ "var": 1 })"_json;
    EXPECT_EQ(my_json, (::env::json::json{{"var", 1}}));
}

namespace test
{
struct vector
{
    BOOST_HANA_DEFINE_STRUCT(
            vector,
            (int, x),
            (int, y));
};

inline constexpr auto vector_accessors = ::env::meta::accessors<vector>();
} // namespace test

ENV_TEST(env, regular)
{
    test::vector a{1, 2};
    test::vector b{2, 3};
    EXPECT_PRED2(::env::meta::not_equal, a, b);
    EXPECT_PRED2(::env::meta::in, BOOST_HANA_STRING("x"), a);
    EXPECT_EQ(
            ::env::meta::keys(a),
            env::meta::make_tuple(
                    BOOST_HANA_STRING("x"),
                    BOOST_HANA_STRING("y")));
    EXPECT_EQ(::env::meta::members(a),
              ::env::meta::make_tuple(
                      1, 2));

    constexpr auto get_vector_x =
            ::env::meta::second(test::vector_accessors[0_c]);
    EXPECT_EQ(get_vector_x(a), 1);
}

ENV_TEST(env, concepts)
{
    using namespace ::env::meta;
    using namespace ::env::syntax;

    EXPECT_TRUE((Functor<tuple<int, int>>::value));
    EXPECT_EQ(transform(
                      make_tuple(1, 2.0),
                      [](auto i) { return ::env::json::json{{"val", i}}; }),
              make_tuple(
                      R"({"val":1})"_json,
                      R"({"val":2.0})"_json));
    // transform on predicate
    EXPECT_EQ(adjust_if(make_tuple(1, 2, 3), _ > 1, _ + 2),
              make_tuple(1, 4, 5));

    EXPECT_TRUE((Applicative<optional<int>>::value));
    EXPECT_EQ()

    EXPECT_TRUE((EuclideanRing<int>::value));

    EXPECT_TRUE((Product<pair<int, int>>::value));
}


int main(int argc, char** argv)
{
    env::test::InitGoogleMock(&argc, argv);
    env::test::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
