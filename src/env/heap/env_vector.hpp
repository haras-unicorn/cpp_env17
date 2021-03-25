#ifndef ENV_VECTOR_HPP
#define ENV_VECTOR_HPP


ENV_TEST_BEGIN

strct alloc_getter_t
{
    tmp<name TVec>
    cmp_fn static get(const TVec& vec) noex
    {
        ret vec._get_alloc();
    }
};

tmp<name TVec>
cmp_fn get_alloc(const TVec& vec) noex
{
    ret alloc_getter_t::get(vec);
}

strct flex_getter_t
{
    tmp<name TVec>
    cmp_fn static get(const TVec& vec) noex
    {
        ret vec._get_flex();
    }
};

tmp<name TVec>
cmp_fn get_flex(const TVec& vec) noex
{
    ret flex_getter_t::get(vec);
}

ENV_TEST_END


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

    con vector_gt(size_t size) :
            _dynamic{ }
    {
        _construct(size);
    }

    con vector_gt(const vector_gt& other) :
            _dynamic{other._get_dynamic()}
    {
        _alloc(other.size());
        _ucopy(other);
    }

    con vector_gt(vector_gt&& other) noex:
            _dynamic{ENV_STD::move(other._get_dynamic())}
    {
        _move(ENV_STD::move(other));
    }

    asgn vector_gt& op=(const vector_gt& other)
    {
        if (_was_invalidated_by(other))
        {
            _get_dynamic().copy(other._get_dynamic());
            ret *this;
        }

        if_cmp (dynamic_t::always_copies)
        {
            _get_dynamic().copy(other._get_dynamic());
            _copy(other);
        }
        else
        {
            if (!_get_dynamic().alloc_equal_to(other._get_dynamic()))
            {
                _realloc_copy(other);
            }
            else
            {
                _get_dynamic().copy(other._get_dynamic());
                _copy(other);
            }
        }

        ret *this;
    }

    asgn vector_gt& op=(vector_gt&& other)
    {
        if (_was_invalidated_by(other))
        {
            _get_dynamic().move(ENV_STD::move(other._get_dynamic()));
            ret *this;
        }

        _move(ENV_STD::move(other),
              _get_dynamic().move_strategy(
                      ENV_STD::move(other._get_dynamic())));

        ret *this;
    }

    fun swap(vector_gt& other)
    {
        _swap(other, _get_dynamic().swap_strategy(other._get_dynamic()));
    }

    dest ~vector_gt()
    {
        _free();
    }


    COMPAT(is_vector);
    CMP_VALIDITY { ret ENV::is_valid(_get_dynamic()); }
    CMP_HASH { ret ENV::hash(_get_dynamic()); }
    CMP_EQUALITY { ret _get_dynamic() == rhs._get_dynamic(); }
    CMP_COMPARISON { ret _get_dynamic() < rhs._get_dynamic(); }


    // lifetime

