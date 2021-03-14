#ifndef ENV_ARRAY_HPP
#define ENV_ARRAY_HPP

tmp<size_t SizeHead, size_t... SizeRest>
strct array_vs
{
    cmp_obj static rank_t rank = rank_t{1 + sizeof...(SizeRest)};

    tmp<name T> typ(value_gt) = name array_vs<SizeRest...>::tmp array_gt<T>;

    tmp<name T> typ(array_gt) = value_gt<T>[SizeHead];
    tmp<name T> typ(ptr_gt) = ptr_gt<value_gt<T>>;

    tmp<name T> typ(array_cgt) = array_gt<add_const_gt < T>>;
    tmp<name T> typ(array_mgt) = array_gt<remove_const_gt < T>>;
};

tmp<size_t Size>
strct array_vs<Size>
{
    cmp_obj static rank_t rank = rank_one;

    tmp<name T> typ(value_gt) = T;

    tmp<name T> typ(array_gt) = value_gt<T>[Size];
    tmp<name T> typ(ptr_gt) = ptr_gt<value_gt<T>>;

    tmp<name T> typ(array_cgt) = array_gt<add_const_gt < T>>;
    tmp<name T> typ(array_mgt) = array_gt<remove_const_gt < T>>;
};

tmp<size_t Size, name T> using array_ngt = name array_vs<Size>::tmp array_gt<T>;

tmp<size_t Size, name T> typ(array_value_ngt) = name array_vs<Size>::tmp value_gt<T>;

tmp<size_t Size, name T> typ(array_ncgt) = array_ngt<Size, ENV_STD::add_const_t<T>>;
tmp<size_t Size, name T> typ(array_nmgt) = array_ngt<Size, ENV_STD::remove_const_t<T>>;

ENV_TEST_CASE("array traits")
{
    REQUIRES(ENV_STD::is_same_v < array_ngt<triple, int>, int[3] >);
    REQUIRES(ENV_STD::is_same_v < array_vs<triple, dual>::array_gt<int>, int[3][2] >);
    REQUIRES(ENV_STD::is_same_v < array_vs<triple, dual>::value_gt<int>, int[2] >);
}

#endif // ENV_ARRAY_HPP
