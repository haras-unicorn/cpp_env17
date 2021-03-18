#ifndef ENV_DYNAMIC_HPP
#define ENV_DYNAMIC_HPP


ENV_DETAIL_BEGIN

EXPR_CHECK_UNARY
(
        is_dynamic_data,

        nonce(T{ }),
        nonce(T{declvall<const T>()}),
        nonce(T{declvalr<T>()}),
        declvalr<T>().~T()
);

#ifdef ENV_STANDARD_REQUIREMENTS

COND_CHECK_UNARY(is_dynamic_alloc, is_std_allocator_g < T >);

#else

COND_CHECK_UNARY(is_dynamic_alloc, true);

#endif


tmp<name TData, name TAlloc, name = requirement_t>
strct dynamic_data_ggt
{
protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(ptr_t) = name traits_t::pointer;
    typ(c_ptr_t) = name traits_t::const_pointer;
    typ(void_ptr_t) = name traits_t::void_pointer;
    typ(c_void_ptr_t) = name traits_t::const_void_pointer;

    typ(alloc_size_t) = name traits_t::size_type;


public:
    typ(data_t) = TData;

    NIL((data_t), data);
    MEM_GETTER(data);

    NIL((alloc_t), alloc);
    MEM_GETTER(alloc);


protected:
    imp cmp inl dynamic_data_ggt() noex = default;

    AUTO_NOCON_LIFE
    (
            dynamic_data_ggt, NO_ATTRIBUTE,
            (: _alloc{ traits_t::select_on_container_copy_construction(other._get_alloc()) }, _data{ }),
            (: _alloc{ ENV_STD::move(other._get_alloc()) }, _data{ })
    );
};

tmp<name TData, name TAlloc>
strct dynamic_data_ggt<
        TData, TAlloc,
        ENV::require_nt<ENV_STD::is_empty_v<TAlloc> && !ENV_STD::is_final_v<TAlloc>>> :
        public TAlloc
{

protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(ptr_t) = name traits_t::pointer;
    typ(c_ptr_t) = name traits_t::const_pointer;
    typ(void_ptr_t) = name traits_t::void_pointer;
    typ(c_void_ptr_t) = name traits_t::const_void_pointer;

    typ(alloc_size_t) = name traits_t::size_type;


public:
    typ(data_t) = TData;

    NIL((data_t), data);
    MEM_GETTER(data);

    GETTER(get_alloc, (*this));

protected:
    GETTER(_get_alloc, (*this));


    imp cmp inl dynamic_data_ggt() noex = default;

    AUTO_NOCON_LIFE
    (
            dynamic_data_ggt, NO_ATTRIBUTE,
            (: alloc_t{ traits_t::select_on_container_copy_construction(other._get_alloc()) }, _data{ }),
            (: alloc_t{ ENV_STD::move(other._get_alloc()) }, _data{ })
    );
};

ENV_DETAIL_END

