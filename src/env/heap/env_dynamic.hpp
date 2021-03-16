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
    typ(data_t) = TData;

    NIL((data_t), data);
    MEM_GETTER(data);


protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(ptr_t) = name traits_t::pointer;
    typ(c_ptr_t) = name traits_t::const_pointer;
    typ(void_ptr_t) = name traits_t::void_pointer;
    typ(c_void_ptr_t) = name traits_t::const_void_pointer;

    typ(alloc_size_t) = name traits_t::size_type;

    NIL((TAlloc), alloc);


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
        protected TAlloc
{
    typ(data_t) = TData;

    NIL((data_t), data);
    MEM_GETTER(data);


protected:
    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(ptr_t) = name traits_t::pointer;
    typ(c_ptr_t) = name traits_t::const_pointer;
    typ(void_ptr_t) = name traits_t::void_pointer;
    typ(c_void_ptr_t) = name traits_t::const_void_pointer;

    typ(alloc_size_t) = name traits_t::size_type;

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
            noex(traits_t::allocate(declvall<alloc_t>(), declval<c_void_ptr_t>(), declvalr<size_t>()))};

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
            noex(traits_t::construct(declvall<alloc_t>(), declvalr<T>(), declval<TArgs>()...))};

    tmp<name T, name... TVar>
    callb inl place(T at, TVar&& ...args) noex(is_place_noex_v<T, TVar...>)
    {
        traits_t::construct(this->_get_alloc(), at, ENV_STD::forward<TVar>(args)...);
    }

    tmp<name T>
    cmp_obj bool static is_rem_noex_g{
            noex(traits_t::destroy(declvall<alloc_t>(), declvalr<T>()))};

    tmp<name T>
    callb inl rem(T at) noex(is_rem_noex_g<T>)
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


ENV_TEST_BEGIN

