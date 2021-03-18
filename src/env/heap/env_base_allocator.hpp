#ifndef ENV_BASE_ALLOCATOR_HPP
#define ENV_BASE_ALLOCATOR_HPP


tmp<name T = byte_t> typ(allocator_gt) = ENV_ALLOCATOR<T>;

typ(allocator_t) = allocator_gt<byte_t>;

tmp<name T = byte_t> typ(alloc_traits_gt) = ENV_STD::allocator_traits<allocator_gt<T>>;

typ(alloc_traits_t) = alloc_traits_gt<byte_t>;


ENV_DETAIL_BEGIN

tmp<name TAlloc, name TVal> typ(rebound_allocator_gt) = name ENV_STD::allocator_traits<TAlloc>::tmp rebind_alloc<TVal>;

ENV_TEST_CASE("rebound allocator")
{
    REQUIRE_EQT(ENV_STD::allocator < int >, rebound_allocator_gt<ENV_STD::allocator < double>, int >);
}

ENV_DETAIL_END


strct unequal_container_allocator_swap_error_t : ENV_STD::logic_error
{
private:
    DECL_BASE(error, (ENV_STD::logic_error));

public:
    using _error_base_t::logic_error;
};


#endif // ENV_BASE_ALLOCATOR_HPP
