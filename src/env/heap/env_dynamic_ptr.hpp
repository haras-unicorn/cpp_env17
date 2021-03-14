#ifndef ENV_DYNAMIC_PTR_HPP
#define ENV_DYNAMIC_PTR_HPP


ENV_DETAIL_BEGIN

tmp<name TAlloc, name = success_t>
strct dynamic_data_gt
{
protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    NIL((TAlloc), alloc);

public:
    typ(ptr_t) = void*;

    NIL((ptr_t), data);
    CMP_GETTER_FML((tmp < name T >), (T * ), data, (rcast<T*>(_get_data())));
};

tmp<name TAlloc>
strct dynamic_data_gt<TAlloc, ENV::require_nt<ENV_STD::is_empty_v<TAlloc> && !ENV_STD::is_final_v<TAlloc>>>
        : protected TAlloc
{
protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    GETTER(_get_alloc, (*this));

public:
    typ(ptr_t) = void*;

    NIL((ptr_t), data);
    CMP_GETTER_FML((tmp < name T >), (T * ), data, (rcast<T*>(_get_data())));
};

ENV_DETAIL_END

/* TAlloc must be an allocator that meets the standard requirements of an allocator! */

COND_TMP((name TAlloc), ENV_STD::is_trivial_v < name TAlloc::value_type >)
strct dynamic_ptr_gt : public detail::dynamic_data_gt<TAlloc>
{
private:
    DECL_BASE(data, (detail::dynamic_data_gt<TAlloc>));

    DECL_THIS(dynamic_ptr_gt);

    using name _data_base_t::alloc_t;
    using name _data_base_t::traits_t;

public:
    using name _data_base_t::ptr_t;


    cmp_obj bool static prop_on_copy = traits_t::propagate_on_container_copy_assignment::value;

    cmp_obj bool static prop_on_move = traits_t::propagate_on_container_move_assignment::value;

    cmp_obj bool static prop_on_swap = traits_t::propagate_on_container_swap::value;


    cmp_obj bool static is_alloc_noex = noex(traits_t::allocate(
            declvall<alloc_t>(), declvalr<size_t>()));

    tmp<name T, name... TArgs>
    cmp_obj bool static is_place_noex_v = noex(traits_t::construct(
            declvall<alloc_t>(), declvalr<T*>(), declval<TArgs>()...));

    tmp<name T>
    cmp_obj bool static is_rem_noex_g = noex(traits_t::destroy(
            declvall<alloc_t>(), declvalr<T*>()));

    cmp_obj bool static is_free_noex = noex(traits_t::deallocate(
            declvall<alloc_t>(), declvalr<name traits_t::pointer>(), declvalr<size_t>()));


    con cmp inl dynamic_ptr_gt() noex = default;

    DEFAULT_CONST_LIFE(dynamic_ptr_gt, CMP);

    cmp_fn copy_from(const dynamic_ptr_gt& other, size_t size)
    {
#if ENV_CPP >= 17
        if_cmp (traits_t::is_always_equal::value)
        {
            copy(other._get_data(), this->_get_data(), size);
            ret;
        }
#endif // ENV_CPP >= 17
        if (this->_get_alloc() == other._get_alloc())
        {
            copy(other._get_data(), this->_get_data(), size);
            if_cmp (prop_on_copy) this->_get_alloc() = other._get_alloc();
            ret;
        }
        else
        {
            free(size);
            this->_get_data() = traits_t::allocate(other._get_alloc(), size);
            copy(other._get_data(), this->_get_data(), size);
            if_cmp (prop_on_copy) this->_get_alloc() = other._get_alloc();
            ret;
        }
    }

    cmp_fn move_from(dynamic_ptr_gt&& other, size_t size)
    {
#if ENV_CPP >= 17
        if_cmp (traits_t::is_always_equal::value)
        {
            free(size);
            this->_get_data() = ENV_STD::move(other._get_data());
            ret;
        }
#endif // ENV_CPP >= 17
        if_cmp (prop_on_move)
        {
            free(size);
            this->_get_alloc() = ENV_STD::move(other._get_alloc());
            this->_get_data() = ENV_STD::move(other._get_data());
            ret;
        }

        if (this->_get_alloc() == other._get_alloc())
        {
            free(size);
            this->_get_data() = ENV_STD::move(other._get_data());
            ret;
        }

        copy(other._get_data(), this->_get_data(), size);
        ret;
    }


    callb inl alloc(size_t size = single) noex(is_alloc_noex)
    {
        this->_get_data() = traits_t::allocate(this->_get_alloc(), size);
    }


    COND_TMP((name T, name... TVar), ENV_STD::is_trivial_v < T > && ENV_STD::is_constructible_v < T, TVar...>)
    callb inl place(TVar&& ...args) noex(is_place_noex_v<T, TVar...>)
    {
        traits_t::construct(this->_get_alloc(), rcast<T*>(this->_get_data()), ENV_STD::forward<TVar>(args)...);
    }

    COND_TMP((name T, name... TVar), ENV_STD::is_trivial_v < T > && ENV_STD::is_constructible_v < T, TVar...>)
    callb inl place(T* at, TVar&& ...args) noex(is_place_noex_v<T, TVar...>)
    {
        traits_t::construct(this->_get_alloc(), at, ENV_STD::forward<TVar>(args)...);
    }

    COND_TMP((name T, name TOff, name... TVar), ENV_STD::is_trivial_v <T>&& ENV_STD::is_constructible_v<T,
            TVar...>)
    callb inl place(unsigned_c <TOff> offset, TVar&& ...args) noex(is_place_noex_v<T, TVar...>)
    {
        traits_t::construct(this->_get_alloc(), rcast<T*>(this->_get_data()) + offset, ENV_STD::forward<TVar>(args)...);
    }


    COND_TMP_UNARY(ENV_STD::is_trivial_v < T >)
    callb inl rem() noex(is_rem_noex_g<T>)
    {
        traits_t::destroy(this->_get_alloc(), rcast<T*>(this->_get_data()));
    }

    COND_TMP_UNARY(ENV_STD::is_trivial_v < T >)
    callb inl rem(T* at) noex(is_rem_noex_g<T>)
    {
        traits_t::destroy(this->_get_allocator(), at);
    }

    COND_TMP((name T, name TOff), ENV_STD::is_trivial_v < T >)
    callb inl rem(unsigned_c <TOff> offset) noex(is_rem_noex_g<T>)
    {
        traits_t::destroy(this->_get_alloc(), rcast<T*>(this->_get_data()) + offset);
    }


    callb inl free(size_t size = single) noex(is_free_noex)
    {
        traits_t::deallocate(this->_get_alloc(), rcast<name traits_t::pointer>(this->_get_data()), size);
        this->_get_data() = nil;
    }


    COND_COMPAT((is_imp_convertible_tmp_g < T, dynamic_ptr_gt > && are_equatable_g < name T::ptr_t, ptr_t >));

    CMP_VALIDITY { ret this->_get_data() != nil; };
    CMP_TMP_HASH { ret hash(this->_get_data()); };
    CMP_TMP_EQUALITY { ret this->_get_data() == rhs._get_data(); };
};

