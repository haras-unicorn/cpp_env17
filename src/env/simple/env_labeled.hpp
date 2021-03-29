#ifndef ENV_LABELED_HPP
#define ENV_LABELED_HPP


tmp<name T, const c_t* Label>
strct labeled_ngt : public T
{
    typ(label_t) = const c_t*;
    cmp_obj static label_t label{Label};

    DECL_BASE(labeled, (T));
    using _labeled_base_t::_labeled_base_t;
};


#define LABEL(_name, ...)                                      \
        ENV_DETAIL_BEGIN                                       \
        cmp_obj static const c_t _name[]{STRING(__VA_ARGS__)}; \
        ENV_DETAIL_END                                         \
        SEMI


LABEL(test_label, ayy lmao);

ENV_TEST_CASE("labeled")
{
    REQUIRES(size_of_g<labeled_ngt<unitary_gt<int>, detail::test_label>> ==
             size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v <
             labeled_ngt<unitary_gt<int>, detail::test_label>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             labeled_ngt<unitary_gt<int>, detail::test_label>>);
}


#endif // ENV_LABELED_HPP
