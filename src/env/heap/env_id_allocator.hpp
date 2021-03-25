#ifndef ENV_ID_ALLOCATOR_HPP
#define ENV_ID_ALLOCATOR_HPP


// Allocator used for testing

ENV_TEST_BEGIN

tmp<name TVal>
strct id_allocator_gt : public allocator_gt<TVal>
{
    DECL_BASE(alloc, (allocator_gt < TVal > ));
    DECL_THIS(id_allocator_gt);
    DEF_ID;

    typ(propagate_on_container_copy_assignment) = ENV_STD::true_type;
    typ(propagate_on_container_move_assignment) = ENV_STD::false_type;
    typ(propagate_on_container_swap) = ENV_STD::false_type;

    typ(is_always_equal) = ENV_STD::false_type;

    tmp<name T>
    strct rebind
    {
        typ(other) = id_allocator_gt<T>;
    };

    using _alloc_base_t::allocator;
};

ENV_TEST_END

ENV_TEST_CASE("id_allocator")
{
    typ(_val_t) = int;
    typ(_alloc_t) = test::id_allocator_gt<int>;
    typ(_traits_t) = ENV_STD::allocator_traits<_alloc_t>;

    STD_REQUIRE(ENV::is_std_allocator_g < _alloc_t >);
    ENV::detail::allocator_traits_gt<_alloc_t> a{ };
    nonce(a);

    _alloc_t alloc{ };

    _val_t* ptr = _traits_t::allocate(alloc, 1);
    _traits_t::construct(alloc, ptr, 10);

    REQUIRE_EQ(*ptr, 10);

    _traits_t::destroy(alloc, ptr);
    _traits_t::deallocate(alloc, ptr, 1);
}


#endif // ENV_ID_ALLOCATOR_HPP