ENV_STD_BEGIN

tmp<name TAlloc>
strct hash<ENV::dynamic_ptr_gt<TAlloc>>
{
    cmp_fn op()(const ENV::dynamic_ptr_gt<TAlloc>& subject) const noex
    {
        ret subject.hash();
    }
};

ENV_STD_END

typ(dynamic_t) = dynamic_ptr_gt<allocator_gt < byte_t>>;


TEST_CASE("dynamic")
{
    REQUIRE_EQ(ENV_STD::is_trivial_v < allocator_t >, ENV_STD::is_trivial_v < dynamic_t >);

    SUBCASE("pair")
    {
        obj dynamic_ptr_gt<allocator_gt < ENV_STD::pair<int, int>> > a;
        a.alloc(2);
        a.place<ENV_STD::pair<int, int>>(1, 2);
        a.place<ENV_STD::pair<int, int>>(1u, 2, 3);

        REQUIRE_EQ(a.get_data < ENV_STD::pair < int, int >> ()[0], ENV_STD::pair{1, 2});
        REQUIRE_EQ(a.get_data < ENV_STD::pair < int, int >> ()[1], ENV_STD::pair{2, 3});

        a.rem<ENV_STD::pair<int, int>>();
        a.rem<ENV_STD::pair<int, int>>(1u);
        a.free(2);
    }

    SUBCASE("byte")
    {
        obj dynamic_t a;
        a.alloc(2);
        a.place<byte_t>(0u, scast<byte_t>(0));
        a.place<byte_t>(1u, scast<byte_t>(1));

        REQUIRE_EQ(a.get_data<byte_t>()[0], byte_t{0});
        REQUIRE_EQ(a.get_data<byte_t>()[1], byte_t{1});

        a.rem<byte_t>(1u);
        a.rem<byte_t>(1u);
        a.free(2);
    }
}


#endif // ENV_DYNAMIC_PTR_HPP
