#ifndef ENV_CONTAINER_ELEMENT_REQUIREMENTS_HPP
#define ENV_CONTAINER_ELEMENT_REQUIREMENTS_HPP

// it is useful to define these here.

FWA_NAMESPACE_TEST_BEGIN

typ(container_key_t) = char;
typ(container_value_t) = int;
typ(container_pair_t) = FWA_STD::pair<const container_key_t, container_value_t>;

// notes: https://en.cppreference.com/w/cpp/container/unordered_set
typ(test_unordered_set_t) = FWA_STD::unordered_set<container_value_t>;
// notes: https://en.cppreference.com/w/cpp/container/set
typ(test_set_t) = FWA_STD::set<container_value_t>;
// notes: https://en.cppreference.com/w/cpp/container/list
typ(test_deque_t) = FWA_STD::deque<container_value_t>;
// notes: https://en.cppreference.com/w/cpp/container/vector
typ(test_vector_t) = FWA_STD::vector<container_value_t>;
// notes: https://en.cppreference.com/w/cpp/container/map
typ(test_map_t) = FWA_STD::map<container_key_t, container_value_t>;
// notes: https://en.cppreference.com/w/cpp/container/unordered_map
typ(test_unordered_map_t) = FWA_STD::unordered_map<container_key_t, container_value_t>;

FWA_NAMESPACE_TEST_END

// useful to get the container allocator

FWA_NAMESPACE_DETAIL_BEGIN

ALIAS_DETECTOR(allocator_type);

tmp<name TContainer, name = success_t>
    strct container_alloc_gs : public type_gt<FWA_STD::allocator<name TContainer::value_type>>{};

tmp<name T>
    strct container_alloc_gs<T, require_nt<has_allocator_type_g<T>>> : public type_gt<name T::allocator_type>{};

tmp<name T> typ(container_alloc_gt) = name container_alloc_gs<T>::type;

tmp<name TContainer, name TValue>
    typ(rebound_container_alloc_gt) =
        name FWA_STD::allocator_traits<container_alloc_gt<TContainer>>::tmp rebind_alloc<TValue>;

FWA_NAMESPACE_DETAIL_END

// these take a container and an element

// these have a kind of circular dependency with container types, so
// I'm defining these first "improperly" because they don't matter as much as the container types
// they don't have the _into/_from suffixes, but it improves readability, so I'm adding it

EXPR_CHECK_BINARY(is_std_default_insertable_into,
                  FWA_STD::allocator_traits<detail::rebound_container_alloc_gt<TRhs, TLhs>>::construct(
                      declvall<detail::rebound_container_alloc_gt<TRhs, TLhs>>(),
                      declval<TRhs *>()));

EXPR_CHECK_BINARY(is_std_copy_insertable_into,
                  FWA_STD::allocator_traits<detail::rebound_container_alloc_gt<TRhs, TLhs>>::construct(
                      declvall<detail::rebound_container_alloc_gt<TRhs, TLhs>>(), declval<TRhs *>(),
                      declval<const TRhs>()));

EXPR_CHECK_BINARY(is_std_move_insertable_into,
                  FWA_STD::allocator_traits<detail::rebound_container_alloc_gt<TRhs, TLhs>>::construct(
                      declvall<detail::rebound_container_alloc_gt<TRhs, TLhs>>(),
                      declval<TRhs *>(), declvalr<TRhs>()));

// EmplaceConstructible is impossible to check except for the container alloc
// notes: https://en.cppreference.com/w/cpp/named_req/EmplaceConstructible
TYPE_CHECK_BINARY(is_std_emplace_constructible_into,
                  FWA_STD::allocator_traits<detail::rebound_container_alloc_gt<TRhs, TLhs>>);

EXPR_CHECK_BINARY(is_std_erasable_from,
                  FWA_STD::allocator_traits<detail::rebound_container_alloc_gt<TRhs, TLhs>>::destroy(
                      declvall<detail::rebound_container_alloc_gt<TRhs, TLhs>>(),
                      declval<TRhs *>()));

#endif // ENV_CONTAINER_ELEMENT_REQUIREMENTS_HPP
