#ifndef ENV_CONTAINER_STANDARD_REQUIREMENTS_HPP
#define ENV_CONTAINER_STANDARD_REQUIREMENTS_HPP


// notes: https://en.cppreference.com/w/cpp/named_req/Container
COND_CHECK_UNARY
(
        is_std_container,
        TYPE_COND(detail::container_traits_gt<T>) &&

        /* all of these have semantics, runtime conditions and complexity requirements which are impossible to check*/

        ENV_STD::is_same_v<decl(T()), T>&&
        ENV_STD::is_same_v<decl(T(declvall<const T>())), T>&&
        ENV_STD::is_same_v<decl(T(declvalr<T>())), T>&&

        implies
        (
                is_std_copy_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
                detail::is_std_copyable_container_g<T>
        ) &&
        ENV_STD::is_same_v<decl(declvall<T>() = declvalr<T>()), T&>&&
        ENV_STD::is_same_v<decl(declval<T>().~T()), void>&&

        implies
        (
                !ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<decl(declval<T>().begin()), name detail::container_traits_gt<T>::iterator_t>
        ) &&
        implies
        (
                ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<decl(declval<T>().begin()), name detail::container_traits_gt<T>::c_iterator_t>
        ) &&

        implies
        (
                !ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<decl(declval<T>().end()), name detail::container_traits_gt<T>::iterator_t>
        ) &&
        implies
        (
                ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<decl(declval<T>().end()), name detail::container_traits_gt<T>::c_iterator_t>
        ) &&

        ENV_STD::is_same_v<decl(declval<T>().cbegin()), name detail::container_traits_gt<T>::c_iterator_t>&&
        ENV_STD::is_same_v<decl(declval<T>().cend()), name detail::container_traits_gt<T>::c_iterator_t>&&

        implies
        (
                is_std_equality_comparable_g<name detail::container_traits_gt<T>::value_t>,
                detail::is_std_equatable_container_g<T>
        ) &&

        ENV_STD::is_same_v<decl(declvall<T>().swap(declvall<T>())), void>&&
        ENV_STD::is_same_v<decl(ENV_STD::swap(declvall<T>(), declvall<T>())), void>&&

        ENV_STD::is_same_v<decl(declval<T>().size()), name detail::container_traits_gt<T>::size_t>&&
        ENV_STD::is_same_v<decl(declval<T>().max_size()), name detail::container_traits_gt<T>::size_t>&&
        ENV_STD::is_convertible_v<decl(declval<T>().empty()), bool>
);