protected:
    callb inl _copy(
            const vector_gt& other,
            nonced name dynamic_t::copy_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_copies) _copy(other);
        else
        {
            if (strategy == dynamic_t::copy_strategy_t::copy)
                _copy(other);
            else
                _realloc_copy(other);
        }
    }

    callb inl _move(
            vector_gt&& other,
            nonced name dynamic_t::move_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_moves) _move(ENV_STD::move(other));
        else
        {
            if (strategy == dynamic_t::move_strategy_t::move)
                _move(ENV_STD::move(other));
            else
                _move_elements(ENV_STD::move(other));
        }
    }

    callb inl _swap(
            vector_gt& other,
            nonced name dynamic_t::swap_strategy_t strategy)
    {
        if_cmp(dynamic_t::always_swaps) _swap(other);
        else
        {
            if (strategy == dynamic_t::swap_strategy_t::swap)
                _swap(other);
            else
                throw unequal_container_allocator_swap_error_t
                        {
                                "Don't use vector swap with allocators "
                                "that are not always equal"
                        };
        }
    }


    callb inl _ucopy(const vector_gt& other)
    noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        ret ucopy(_get_alloc(),
                  other._get_begin(), other._get_end(),
                  _get_begin());
    }

    callb inl _copy(const vector_gt& other)
    noex(ENV_STD::is_nothrow_copy_assignable_v<TVal>)
    {
        _resize(other.size());
        ret copy(other._get_begin(), other._get_end(), _get_begin());
    }

    callb inl _realloc_copy(const vector_gt& other)
    {
        _free();
        _get_dynamic().copy(other._get_dynamic());
        _alloc(other.size());
        ret _ucopy(other);
    }

    callb inl _move(vector_gt&& other) noex
    {
        _get_flex() = ENV_STD::move(other._get_flex());
        ret _get_end();
    }

    callb inl _move_elements(vector_gt&& other)
    noex(ENV_STD::is_nothrow_move_assignable_v<TVal>)
    {
        _resize(other.size());
        ret move(other._get_begin(), other._get_end(), _get_begin());
    }

    callb inl _swap(vector_gt& other) noex
    {
        ENV_STD::swap(_get_flex(), other._get_flex());
        ret _get_end();
    }


    callb inl _resize(size_t to)
    {
        if (to <= size())
        {
            _get_end() = _get_begin() + to;
            ret _get_end();
        }
        if (to <= capacity())
        {
            _get_end() = emplace(_get_alloc(), _get_end(), _get_begin() + to);
            ret _get_end();
        }
        ret _expand(to);
    }

    callb inl _expand(size_t to)
    {
        let _old{_get_flex()};
        _alloc(to);

        _get_end() = ucopy(
                _get_alloc(),
                _old.get_begin(), _old.get_end(),
                _get_begin());
        _get_dynamic().free(_old.get_begin(), _old.capacity());

        _get_end() = emplace(_get_alloc(), _get_end(), _get_begin() + to);
        ret _get_end();
    }


    fun inl _was_invalidated_by(const vector_gt& other)
    {
        if (ENV::is_invalid(other))
        {
            _close();
            ret true;
        }

        ret false;
    }

    callb inl _close()
    {
        _free();
        _invalidate();
    }


    callb inl _construct(size_t _size)
    {
        let _cap = _capacity(_size);

        _get_begin() = _get_dynamic().alloc(_cap);
        _get_end() = emplace(_get_alloc(), _get_begin(), _get_begin() + _size);
        _get_last() = _get_begin() + _cap;

        ret _get_end();
    }

    callb inl _alloc(size_t size)
    {
        let _cap = _capacity(size);

        _get_begin() = _get_dynamic().alloc(_cap);
        _get_end() = _get_begin();
        _get_last() = _get_begin() + _cap;

        ret _get_last();
    }

    callb inl _free()
    {
        _get_dynamic().free(_get_begin(), capacity());
    }

    callb inl _invalidate()
    {
        _get_begin() = _get_end() = _get_last() = nil;
    }


    cmp_fn static _capacity(size_t _size) noex -> size_t
    {
        ret scast<size_t>(next_pow2(_size));
    }


    // getters

protected:
    GETTER(_get_alloc, _get_dynamic().get_alloc());

    GETTER(_get_flex, _get_dynamic().get_data());

    GETTER(_get_range, _get_flex().get_range());
    GETTER(_get_last, _get_flex().get_last());

    GETTER(_get_begin, _get_range().get_begin());
    GETTER(_get_end, _get_range().get_end());


    friend struct test::alloc_getter_t;
    friend struct test::flex_getter_t;
};



