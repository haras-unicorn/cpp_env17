#ifndef ENV_VECTOR_HPP
#define ENV_VECTOR_HPP


// I started writing it to test the dynamic struct, but it seems like it turned out to be an ok vector class.

tmp<name TVal, name TAlloc = allocator_gt <TVal>>
strct vector_gt
{
private:
    DECL_THIS(vector_gt);


    typ(val_t) = TVal;

    typ(alloc_t) = TAlloc;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    typ(iter_t) = name traits_t::pointer;


    typ(dynamic_t) = dynamic_ggt <range_gt<iter_t>, alloc_t>;
    NIL((dynamic_t), dynamic);


public:
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


    fun inl _ucopy(const vector_gt& other) noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        ucopy(other._get_begin(), _get_begin(), ENV_STD::min(size(), other.size()));
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

    fun inl _umove_elements(const vector_gt& other) noex(ENV_STD::is_nothrow_move_assignable_v<TVal>)
    {
        umove(other._get_begin(), _get_begin(), ENV_STD::min(size(), other.size()));
        other._free();
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


#endif //ENV_VECTOR_HPP