// Data has to have constructors, assignments, swap and destructor
// Allocator has to satisfy Allocator named requirements.
// Allocator named requirements: https://en.cppreference.com/w/cpp/named_req/Allocator
COND_TMP((name TData, name TAlloc), detail::is_dynamic_data_g<TData> && detail::is_dynamic_alloc_g<TAlloc>)
strct dynamic_ggt final : public detail::dynamic_data_ggt<TData, TAlloc>
{
private:
    DECL_BASE(data, (detail::dynamic_data_ggt<TData, TAlloc>));

    DECL_THIS(dynamic_ggt);

    using name _data_base_t::alloc_t;
    using name _data_base_t::traits_t;

    using name _data_base_t::ptr_t;
    using name _data_base_t::c_ptr_t;
    using name _data_base_t::void_ptr_t;
    using name _data_base_t::c_void_ptr_t;

public:
    using name _data_base_t::data_t;


private:
#if ENV_CPP >= 17
    nonced cmp_obj_p bool static alloc_always_equal{traits_t::propagate_on_container_copy_assignment::value};
#endif // ENV_CPP >= 17

    nonced cmp_obj_p bool static alloc_prop_on_copy{traits_t::propagate_on_container_copy_assignment::value};

    nonced cmp_obj_p bool static alloc_prop_on_move{traits_t::propagate_on_container_move_assignment::value};

    nonced cmp_obj_p bool static alloc_prop_on_swap{traits_t::propagate_on_container_swap::value};


public:
    imp cmp inl dynamic_ggt() noex = default;

    DEFAULT_CONST_LIFE(dynamic_ggt, NO_ATTRIBUTE);


    nonced cmp_obj_p bool static always_copies{alloc_always_equal};

    enm copy_strategy_t { copy, realloc_copy };

    cmp_fn copy_strategy(nonced const dynamic_ggt& other) noex
    {
#if ENV_CPP >= 17
        if_cmp (alloc_always_equal) ret copy_strategy_t::copy;
#endif // ENV_CPP >= 17

        let allocs_equal = this->_get_alloc() == other._get_alloc();

        // TODO: test if this should check for eq first
        if_cmp (alloc_prop_on_copy) this->_get_alloc() = other._get_alloc();

        ret allocs_equal ? copy_strategy_t::copy : copy_strategy_t::realloc_copy;
    }


    nonced cmp_obj_p bool static always_moves{alloc_always_equal || alloc_prop_on_move};

    enm move_strategy_t { move, move_elements };

    cmp_fn move_strategy(nonced dynamic_ggt&& other) noex
    {
#if ENV_CPP >= 17
        if_cmp (alloc_always_equal) ret copy_strategy_t::move;
#endif // ENV_CPP >= 17
        if_cmp (alloc_prop_on_move)
        {
            // TODO: test if this should check for eq first
            this->_get_alloc() = ENV_STD::move(other._get_alloc());
            ret move_strategy_t::move;
        }

        let allocs_equal = this->_get_alloc() == other._get_alloc();
        ret allocs_equal ? move_strategy_t::move : move_strategy_t::move_elements;
    }


    nonced cmp_obj_p bool static always_swaps{alloc_always_equal || alloc_prop_on_swap};

    enm swap_strategy_t { swap, undefined };

    cmp_fn swap_strategy(nonced dynamic_ggt& other) noex
    {
#if ENV_CPP >= 17
        if_cmp (alloc_always_equal) ret copy_strategy_t::swap;
#endif // ENV_CPP >= 17
        if_cmp (alloc_prop_on_swap)
        {
            ENV_STD::swap(this->_get_alloc(), other._get_alloc());
            ret swap_strategy_t::swap;
        }

        let allocs_equal = this->_get_alloc() == other._get_alloc();
        ret allocs_equal ? swap_strategy_t::swap : swap_strategy_t::undefined;
    }


    CMP_GETTER(max, traits_t::max_size(this->_get_alloc()));


    cmp_obj bool static is_alloc_noex{
            noex(traits_t::allocate(declvall<alloc_t>(), declvalr<size_t>()))};

    fun inl alloc(size_t size = single) noex(is_alloc_noex)
    {
        ret traits_t::allocate(this->_get_alloc(), size);
    }

    cmp_obj bool static is_alloc_at_noex{
            noex(traits_t::allocate(declvall<alloc_t>(), declvalr<size_t>(), declval<c_void_ptr_t>()))};

    fun inl alloc(c_void_ptr_t at, size_t size = single) noex(is_alloc_at_noex)
    {
        ret traits_t::allocate(this->_get_alloc(), size, at);
    }

    cmp_obj bool static is_free_noex{
            noex(traits_t::deallocate(declvall<alloc_t>(), declvalr<ptr_t>(), declvalr<size_t>()))};

    callb inl free(ptr_t at, size_t size = single) noex(is_free_noex)
    {
        traits_t::deallocate(this->_get_alloc(), at, size);
    }


    tmp<name T, name... TArgs>
    cmp_obj bool static is_place_noex_v{
            noex(traits_t::construct(declvall<alloc_t>(), declvalr<T*>(), declval<TArgs>()...))};

    tmp<name T, name... TVar>
    callb inl place(T* at, TVar&& ...args) noex(is_place_noex_v<T, TVar...>)
    {
        traits_t::construct(this->_get_alloc(), at, ENV_STD::forward<TVar>(args)...);
    }

    tmp<name T>
    cmp_obj bool static is_rem_noex_g{
            noex(traits_t::destroy(declvall<alloc_t>(), declvalr<T*>()))};

    tmp<name T>
    callb inl rem(T* at) noex(is_rem_noex_g<T>)
    {
        traits_t::destroy(this->_get_alloc(), at);
    }


private:
    cmp_obj static bool _is_dynamic{true};

public:
    ELABORATE_COND_COMPAT
    (
            (T::_is_dynamic && is_hashable_g < name T::data_t >),
            (T::_is_dynamic && are_equatable_g < name T::data_t, data_t >),
            (T::_is_dynamic && are_comparable_g < name T::data_t, data_t >)
    );

    CMP_VALIDITY { ret this->_get_data() != nil; }
    CMP_TMP_HASH { ret hash(this->_get_data()); }
    CMP_TMP_EQUALITY { ret this->_get_data() == rhs._get_data(); }
    CMP_TMP_COMPARISON { ret this->_get_data() < rhs._get_data(); }
};


