#ifndef ENV_CONSTEXPR_HPP
#define ENV_CONSTEXPR_HPP


// this seems to be a GCC bug
// https://stackoverflow.com/questions/43043397/can-one-use-parameter-value-directly-not-its-type-but-value-itself-in-trailing

#if ENV_GCC == 0

template<name TCallable, name... TArgs>
cmp_callb enable_constexpr(TCallable&& callable, TArgs&&... args) noex
        ->value_gnt<decl((ENV_STD::forward<TCallable>(callable))(
                            ENV_STD::forward<TArgs>(args)...)),
                    (ENV_STD::forward<TCallable>(callable))(
                            ENV_STD::forward<TArgs>(args)...)>
{
    ret value_gnt<decl((ENV_STD::forward<TCallable>(callable))(
                          ENV_STD::forward<TArgs>(args)...)),
                  (ENV_STD::forward<TCallable>(callable))(
                          ENV_STD::forward<TArgs>(args)...)>{};
}


#else // ENV_GCC

template<name TCallable, name... TArgs>
cmp_callb enable_constexpr(TCallable&& callable, TArgs&& args...) { }

#endif // ENV_GCC


template<name TCallable, name... TArgs>
cmp_callb is_constexpr(
        nonced TCallable&& callable, nonced TArgs&&... args, ...) noex
->false_t
{
    ret false_t{};
}

template<name TCallable, name... TArgs>
cmp_callb is_constexpr(
        nonced TCallable&& callable, nonced TArgs&&... args) noex
->decl(nonce(enable_constexpr(
        ENV_STD::forward<TCallable>(callable),
        ENV_STD::forward<TArgs>(args)...)),
        true_t{})
{
    ret true_t{};
}


ENV_TEST_CASE("is constexpr")
{
    let cmp lambda = []() { ret 1; };
    nonced value_gnt<int, lambda()> val{};
    //    enable_constexpr(lambda);

    //    REQUIRES(is_constexpr([] { ret 1; }));
}


#endif // ENV_CONSTEXPR_HPP