ENV_TEST_CASE("vector")
{
    SUBCASE("nil")
    {
        typ(_val_t) = int;
        typ(_alloc_t) = test::id_allocator_gt<_val_t>;

        vector_gt<_val_t, _alloc_t> def{ };
        REQUIRE(is_invalid(def));

        mut copied{def};
        REQUIRE(is_invalid(copied));
        REQUIRE_EQ(test::get_alloc(def), test::get_alloc(copied));
        REQUIRE_EQ(id(test::get_alloc(def)), id(test::get_alloc(copied)));

        mut moved{ENV_STD::move(def)};
        REQUIRE(is_invalid(moved));
        REQUIRE_EQ(test::get_alloc(def), test::get_alloc(moved));
        REQUIRE_EQ(id(test::get_alloc(def)), id(test::get_alloc(moved)));

        vector_gt<_val_t, _alloc_t> move_assigned{ };
        move_assigned = ENV_STD::move(moved);
        REQUIRE(is_invalid(move_assigned));
        REQUIRE_NE(test::get_alloc(moved), test::get_alloc(move_assigned));
        REQUIRE_NE(id(test::get_alloc(moved)),
                   id(test::get_alloc(move_assigned)));

        REQUIRE_THROWS(copied.swap(move_assigned));
        let check_invalid = is_invalid(copied) && is_invalid(move_assigned);
        REQUIRE(check_invalid);
        REQUIRE_NE(test::get_alloc(move_assigned), test::get_alloc(copied));
        REQUIRE_NE(id(test::get_alloc(move_assigned)),
                   id(test::get_alloc(copied)));

        vector_gt<_val_t, _alloc_t> copy_assigned{ };
        copy_assigned = copied;
        REQUIRE(is_invalid(copy_assigned));
        REQUIRE_EQ(test::get_alloc(copied), test::get_alloc(copy_assigned));
        REQUIRE_EQ(id(test::get_alloc(copied)),
                   id(test::get_alloc(copy_assigned)));
    }

    SUBCASE("non nil")
    {
        typ(_val_t) = int;
        typ(_alloc_t) = test::id_allocator_gt<_val_t>;

        vector_gt<_val_t, _alloc_t> ten{10_s};
        REQUIRE_EQ(ten.size(), 10_s);
        REQUIRE_EQ(ten.capacity(), 16_s);
        REQUIRE(is_valid(ten));

//        mut copied{ten};
//        REQUIRE(is_invalid(copied));
//        REQUIRE_EQ(test::get_alloc(ten), test::get_alloc(copied));
//        REQUIRE_EQ(id(test::get_alloc(ten)), id(test::get_alloc(copied)));
//
//        mut moved{ENV_STD::move(ten)};
//        REQUIRE(is_invalid(moved));
//        REQUIRE_EQ(test::get_alloc(ten), test::get_alloc(moved));
//        REQUIRE_EQ(id(test::get_alloc(ten)), id(test::get_alloc(moved)));
//
//        vector_gt<_val_t, _alloc_t> move_assigned{ };
//        move_assigned = ENV_STD::move(moved);
//        REQUIRE(is_invalid(move_assigned));
//        REQUIRE_NE(test::get_alloc(moved), test::get_alloc(move_assigned));
//        REQUIRE_NE(id(test::get_alloc(moved)),
//                   id(test::get_alloc(move_assigned)));
//
//        REQUIRE_THROWS(copied.swap(move_assigned));
//        let check_invalid = is_invalid(copied) && is_invalid(move_assigned);
//        REQUIRE(check_invalid);
//        REQUIRE_NE(test::get_alloc(move_assigned), test::get_alloc(copied));
//        REQUIRE_NE(id(test::get_alloc(move_assigned)),
//                   id(test::get_alloc(copied)));
//
//        vector_gt<_val_t, _alloc_t> copy_assigned{ };
//        copy_assigned = copied;
//        REQUIRE(is_invalid(copy_assigned));
//        REQUIRE_EQ(test::get_alloc(copied), test::get_alloc(copy_assigned));
//        REQUIRE_EQ(id(test::get_alloc(copied)),
//                   id(test::get_alloc(copy_assigned)));
    }
}


#endif //ENV_VECTOR_HPP
