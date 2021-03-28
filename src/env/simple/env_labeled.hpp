#ifndef ENV_LABELED_HPP
#define ENV_LABELED_HPP


tmp<name T, const c_t* Label>
strct labeled_ngt : public unitary_gt<T>
{
    typ(label_t) = const c_t*;
    cmp_obj static label_t label{Label};

    using name unitary_gt<T>::value_t;
};

ENV_TEST_CASE("labeled")
{
    obj cmp static const c_t _label[]{"ayy"};
    REQUIRES(size_of_g<labeled_ngt<int, _label>> == size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v < labeled_ngt<int, _label >>);
    REQUIRES(ENV_STD::is_standard_layout_v < labeled_ngt<int, _label >>);
}


#endif // ENV_LABELED_HPP
