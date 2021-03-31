#ifndef ENV_FIELDS_HPP
#define ENV_FIELDS_HPP


tmp<name... T> typ(fields_vt) = ENV_HANA::tuple<T...>;

#define FIELDS(...) \
        DECL((fields_vt<__VA_ARGS__>), fields)

#define FIELD(_name, _index) \
        GETTER(GETTER_NAME(_name), _get_fields()[ENV_HANA::ullong_c<_index>])

#define _FIELD(_name, _index) \
        GETTER(_GETTER_NAME(_name), _get_fields()[ENV_HANA::ullong_c<_index>])


tmp<name T>
strct fields_equal_gt
{
    CRTP_INHERITOR(fields_equal_gt, (T));

    cmp_fn friend op==(const T& _lhs, const T& _rhs) noex
    {
        ret _lhs._get_fields() == _rhs._get_fields();
    }
};


ENV_TEST_CASE("fields")
{
    strct vector_t : fields_equal_gt<vector_t>
    {
        CRTP_BASE(equals, (fields_equal_gt<vector_t>));

        FIELDS(empty_t, int, int, int);
        FIELD(x, 1);
        FIELD(y, 2);
        FIELD(z, 3);

        con cmp inl vector_t(int x, int y, int z) : _fields{{ }, x, y, z} { }
        DEFAULT_LIFE(vector_t, CMP);
    };

    REQUIRES(size_of_g < vector_t > == size_of_g < int > *3);
    REQUIRES(ENV_STD::is_trivially_copyable_v < vector_t >);

    cmp vector_t vec{1, 2, 3};
    REQUIRES(vec.get_x() == 1);
    REQUIRES(vec.get_y() == 2);
    REQUIRES(vec.get_z() == 3);

    cmp vector_t vec_equal{1, 2, 3};
    cmp vector_t vec_unequal{1, 5, 3};
    REQUIRES(vec == vec_equal);
    REQUIRES_FALSE(vec == vec_unequal);
}


#endif // ENV_FIELDS_HPP