ENV_TEST_CASE("container")
{
    STD_REQUIRE(is_std_container_g<test::test_unordered_set_t>);
    STD_REQUIRE(is_std_container_g<test::test_set_t>);
    STD_REQUIRE(is_std_container_g<test::test_deque_t>);
    STD_REQUIRE(is_std_container_g<test::test_vector_t>);
    STD_REQUIRE(is_std_container_g<test::test_map_t>);
    STD_REQUIRE(is_std_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer
COND_CHECK_UNARY
(
        is_std_allocator_aware_container,
        is_std_container_g<T>&&
        TYPE_COND(detail::allocator_aware_container_traits_gt<T>) &&
        ENV_STD::is_same_v<
                decl(declval<T>().get_allocator()),
                name detail::allocator_aware_container_traits_gt<T>::allocator_t>
);

ENV_TEST_CASE("allocator aware container")
{
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_unordered_set_t>);
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_set_t>);
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_deque_t>);
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_vector_t>);
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_map_t>);
    STD_REQUIRE(is_std_allocator_aware_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_allocator_aware_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/ReversibleContainer
COND_CHECK_UNARY
(
        is_std_reversible_container,
        is_std_container_g<T>&&
        TYPE_COND(detail::reversible_container_traits_gt<T>) &&

        /* these have conditions like T.rbegin() has to be the same as reverse_iterator(T.begin()), and so on */

        implies
        (
                !ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<
                        decl(declval<T>().rbegin()),
                        name detail::reversible_container_traits_gt<T>::reverse_iterator_t>
        ) &&
        implies
        (
                ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<
                        decl(declval<T>().rbegin()),
                        name detail::reversible_container_traits_gt<T>::c_reverse_iterator_t>
        ) &&
        implies
        (
                !ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<
                        decl(declval<T>().rend()),
                        name detail::reversible_container_traits_gt<T>::reverse_iterator_t>
        ) &&
        implies
        (
                ENV_STD::is_const_v<T>,
                ENV_STD::is_same_v<
                        decl(declval<T>().rend()),
                        name detail::reversible_container_traits_gt<T>::c_reverse_iterator_t>
        ) &&

        ENV_STD::is_same_v<
                decl(declval<T>().crbegin()),
                name detail::reversible_container_traits_gt<T>::c_reverse_iterator_t>&&
        ENV_STD::is_same_v<
                decl(declval<T>().crend()),
                name detail::reversible_container_traits_gt<T>::c_reverse_iterator_t>
);

ENV_TEST_CASE("reversible container")
{
    STD_REQUIRE_FALSE(is_std_reversible_container_g<test::test_unordered_set_t>);
    STD_REQUIRE(is_std_reversible_container_g<test::test_set_t>);
    STD_REQUIRE(is_std_reversible_container_g<test::test_deque_t>);
    STD_REQUIRE(is_std_reversible_container_g<test::test_vector_t>);
    STD_REQUIRE(is_std_reversible_container_g<test::test_map_t>);
    STD_REQUIRE_FALSE(is_std_reversible_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_reversible_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
COND_CHECK_UNARY
(
        is_std_contiguous_container,
        is_std_container_g<T>&&
        is_std_legacy_random_access_iterator_g<name detail::container_traits_gt<T>::iterator_t>&&
        is_std_legacy_random_access_iterator_g<name detail::container_traits_gt<T>::c_iterator_t>
);

ENV_TEST_CASE("contiguous container")
{
    STD_REQUIRE_FALSE(is_std_contiguous_container_g<test::test_unordered_set_t>);
    STD_REQUIRE_FALSE(is_std_contiguous_container_g<test::test_set_t>);
    // Not standard required
    // STD_REQUIRE(is_std_contiguous_container_g<test::test_deque_t>);
    STD_REQUIRE(is_std_contiguous_container_g<test::test_vector_t>);
    STD_REQUIRE_FALSE(is_std_contiguous_container_g<test::test_map_t>);
    STD_REQUIRE_FALSE(is_std_contiguous_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_contiguous_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/SequenceContainer
COND_CHECK_UNARY
(
        is_std_sequence_container,
        is_std_container_g<T>&&

        /* it is impossible to check for emplace except for checking if it exists */
        /* there are special requirements for specific containers which is not the goal of this project (see PSA 6) */
        /* all of these have even more requirements */
        implies
        (
                is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>,
                detail::has_emplace_g<T>&&

                detail::is_std_init_constructible_container_g<T>&&
                detail::is_std_init_insertable_container_g<T>&&
                detail::is_std_init_place_assignable_container_g<T>
        ) &&
        implies
        (
                is_std_copy_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
                implies
                (
                        is_std_copy_assignable_g<name detail::container_traits_gt<T>::value_t>,
                        detail::is_std_init_copy_assignable_container_g<T>
                ) &&
                detail::is_std_copy_n_constructible_container_g<T>&&

                detail::is_std_copy_insertable_container_g<T>&&
                detail::is_std_copy_n_insertable_container_g<T>&&

                detail::is_std_copy_n_place_assignable_container_g<T>
        ) &&
        implies
        (
                is_std_move_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
                detail::is_std_move_insertable_container_g<T>
        ) &&

        ENV_STD::is_same_v<
                decl(declvalr<T>().erase(declval<name detail::container_traits_gt<T>::c_iterator_t>())),
                name detail::container_traits_gt<T>::iterator_t>&&

        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().erase(
                                declval<name detail::container_traits_gt<T>::c_iterator_t>(),
                                declval<name detail::container_traits_gt<T>::c_iterator_t>())),
                name detail::container_traits_gt<T>::iterator_t>&&

        ENV_STD::is_same_v<decl(declvalr<T>().clear()), void>
);

ENV_TEST_CASE("sequence container")
{
    STD_REQUIRE_FALSE(is_std_sequence_container_g<test::test_unordered_set_t>);
    STD_REQUIRE_FALSE(is_std_sequence_container_g<test::test_set_t>);
    STD_REQUIRE(is_std_sequence_container_g<test::test_deque_t>);
    STD_REQUIRE(is_std_sequence_container_g<test::test_vector_t>);
    STD_REQUIRE_FALSE(is_std_sequence_container_g<test::test_map_t>);
    STD_REQUIRE_FALSE(is_std_sequence_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_sequence_container_g<int>);
}


// There are many more requirements of sequence containers that are container-specific,
// which is not part of the goal of this project.
// I made the mistake of creating checks for these in the beginning,
// so to not waste my work I'm putting these here.
// only std::deque satisfies all requirements

COND_CHECK_UNARY
(
        is_std_complete_sequence_container,
        is_std_sequence_container_g<T>&&

        implies
        (
            is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>,
            detail::has_emplace_front_g<T>&&
            detail::has_emplace_back_g<T>
        ) &&
        implies
        (
            is_std_copy_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
            detail::is_std_push_front_copyable_container_g<T>&&
            detail::is_std_push_back_copyable_container_g<T>
        ) &&
        implies
        (
            is_std_move_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
            detail::is_std_push_front_moveable_container_g<T>&&
            detail::is_std_push_back_moveable_container_g<T>
        ) &&

        (
                ENV_STD::is_const_v<T>?
                (
                          ENV_STD::is_same_v<
                                  decl(declvalr<T>().front()),
                                  name detail::container_traits_gt<T>::c_ref_t>&&
                          ENV_STD::is_same_v<
                                  decl(declvalr<T>().back()),
                                  name detail::container_traits_gt<T>::c_ref_t>&&
                          ENV_STD::is_same_v<
                                  decl(declvalr<T>()[declval<name detail::container_traits_gt<T>::size_t>()]),
                                  name detail::container_traits_gt<T>::c_ref_t>
                )
                :
                (
                      ENV_STD::is_same_v<
                              decl(declvalr<T>().front()),
                              name detail::container_traits_gt<T>::ref_t>&&
                      ENV_STD::is_same_v<
                              decl(declvalr<T>().back()),
                              name detail::container_traits_gt<T>::ref_t>&&
                      ENV_STD::is_same_v<
                              decl(declvalr<T>()[declval<name detail::container_traits_gt<T>::size_t>()]),
                              name detail::container_traits_gt<T>::ref_t>
                )
        )
);

ENV_TEST_CASE("complete sequence container")
{
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<test::test_unordered_set_t>);
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<test::test_set_t>);
    STD_REQUIRE(is_std_complete_sequence_container_g<test::test_deque_t>);
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<test::test_vector_t>);
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<test::test_map_t>);
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_complete_sequence_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/AssociativeContainer
COND_CHECK_UNARY
(
        is_std_associative_container,
        is_std_container_g<T>&&
        TYPE_COND(detail::associative_container_traits_gt<T>) &&

        /* all of these have lots of effects and complexity requirements */

        implies
        (
                is_std_copy_constructible_g<name detail::associative_container_traits_gt<T>::key_compare_t>,
                detail::is_std_key_compare_constructible_container_g<T>&&
                implies
                (
                        is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>,
                        detail::is_std_init_key_compare_constructible_container_g<T>
                )
        ) &&
        implies
        (
                is_std_default_constructible_g<name detail::associative_container_traits_gt<T>::key_compare_t>,
                detail::is_std_default_constructible_container_g<T>&&
                implies
                (
                        is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>,
                        detail::is_std_init_constructible_container_g<T>
                )
        ) &&
        implies
        (
                is_std_copy_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>&&
                is_std_copy_assignable_g<name detail::container_traits_gt<T>::value_t>,
                detail::is_std_init_copy_assignable_container_g<T>
        ) &&

        ENV_STD::is_same_v<
                decl(declval<T>().key_comp()),
                name detail::associative_container_traits_gt<T>::key_compare_t>&&
        ENV_STD::is_same_v<
                decl(declval<T>().value_comp()),
                name detail::associative_container_traits_gt<T>::value_compare_t>
);

ENV_TEST_CASE("associative container")
{
    STD_REQUIRE_FALSE(is_std_associative_container_g<test::test_unordered_set_t>);
    STD_REQUIRE(is_std_associative_container_g<test::test_set_t>);
    STD_REQUIRE_FALSE(is_std_associative_container_g<test::test_deque_t>);
    STD_REQUIRE_FALSE(is_std_associative_container_g<test::test_vector_t>);
    STD_REQUIRE(is_std_associative_container_g<test::test_map_t>);
    STD_REQUIRE_FALSE(is_std_associative_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_associative_container_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/UnorderedAssociativeContainer
COND_CHECK_UNARY
(
        is_std_unordered_associative_container,
        is_std_container_g<T>&&
        TYPE_COND(detail::unordered_associative_container_traits_gt<T>) &&

        /* all of these have some kind of post effects */
        is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>?
        (
                implies
                (
                        is_std_copy_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_copy_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_init_key_hash_equal_constructible_container_g<T>
                ) &&
                implies
                (
                        is_std_copy_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_default_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_init_key_hash_constructible_container_g<T>
                ) &&
                implies
                (
                        is_std_default_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_default_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_init_buckets_constructible_container_g<T>
                )
        )
        :
        (
                !is_std_emplace_constructible_into_g<name detail::container_traits_gt<T>::value_t, T>,
                implies
                (
                        is_std_copy_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_copy_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_key_hash_equal_constructible_container_g<T>
                ) &&
                implies
                (
                        is_std_copy_constructible_g<
                              name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_default_constructible_g<
                              name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_key_hash_constructible_container_g<T>
                ) &&
                implies
                (
                        is_std_default_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::hasher_t>&&
                        is_std_default_constructible_g<
                                name detail::unordered_associative_container_traits_gt<T>::key_equal_t>,
                        detail::is_std_buckets_constructible_container_g<T>
                )
        ) &&
        implies
        (
                  is_std_copy_assignable_g<name detail::container_traits_gt<T>::value_t>&&
                  is_std_copy_insertable_into_g<name detail::container_traits_gt<T>::value_t, T>,
                  detail::is_std_init_copy_assignable_container_g<T>
        )
);

ENV_TEST_CASE("unordered associative container")
{
    STD_REQUIRE(is_std_unordered_associative_container_g<test::test_unordered_set_t>);
    STD_REQUIRE_FALSE(is_std_unordered_associative_container_g<test::test_set_t>);
    STD_REQUIRE_FALSE(is_std_unordered_associative_container_g<test::test_deque_t>);
    STD_REQUIRE_FALSE(is_std_unordered_associative_container_g<test::test_vector_t>);
    STD_REQUIRE_FALSE(is_std_unordered_associative_container_g<test::test_map_t>);
    STD_REQUIRE(is_std_unordered_associative_container_g<test::test_unordered_map_t>);
    STD_REQUIRE_FALSE(is_std_unordered_associative_container_g<int>);
}


#endif // ENV_CONTAINER_STANDARD_REQUIREMENTS_HPP
