#ifndef ENV_RESULT_HPP
#define ENV_RESULT_HPP



// nullptr - since void is also here

typ(nullptr_t) = decltype(nullptr);

ENV_TEST_CASE("nullptr")
{
    obj nullptr_t some_nullptr{};
    nonce(some_nullptr);
}

// no return

typ(void_t) = void;
typ(noret_t) = void_t;

ENV_TEST_CASE("no return")
{
    strct
    {
        callb op()() const noex->noret_t {}
    }
    no_return;
    no_return();
}

// result

TAG(nores);

#define res_name name TRes = nores_t, ENV::require_gt<ENV_STD::negation<ENV_STD::is_same<TRes, noret_t>>> = ENV::success
#define deduc_res(...) ENV_STD::conditional_t<ENV_STD::is_same_v<TRes, nores_t>, __VA_ARGS__, TRes>
#define res_t(...) ENV_STD::conditional_t<ENV_STD::is_same_v<TRes, nores_t>, __VA_ARGS__, TRes>
#define res_cast(...) CMP_TERN((ENV_STD::is_same_v<TRes, nores_t>), (__VA_ARGS__), (scast<TRes>(__VA_ARGS__)))
#define res_con(...) CMP_TERN((ENV_STD::is_same_v<TRes, nores_t>), (__VA_ARGS__), (TRes(__VA_ARGS__)))

ENV_NAMESPACE_TEST_BEGIN

tmp<res_name> cmp_fn cast_res() noex->deduc_res(int) { ret res_cast(1); }

tmp<res_name> cmp_fn con_res() noex->deduc_res(int) { ret res_con(1); }

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("result")
{
    let res_cast_i = test::cast_res<int>();
    let res_cast = test::cast_res();
    REQUIRE_EQT(decltype(res_cast_i), decltype(res_cast));

    let res_con_i = test::con_res<int>();
    let res_con = test::con_res();
    REQUIRE_EQT(decltype(res_con_i), decltype(res_con));
}

#endif // ENV_RESULT_HPP
