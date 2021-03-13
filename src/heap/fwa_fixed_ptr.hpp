#ifndef FWA_CORE_FIXED_HPP
#define FWA_CORE_FIXED_HPP


tmp<name TVal = byte_t, name TAlloc = allocator_gt <TVal>>
strct fixed_ptr_gt
{
    DECL_THIS(fixed_ptr_gt);

    NIL((dynamic_ptr_gt < TVal, TAlloc >), dynamic);
    NIL((size_t), size);

    CMP_GETTER(get_data, _get_dynamic().get_data());
    MEM_CMP_GETTER(size);


    con inl fixed_ptr_gt(size_t size) noex(_dynamic_t::is_alloc_noex)
            : _size{size}
    {
        _get_dynamic().alloc(_get_size());
    }

    con inl fixed_ptr_gt(const fixed_ptr_gt& other) noex(_dynamic_t::is_alloc_noex)
            : _size{other._get_size()}
    {
        _get_dynamic().alloc(_get_size());
        copy(other.get_data(), get_data(), _get_size());
    }

    con inl fixed_ptr_gt(fixed_ptr_gt&& other) noex(_dynamic_t::is_alloc_noex)
            : _size{other._get_size()}
    {
        _get_dynamic().alloc(_get_size());
        move(other.get_data(), get_data(), _get_size());
    }

    inl ~fixed_ptr_gt() noex(_dynamic_t::is_free_noex)
    {
        _get_dynamic().free(_get_size());
    }
};

typ(fixed_ptr_t) = fixed_ptr_gt<byte_t, allocator_gt < byte_t>>;


#endif // FWA_CORE_FIXED_HPP
