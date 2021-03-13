#ifndef ENV_DYNAMIC_PTR_HPP
#define ENV_DYNAMIC_PTR_HPP

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TPtr, name TAlloc, name = success_t>
    strct dynamic_data_gt
{
    typ(ptr_t) = TPtr;

    NIL((ptr_t), data);
    MEM_CMP_GETTER(data);

protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = FWA_STD::allocator_traits<alloc_t>;

    NIL((TAlloc), alloc);
};

tmp<name TPtr, name TAlloc>
    strct dynamic_data_gt<TPtr, TAlloc, FWA_CORE::require_nt<FWA_STD::is_empty_v<TAlloc> && !FWA_STD::is_final_v<TAlloc>>> : protected TAlloc
{
    typ(ptr_t) = TPtr;

    NIL((ptr_t), data);
    MEM_CMP_GETTER(data);

protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = FWA_STD::allocator_traits<alloc_t>;

    GETTER(_get_alloc, (*this));
};

FWA_NAMESPACE_DETAIL_END

/* TAlloc must be an allocator that meets the standard requirements of an allocator! */
tmp<name TPtr, name TAlloc>
    strct dynamic_ptr_gt : public detail::dynamic_data_gt<TPtr, TAlloc>
{
private:
    DECL_BASE(data, (detail::dynamic_data_gt<TPtr, TAlloc>));

    DECL_THIS(dynamic_ptr_gt);

    using typename _data_base_t::alloc_t;
    using typename _data_base_t::traits_t;

public:
    using typename _data_base_t::ptr_t;

    let_cmp static prop_on_copy = traits_t::propagate_on_container_copy_assignment::value;

    let_cmp static prop_on_move = traits_t::propagate_on_container_move_assignment::value;

    let_cmp static prop_on_swap = traits_t::propagate_on_container_swap::value;

    con cmp inl dynamic_ptr_gt() noex = default;

    DEFAULT_CONST_LIFE(dynamic_ptr_gt, CMP);

    let_cmp static is_alloc_noex = noex(traits_t::allocate(
        declvall<alloc_t>(), declvalr<size_t>()));

    tmp<name... TArgs>
        cmp_obj bool static is_place_noex_vt = noex(traits_t::construct(
            declvall<alloc_t>(), declvalr<ptr_t>(), declval<TArgs>()...));

    let_cmp static is_rem_noex = noex(traits_t::destroy(
        declvall<alloc_t>(), declvalr<ptr_t>()));

    let_cmp static is_free_noex = noex(traits_t::deallocate(
        declvall<alloc_t>(), declvalr<ptr_t>(), declvalr<size_t>()));

    callb inl alloc(size_t size = single) noex(is_alloc_noex)
    {
        this->_get_data() = traits_t::allocate(this->_get_alloc(), size);
    }

    COND_TMP_VARIADIC(FWA_STD::is_constructible_v<val_t, TVar...>)
    callb inl place(TVar && ...args) noex(is_place_noex_vt<TVar...>)
    {
        traits_t::construct(this->_get_alloc(), this->_get_data(), FWA_STD::forward<TVar>(args)...);
    }

    COND_TMP_VARIADIC(FWA_STD::is_constructible_v<val_t, TVar...>)
    callb inl place(ptr_t at, TVar && ...args) noex(is_place_noex_vt<TVar...>)
    {
        traits_t::construct(this->_get_alloc(), at, FWA_STD::forward<TVar>(args)...);
    }

    COND_TMP((name T, name... TVar), (FWA_STD::is_constructible_v<val_t, TVar...>))
    callb inl place(unsigned_c<T> offset, TVar && ...args) noex(is_place_noex_vt<TVar...>)
    {
        traits_t::construct(this->_get_alloc(), this->_get_data() + offset, FWA_STD::forward<TVar>(args)...);
    }

    callb inl rem() noex(is_rem_noex)
    {
        traits_t::destroy(this->_get_alloc(), this->_get_data());
    }

    callb inl rem(ptr_t at) noex(is_rem_noex)
    {
        traits_t::destroy(this->_get_allocator(), at);
    }

    tmp<name T>
        callb inl rem(unsigned_c<T> offset) noex(is_rem_noex)
    {
        traits_t::destroy(this->_get_alloc(), this->_get_data() + offset);
    }

    callb inl free(size_t size = single) noex(is_free_noex)
    {
        traits_t::deallocate(this->_get_alloc(), this->_get_data(), size);
        this->_get_data() = nil;
    }

    ENABLE_IF_COMPAT((is_imp_convertible_tmp_g<T, dynamic_ptr_gt> && are_equatable_g<name T::ptr_t, ptr_t>));

    CMP_VALIDITY { ret this->_get_data() != nil; };
    CMP_TMP_HASH { ret hash(this->_get_data()); };
    CMP_TMP_EQUALITY { ret this->_get_data() == rhs._get_data(); };
};

FWA_NAMESPACE_STD_BEGIN

tmp<name TPtr, name TAlloc>
    strct hash<FWA_CORE::dynamic_ptr_gt<TPtr, TAlloc>>{
        cmp_fn op()(const FWA_CORE::dynamic_ptr_gt<TPtr, TAlloc> &subject) const noex{
            ret subject.hash();
}
}
;

FWA_NAMESPACE_STD_END

typ(dynamic_t) = dynamic_ptr_gt<byte_t>;

TEST_CASE("dynamic")
{
    REQUIRE_EQ(FWA_STD::is_trivial_v<allocator_t>, FWA_STD::is_trivial_v<dynamic_t>);

    SUBCASE("pair")
    {
        obj dynamic_ptr_gt<FWA_STD::pair<int, int>> a;
        a.alloc(2);
        a.place(1, 2);
        a.place(1u, 2, 3);

        REQUIRE_EQ(a.get_data()[0], FWA_STD::pair{1, 2});
        REQUIRE_EQ(a.get_data()[1], FWA_STD::pair{2, 3});

        a.rem();
        a.rem(1u);
        a.free(2);
    }

    SUBCASE("byte")
    {
        obj dynamic_t a;
        a.alloc(2);
        a.place(0u, scast<byte_t>(0));
        a.place(1u, scast<byte_t>(1));

        REQUIRE_EQ(a.get_data()[0], byte_t{0});
        REQUIRE_EQ(a.get_data()[1], byte_t{1});

        a.rem(1u);
        a.rem(1u);
        a.free(2);
    }
}

#endif // ENV_DYNAMIC_PTR_HPP
