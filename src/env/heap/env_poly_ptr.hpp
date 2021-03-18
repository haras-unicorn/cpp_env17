#ifndef ENV_POLY_PTR_HPP
#define ENV_POLY_PTR_HPP


// Write an actual pointer that uses an allocator.

tmp<name TValue, name TDeleter = deleter_gt <TValue>>
typ(poly_gt) = ENV_STD::unique_ptr<TValue, TDeleter>;

tmp<name TValue>
typ(poly_cgt) = poly_gt<add_const_gt < TValue>>;

tmp<name TValue>
typ(poly_mgt) = poly_gt<remove_const_gt < TValue>>;

COND_TMP((name T, name... TArgs), (ENV_STD::is_constructible_v<T, TArgs...>))
fun poly(TArgs&& ...args)
{
    obj static allocator_gt <T> allocator{ };

    let _subject = alloc_traits_gt<T>::allocate(allocator, one);
    alloc_traits_gt<T>::construct(allocator, _subject, ENV_STD::forward<TArgs>(args)...);

    ret poly_gt<T>{_subject};
}


ENV_TEST_CASE("poly ptr")
{
    let _poly = poly<double>(1);
    REQUIRE_EQ(*_poly, 1);
}


#endif // ENV_POLY_PTR_HPP
