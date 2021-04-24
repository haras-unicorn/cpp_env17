#include <env/test.hpp>

ENV_MSVC_SUPPRESS_PUSH(6326);


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

    auto           vec = ::env::immut::vector{1, 2, 3, 4, 5};
    constexpr auto drop_two = ::env::trans::drop(2);
    EXPECT_EQ(::env::trans::into(::std::vector<int>{}, drop_two, vec),
              (::std::vector{3, 4, 5}));
}


namespace test
{
template<typename T>
struct vector
{
    BOOST_HANA_DEFINE_STRUCT(vector, (T, x), (T, y));
};

struct vector_tag
{
    using sample = vector<int>;
};

constexpr auto vector_accessors = ::boost::hana::accessors<vector_tag>();
} // namespace test

ENV_TEST(env, regular)
{
    using namespace ::test;
    using namespace ::env::meta;

    vector<int> a{1, 2};
    vector<int> b{2, 3};
    EXPECT_PRED2(not_equal, a, b);
    EXPECT_PRED2(in, BOOST_HANA_STRING("x"), a);
    EXPECT_EQ(keys(a),
              make_tuple(
                      BOOST_HANA_STRING("x"),
                      BOOST_HANA_STRING("y")));
    EXPECT_EQ(members(a),
              make_tuple(1, 2));

    constexpr auto get_vector_x = second(vector_accessors[0_c]);
    EXPECT_EQ(get_vector_x(a), 1);
}


ENV_TEST(env, meta)
{
    using namespace ::env::meta;

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
    EXPECT_EQ(ap(
                      make_tuple(plus, minus),
                      make_tuple(1, 2),
                      make_tuple(2, 3)),
              make_tuple(
                      3, 4, 4, 5,
                      -1, -2, 0, -1));

    EXPECT_TRUE((Monoid<integral_constant<int, 1>>::value));
    EXPECT_EQ(plus(
                      BOOST_HANA_STRING("a"),
                      zero<string_tag>()),
              BOOST_HANA_STRING("a"));

    EXPECT_TRUE((Monad<tuple<int>>::value));
    constexpr auto doublet = [](auto x) { return make_tuple(x, x); };
    constexpr auto quadruplet = monadic_compose(doublet, doublet);
    EXPECT_EQ(quadruplet(1), make_tuple(1, 1, 1, 1));
    EXPECT_EQ(flatten(make_tuple(make_tuple())), make_tuple());

    EXPECT_TRUE(Comonad<lazy_value_t<int>>::value);
    EXPECT_EQ(extract(make_lazy(make_tuple)(0, 1)),
              make_tuple(0, 1));

    EXPECT_TRUE((Hashable<integral_constant<int, 1>>::value));
    EXPECT_EQ(hash(1_c), (type_c<integral_constant<long long, 1>>) );

    EXPECT_TRUE((EuclideanRing<int>::value));

    EXPECT_TRUE((Product<pair<int, int>>::value));

    // TODO: explore more
}

ENV_TEST(env, immutable)
{
    using namespace ::env::immut;

    const alloc::vector<int> v1{1, 2, 3};
    const auto               v2 = v1.push_back(4);
    const auto               v3 = v2.push_back(5);
    const auto               v4 = v3.push_back(6).push_back(7);

    EXPECT_NE(v1, v2);
    EXPECT_NE(v2, v3);
    EXPECT_NE(v4, v3);
    EXPECT_EQ(v4, (alloc::vector<int>{1, 2, 3, 4, 5, 6, 7}));

    const alloc::flex_vector<int> f1{1, 2, 3};
    const auto                    f2 = f1 + alloc::flex_vector<int>{4, 5, 6};

    EXPECT_NE(f1, f2);
    EXPECT_EQ(f2, (alloc::flex_vector<int>{1, 2, 3, 4, 5, 6}));
}

ENV_TEST(env, alloc)
{
    EXPECT_TRUE(mi_option_is_enabled(mi_option_reserve_huge_os_pages));
    EXPECT_TRUE(mi_option_is_enabled(mi_option_eager_commit));
    EXPECT_TRUE(mi_option_is_enabled(mi_option_eager_region_commit));
}


ENV_MSVC_SUPPRESS_POP;