ENV_STD_BEGIN

tmp<name TData, name TAlloc>
strct hash<ENV::dynamic_ggt<TData, TAlloc>>
{
    cmp_fn op()(const ENV::dynamic_ggt<TData, TAlloc>& subject) const noex { ret subject.hash(); }
};

ENV_STD_END


tmp<name TElement, name TAlloc = allocator_gt <TElement>>
typ(dynamic_gt) = dynamic_ggt<TElement*, name ENV_STD::allocator_traits<TAlloc>::tmp rebind_alloc<TElement>>;

typ(dynamic_t) = dynamic_gt<byte_t>;


TEST_CASE("dynamic")
{
    REQUIRE_EQ(ENV_STD::is_trivial_v < allocator_t >, ENV_STD::is_trivial_v < dynamic_t >);

    SUBCASE("pair")
    {
        obj dynamic_gt<ENV_STD::pair<int, int>> dynamic;
        dynamic.get_data() = dynamic.alloc(dual);
        dynamic.place(dynamic.get_data(), 1, 2);
        dynamic.place(dynamic.get_data() + 1, 2, 3);

        REQUIRE_EQ(dynamic.get_data()[0], ENV_STD::pair{1, 2});
        REQUIRE_EQ(dynamic.get_data()[1], ENV_STD::pair{2, 3});

        dynamic.rem(dynamic.get_data() + 1);
        dynamic.rem(dynamic.get_data());
        dynamic.free(dynamic.get_data(), dual);
    }

    SUBCASE("byte")
    {
        obj dynamic_t dynamic;
        dynamic.get_data() = dynamic.alloc(dual);
        dynamic.place(dynamic.get_data(), scast<byte_t>(0));
        dynamic.place(dynamic.get_data() + 1, scast<byte_t>(1));

        REQUIRE_EQ(dynamic.get_data()[0], byte_t{0});
        REQUIRE_EQ(dynamic.get_data()[1], byte_t{1});

        dynamic.rem(dynamic.get_data() + 1);
        dynamic.rem(dynamic.get_data());
        dynamic.free(dynamic.get_data(), dual);
    }

    SUBCASE("int range")
    {
        typ(alloc_t) = ENV::allocator_gt<int>;
        typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

        typ(iter_t) = traits_t::pointer;


        obj dynamic_ggt<range_gt < iter_t>, alloc_t > dynamic;
        dynamic.get_data().get_begin() = dynamic.alloc(11_s);
        dynamic.get_data().get_end() = &dynamic.get_data().get_begin()[10];
        for (auto i = dynamic.get_data().begin() ; i != dynamic.get_data().end() ; i++) dynamic.place(i, 0);

        ENV_STD::fill(dynamic.get_data().begin(), dynamic.get_data().end(), 1);
        let ones = ENV_STD::all_of(dynamic.get_data().begin(), dynamic.get_data().end(), [](auto i) { ret i == 1; });
        REQUIRE(ones);

        for (auto i = dynamic.get_data().begin() ; i != dynamic.get_data().end() ; i++) dynamic.rem(i);
        dynamic.free(dynamic.get_data().get_begin(), 11_s);
    }
}


#endif // ENV_DYNAMIC_HPP
