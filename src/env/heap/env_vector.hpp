#ifndef ENV_VECTOR_HPP
#define ENV_VECTOR_HPP


// I started writing it to test the dynamic struct, but it seems like it turned out to be an ok vector class.

COND_TMP
(
        (name TVal, name TAlloc = allocator_gt < TVal >),
        (is_stable_g < TVal > && detail::is_dynamic_alloc_g < TAlloc > )
)
strct vector_gt
{
private:
    DECL_THIS(vector_gt);


    typ(val_t) = TVal;

    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(iter_t) = name traits_t::pointer;
    typ(range_t) = range_gt<iter_t>;
    typ(flex_t) = flex_gt<iter_t>;


    typ(dynamic_t) = dynamic_ggt <flex_gt<iter_t>, alloc_t>;
    NIL((dynamic_t), dynamic);


public:
    CMP_GETTER(begin, _get_begin());
    CMP_GETTER(end, _get_end());

    CMP_GETTER(size, _get_flex().size());
    CMP_GETTER(capacity, _get_flex().capacity());


    imp cmp inl vector_gt() noex = default;

    con vector_gt(size_t size) : _dynamic{ } { _construct(size); }

    con vector_gt(const vector_gt& other) : _dynamic{other._get_dynamic()}
    {
        _alloc(other.size()), _ucopy(other);
    }

    con vector_gt(vector_gt&& other) noex: _dynamic{ENV_STD::move(other._get_dynamic())}
    {
        _move(ENV_STD::move(other));
    }

    fun operator=(const vector_gt& other)
    {
        if (_did_invalidate(other)) ret;
        _copy(other, _get_dynamic().copy_strategy(other._get_dynamic()));
    }

    fun operator=(vector_gt&& other)
    {
        if (_did_invalidate(other)) ret;
        _move(ENV_STD::move(other), _get_dynamic().move_strategy(ENV_STD::move(other._get_dynamic())));
    }

    fun swap(vector_gt& other) { _swap(other, _get_dynamic().swap_strategy(other._get_dynamic())); }

    dest ~vector_gt() { _free(); }


    COMPAT(is_vector);
    CMP_VALIDITY { ret ENV::is_valid(_get_dynamic()); }
    CMP_HASH { ret ENV::hash(_get_dynamic()); }
    CMP_EQUALITY { ret _get_dynamic() == rhs._get_dynamic(); }
    CMP_COMPARISON { ret _get_dynamic() < rhs._get_dynamic(); }


protected:
    callb inl _copy(const vector_gt& other, nonced name dynamic_t::copy_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_copies) _copy(other);
        else
        {
            if (strategy == dynamic_t::copy_strategy_t::copy) _copy(other);
            else _realloc_copy(other);
        }
    }

    callb inl _move(vector_gt&& other, nonced name dynamic_t::move_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_moves) _move(other);
        else
        {
            if (strategy == dynamic_t::move_strategy_t::move) _move(ENV_STD::move(other));
            else _move_elements(ENV_STD::move(other));
        }
    }

    callb inl _swap(vector_gt& other, nonced name dynamic_t::swap_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_swaps) _swap(other);
        else
        {
            if (strategy == dynamic_t::swap_strategy_t::swap)
                _swap(other);
            else
                throw unequal_container_allocator_swap_error_t
                        {"Don't use vector swap with allocators that are not always equal"};
        }
    }


    callb inl _ucopy(const vector_gt& other) noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        ret ucopy(_get_alloc(), other._get_begin(), other._get_end(), _get_begin());
    }

    callb inl _copy(const vector_gt& other) noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        ret _resize(other.size()), copy(other._get_begin(), other._get_end(), _get_begin());
    }

    callb inl _realloc_copy(const vector_gt& other)
    {
        ret _realloc(other._get_dynamic(), other.size()), _ucopy(other);
    }

    callb inl _move(vector_gt&& other) noex
    {
        ret (_get_flex() = ENV_STD::move(other._get_flex())), _get_end();
    }

    callb inl _move_elements(vector_gt&& other) noex(ENV_STD::is_nothrow_move_assignable_v<TVal>)
    {
        ret _resize(other.size()), move(other._get_begin(), other._get_end(), _get_begin());
    }

    callb inl _swap(vector_gt& other) noex
    {
        ret ENV_STD::swap(_get_flex(), other._get_flex()), _get_end();
    }


    callb inl _resize(size_t to)
    {
        ret to <= size() ? (_get_end() = _get_begin() + to) :
            to <= capacity() ? (_get_end() = emplace(_get_dynamic(), _get_begin(), _get_begin() + to)) :
            _expand(_get_dynamic(), to);
    }

    callb inl _expand(size_t to)
    {
        obj const range_t old_range{_get_begin(), _get_end()};

        _alloc(to);

        _get_end() = ucopy(_get_alloc(), old_range.get_begin(), old_range.get_end(), _get_begin());
        _get_dynamic().free(old_range.get_begin(), old_range.size());

        ret _get_end() = emplace(_get_alloc(), _get_end(), _get_end() + (to - old_range.size()));
    }

    callb inl _realloc(dynamic_t& with, size_t to) { ret _free(), _alloc(with, to); }


    fun inl _did_invalidate(const vector_gt& other) { ret ENV::is_invalid(other) ? (_close(), true) : false; }

    callb inl _close() { _free(), _invalidate(); }


    callb inl _construct(dynamic_t& with, size_t size)
    {
        _get_begin() = with.alloc(size_t{size});
        ret _get_last() = _get_end() = emplace(with.get_alloc(), _get_begin(), _get_begin() + size);
    }

    callb inl _construct(size_t size) { ret _construct(_get_dynamic(), size); }

    callb inl _alloc(dynamic_t& with, size_t size)
    {
        ret _get_last() = size + (_get_end() = _get_begin() = with.alloc(size_t{size}));
    }

    callb inl _alloc(size_t size) { ret _alloc(_get_dynamic(), size); }

    callb inl _free(dynamic_t& with) { with.free(_get_begin(), size()); }

    callb inl _free() { _free(_get_dynamic()); }

    callb inl _invalidate() { _get_begin() = _get_end() = _get_last() = nil; }


    GETTER(_get_alloc, _get_dynamic().get_alloc());

    GETTER(_get_flex, _get_dynamic().get_data());

    GETTER(_get_range, _get_flex().get_range());
    GETTER(_get_last, _get_flex().get_last());

    GETTER(_get_begin, _get_flex().get_begin());
    GETTER(_get_end, _get_flex().get_end());
};


ENV_TEST_CASE("vector")
{
    SUBCASE("nil")
    {
        vector_gt<int> def{ };
        mut copied{def};
        mut moved{ENV_STD::move(def)};
        mut copy_assigned{copied};
        mut move_assigned{ENV_STD::move(moved)};
        copied.swap(copy_assigned);
        nonce(move_assigned);
    }
}


#endif //ENV_VECTOR_HPP