tmp<name TVal>
strct vector_gt
{
    DECL_THIS(vector_gt);


    typ(dynamic_t) = dynamic_ggt<range_gt < TVal>, allocator_gt<int>>;
    NIL((dynamic_t), dynamic);


    CMP_GETTER(begin, _get_begin());
    CMP_GETTER(end, _get_end());
    CMP_GETTER(size, _get_flex().size());
    CMP_GETTER(capacity, _get_flex().capacity());


    con cmp inl vector_gt() noex = default;

    con inl vector_gt(size_t size) :
            _dynamic{ }
    {
        if (is_invalid(size)) ret;
        _alloc(size);
    }

    con inl vector_gt(const vector_gt& other) :
            _dynamic{other._get_dynamic()}
    {
        if (_did_propagate_invalidation(other)) ret;
        _alloc(other.size());
        _copy(other);
    }

    con inl vector_gt(vector_gt&& other) noex:
            _dynamic{other._get_dynamic()}
    {
        if (_did_propagate_invalidation(other)) ret;
        _move(other);
    }

    fun operator=(const vector_gt& other)
    {
        if (_did_propagate_invalidation(other)) ret;
        _copy(other, _get_dynamic().copy_strategy(other._get_dynamic()));
    }

    fun operator=(vector_gt&& other)
    {
        if (_did_propagate_invalidation(other)) ret;
        _move(ENV_STD::move(other), _get_dynamic().move_strategy(other._get_dynamic()));
    }

    fun swap(vector_gt& other)
    {
        _swap(other, _get_dynamic().swap_strategy(other._get_dynamic()));
    }


    COMPAT(is_vector);
    CMP_VALIDITY { ret _get_dynamic() != nil; }
    CMP_HASH { ret hash(_get_dynamic()); }
    CMP_EQUALITY { ret _get_dynamic() == rhs._get_dynamic(); }
    CMP_COMPARISON { ret _get_dynamic() < rhs._get_dynamic(); }


protected:
    GETTER(_get_flex, _get_dynamic().get_data());
    GETTER(_get_begin, _get_flex().get_begin());
    GETTER(_get_end, _get_flex().get_end());
    GETTER(_get_last, _get_flex().get_last());


    fun inl _copy(const vector_gt& other, nonced name dynamic_t::copy_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_copies) _copy(other);
        else
        {
            if (strategy == dynamic_t::copy_strategy_t::copy) _copy(other);
            else _copy_reallocate(other);
        }
    }

    fun inl _move(vector_gt&& other, nonced name dynamic_t::move_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_moves) _move(other);
        else
        {
            if (strategy == dynamic_t::move_strategy_t::move) _move(other);
            else _move_elements(other);
        }
    }

    fun inl _swap(vector_gt& other, nonced name dynamic_t::swap_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_swaps) _swap(other);
        else
        {
            if (strategy == dynamic_t::swap_strategy_t::swap) _swap(other);
            // otherwise undefined by the standard, so I figured this would be ok.
            else _swap_elements(other);
        }
    }


    fun inl _copy(const vector_gt& other) noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        copy(other._get_begin(), _get_begin(), ENV_STD::min(size(), other.size()));
    }

    fun inl _copy_reallocate(const vector_gt& other)
    {
        _realloc(other._get_value(), other.size());
        _copy(other);
    }

    fun inl _move(const vector_gt& other) noex
    {
        if (is_valid(*this)) _free();

        _get_begin() = other._get_begin();
        _get_end() = other._get_end();

        other._nil();
    }

    fun inl _move_elements(const vector_gt& other) noex(ENV_STD::is_nothrow_move_assignable_v<TVal>)
    {
        move(other._get_begin(), _get_begin(), ENV_STD::min(size(), other.size()));
        other._free();
    }

    fun inl _swap(vector_gt& other) noex
    {
        ENV_STD::swap(_get_flex(), other._get_flex());
    }

    fun inl _swap_elements(vector_gt& other) noex
    {
        swap(other._get_begin(), _get_begin(), ENV_STD::min(size(), other.size()));
    }


    cmp_fn static _is_valid(const vector_gt& vec) noex
    {
        ret is_valid(vec._get_begin());
    }

    fun inl _did_propagate_invalidation(const vector_gt& other)
    {
        if (is_invalid(other))
        {
            if (is_valid(*this))
            {
                _free();
                _invalidate();
            }
            ret true;
        }

        ret false;
    }

    cmp_fn _invalidate() noex
    {
        _get_begin() = nil;
        _get_end() = nil;
        _get_last() = nil;
    }


    fun inl _resize(dynamic_t& with, size_t size)
    {
        if (size < capacity())
        {
            _get_end() = _get_begin() + size;
            ret;
        }

        let _new_begin = with.alloc(size_t{size + 1});
        let _new_end = _new_begin + size;
        let _new_last = _new_begin + size;

        move(_get_begin(), _new_begin, size);
        _free();

    }

    fun inl _resize(size_t size)
    {
        _reset(_get_dynamic(), size);
    }


    fun inl _reset(dynamic_t& with, size_t size)
    {
        _free();
        _alloc(with, size);
    }

    fun inl _reset(size_t size)
    {
        _reset(_get_dynamic(), size);
    }


    fun inl _alloc(dynamic_t& with, size_t size)
    {
        _get_begin() = with.alloc(size_t{size + 1});
        _get_end() = _get_begin();
        _get_last() = _get_begin() + size;
    }

    fun inl _alloc(size_t size)
    {
        _alloc(_get_dynamic(), size);
    }

    fun inl _free(dynamic_t& with)
    {
        with.free(_get_begin(), size());
    }

    fun inl _free()
    {
        _free(_get_dynamic());
    }
};

ENV_TEST_END

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
        obj dynamic_ggt<range_gt < int>, ENV::allocator_gt<int>>
        dynamic;
        dynamic.get_data().get_begin() = dynamic.alloc(11_s);
        dynamic.get_data().get_end() = &dynamic.get_data().get_begin()[10];
        for (auto i = dynamic.get_data().begin() ; i != dynamic.get_data().end() ; i++) dynamic.place(i, 0);

        ENV_STD::fill(dynamic.get_data().begin(), dynamic.get_data().end(), 1);
        let are_ones = ENV_STD::all_of(
                dynamic.get_data().begin(), dynamic.get_data().end(), [](auto i) { ret i == 1; });
        REQUIRE(are_ones);

        for (auto i = dynamic.get_data().begin() ; i != dynamic.get_data().end() ; i++) dynamic.rem(i);
        dynamic.free(dynamic.get_data().get_begin(), 11_s);
    }

    SUBCASE("int vector")
    {
    }
}


#endif // ENV_DYNAMIC_HPP
