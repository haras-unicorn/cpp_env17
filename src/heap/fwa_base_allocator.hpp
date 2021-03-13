#ifndef FWA_CORE_BASE_ALLOCATOR_HPP
#define FWA_CORE_BASE_ALLOCATOR_HPP


tmp<name T = byte_t> typ(allocator_gt) = FWA_ALLOCATOR<T>;

typ(allocator_t) = allocator_gt<byte_t>;

tmp<name T = byte_t> typ(alloc_traits_gt) = FWA_STD::allocator_traits<allocator_gt<T>>;

typ(alloc_traits_t) = alloc_traits_gt<byte_t>;


FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TAlloc, name TVal> typ(rebound_allocator_gt) = name FWA_STD::allocator_traits<TAlloc>::tmp rebind_alloc<TVal>;

FWA_CORE_TEST_CASE("rebound allocator")
{
    REQUIRE_EQT(FWA_STD::allocator<int>, rebound_allocator_gt<FWA_STD::allocator<double>, int>);
}

FWA_NAMESPACE_DETAIL_END


#endif // FWA_CORE_BASE_ALLOCATOR_HPP
