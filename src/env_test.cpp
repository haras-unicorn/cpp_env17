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
template<typename T>
struct vector
{
    BOOST_HANA_DEFINE_STRUCT(vector, (T, x), (T, y));
};

struct vector_tag
{
    using sample = vector<int>;
};
} // namespace test

namespace boost::hana
{
template<typename TTag>
struct accessors_impl<TTag, when<struct_detail::is_valid<typename TTag::sample>::value>>
{
private:
    static constexpr auto sample_inner =
            TTag::sample::hana_accessors_impl::apply();

    template<typename T>
    static constexpr decltype(T::hana_accessors_impl::apply()) inner =
            T::hana_accessors_impl::apply();

    template<std::size_t... I, typename... T>
    static constexpr decltype(auto) _apply(
            detail::basic_tuple_impl<std::index_sequence<I...>, T...>
                    _sample_inner)
    {
        constexpr auto getter =
                [](auto i, auto&& x) {
                    using type = decltype(x);
                    using unqualified =
                            std::remove_reference_t<
                                    std::remove_cv_t<type>>;

                    return second(inner<unqualified>[i])(
                            std::forward<type>(x));
                };

        return make_tuple(
                make_pair(
                        first(detail::ebo_get<detail::bti<I>>(_sample_inner)),
                        partial(getter, size_c<I>))...);
    }

public:
    static constexpr decltype(auto) apply()
    {
        return _apply(sample_inner.storage_);
    }
};
} // namespace boost::hana

namespace test
{
constexpr auto vector_accessors = ::boost::hana::accessors<vector_tag>();
} // namespace test

ENV_TEST(env, regular)
{
    test::vector<int> a{1, 2};
    test::vector<int> b{2, 3};
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
    constexpr auto doublet = [=](auto x) { return make_tuple(x, x); };
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
}


int main(int argc, char** argv)
{
    env::test::InitGoogleMock(&argc, argv);
    env::test::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
