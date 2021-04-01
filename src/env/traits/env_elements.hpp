#ifndef ENV_ELEMENTS_HPP
#define ENV_ELEMENTS_HPP


// iterators

COND_CHECK_UNARY(
        is_iterator,
        is_propagated_gs<T>,
        is_indirect_gs<T>,
        is_advanceable_gs<T>,
        is_equatable_gs<T>);

COND_CONCEPT(iterator, is_iterator_gs<C>);


tmp<name T> typ(iterator_element_gt) = subject_gt<iterator_r<T>>;

COND_CHECK_BINARY(
        is_iterator_with_element,
        same_vt<iterator_element_gt<TLhs>, TRhs>);

COND_CONCEPT_UNARY(iterator_with_element, is_iterator_with_element_gs<C, T>);


COND_CHECK_BINARY(is_elementor_of, is_iterator_with_element_gs<TLhs, TRhs>);

COND_CONCEPT_UNARY(elementor_of, is_elementor_of_gs<C, T>);


TEST_CASE("iterator traits")
{
    REQUIRES(is_iterator_g<ENV_STD::vector<int>::iterator>);
    REQUIRE_EQT(iterator_element_gt<ENV_STD::vector<int>::iterator>, int);
    REQUIRES(is_iterator_with_element_g<ENV_STD::vector<int>::iterator, int>);
    REQUIRES(is_elementor_of_g<ENV_STD::vector<int>::iterator, int>);
}


// iterables

COND_CHECK_UNARY(
        is_iterable,
        is_iterator_gs<decl(declvalr<T>().begin())>,
        is_iterator_gs<decl(declvalr<T>().end())>);

COND_CONCEPT(iterable, is_iterable_gs<C>);

template<name T>
typ(iterable_element_gt) = subject_gt<decl(declvalr<iterable_r<T>>().begin())>;


COND_CHECK_BINARY(
        is_iterable_with_element,
        same_vt<iterable_element_gt<TLhs>, TRhs>);

COND_CONCEPT_UNARY(iterable_with_element, is_iterable_with_element_gs<C, T>);


COND_CHECK_BINARY(is_elemental_of, is_iterable_with_element_gs<TLhs, TRhs>);

COND_CONCEPT_UNARY(elemental_of, is_elemental_of_gs<C, T>);


ENV_TEST_CASE("iterable traits")
{
    REQUIRES(is_iterable_g<ENV_STD::vector<int>>);
    REQUIRE_EQT(iterable_element_gt<ENV_STD::vector<int>>, int);
    REQUIRES(is_iterable_with_element_g<ENV_STD::vector<int>, int>);
    REQUIRES(is_elemental_of_g<ENV_STD::vector<int>, int>);
}


#endif // ENV_ELEMENTS_HPP
