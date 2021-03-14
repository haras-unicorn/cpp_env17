#ifndef ENV_CONTAINER_TRAITS_REQUIREMENTS_HPP
#define ENV_CONTAINER_TRAITS_REQUIREMENTS_HPP


ENV_DETAIL_BEGIN

TYPE_TMP_UNARY(name T::iterator)
strct container_traits_gt
{
    typ(value_t) = name T::value_type;

    typ(ref_t) = name T::reference;
    typ(c_ref_t) = name T::const_reference;

    typ(difference_t) = name T::difference_type;
    typ(size_t) = name T::size_type;

    typ(iterator_t) = name T::iterator;
    typ(c_iterator_t) = name T::const_iterator;

    sass(is_std_erasable_from_g<value_t, T>);
    sass(is_std_copy_insertable_into_g<value_t, T>);
    sass(is_std_equality_comparable_g<T>);

    sass(ENV_STD::is_same_v<ref_t, value_t&>);
    sass(ENV_STD::is_same_v<c_ref_t, const value_t&>);

    sass(ENV_STD::is_same_v<name ENV_STD::iterator_traits<iterator_t>::difference_type, difference_t>);
    sass(ENV_STD::is_same_v<name ENV_STD::iterator_traits<c_iterator_t>::difference_type, difference_t>);

    /* size_t must be large enough to represent all positive values of difference_t */

    sass(is_std_legacy_forward_iterator_g<iterator_t>);
    sass(is_std_legacy_forward_iterator_g<c_iterator_t>);
    sass(ENV_STD::is_convertible_v<iterator_t, c_iterator_t>);
};


TYPE_TMP_UNARY(name T::allocator_type)
strct allocator_aware_container_traits_gt : public container_traits_gt<T>
{
private:
    typ(container_traits_t) = container_traits_gt<T>;

public:
    using name container_traits_t::value_t;

    using name container_traits_t::ref_t;
    using name container_traits_t::c_ref_t;

    using name container_traits_t::difference_t;
    using name container_traits_t::size_t;

    using name container_traits_t::iterator_t;
    using name container_traits_t::c_iterator_t;

    typ(allocator_t) = name T::allocator_type;

    sass(implies(is_std_copy_assignable_g<value_t>, is_std_copy_insertable_into_g<value_t, T>));
    sass(implies(is_std_move_assignable_g<value_t>, is_std_move_insertable_into_g<value_t, T>));

    sass(is_std_allocator_g<allocator_t>);
    sass(is_std_default_constructible_g<allocator_t>);
};

TYPE_TMP_UNARY(name T::reverse_iterator)
strct reversible_container_traits_gt : public container_traits_gt<T>
{
private:
    typ(container_traits_t) = container_traits_gt<T>;

public:
    using name container_traits_t::value_t;

    using name container_traits_t::ref_t;
    using name container_traits_t::c_ref_t;

    using name container_traits_t::difference_t;
    using name container_traits_t::size_t;

    using name container_traits_t::iterator_t;
    using name container_traits_t::c_iterator_t;

    typ(reverse_iterator_t) = name T::reverse_iterator;
    typ(c_reverse_iterator_t) = name T::const_reverse_iterator;

    sass(is_std_legacy_bidirectional_iterator_g<iterator_t>);
    sass(is_std_legacy_bidirectional_iterator_g<c_iterator_t>);

    sass(ENV_STD::is_same_v<ENV_STD::reverse_iterator<iterator_t>, reverse_iterator_t>);
    sass(ENV_STD::is_same_v<ENV_STD::reverse_iterator<c_iterator_t>, c_reverse_iterator_t>);
};


TYPE_TMP_UNARY(name T::key_compare)
strct associative_container_traits_gt : public container_traits_gt<T>
{
private:
    typ(container_traits_t) = container_traits_gt<T>;

public:
    using name container_traits_t::value_t;

    using name container_traits_t::ref_t;
    using name container_traits_t::c_ref_t;

    using name container_traits_t::difference_t;
    using name container_traits_t::size_t;

    using name container_traits_t::iterator_t;
    using name container_traits_t::c_iterator_t;

    typ(key_t) = name T::key_type;
    typ(key_compare_t) = name T::key_compare;
    typ(value_compare_t) = name T::value_compare;

    sass(is_std_destructible_g<key_t>);
    sass(is_std_binary_predicate_g<key_compare_t>);
    sass(is_std_binary_predicate_g<value_compare_t>);
};


TYPE_TMP_UNARY(name T::key_equal)
strct unordered_associative_container_traits_gt : public allocator_aware_container_traits_gt<T>
{
private:
    typ(container_traits_t) = container_traits_gt<T>;

public:
    using name container_traits_t::value_t;

    using name container_traits_t::ref_t;
    using name container_traits_t::c_ref_t;

    using name container_traits_t::difference_t;
    using name container_traits_t::size_t;

    using name container_traits_t::iterator_t;
    using name container_traits_t::c_iterator_t;

    typ(key_t) = name T::key_type;
    typ(hasher_t) = name T::hasher;
    typ(key_equal_t) = name T::key_equal;

    typ(local_iterator_t) = name T::local_iterator;
    typ(c_local_iterator_t) = name T::const_local_iterator;

    sass(is_std_hash_g<hasher_t>);
    sass(is_std_binary_predicate_g<key_equal_t>);

    sass(is_std_legacy_iterator_g<local_iterator_t>);
    sass(iterator_traits_gt<local_iterator_t>::tmp is_same_g<iterator_traits_gt<iterator_t>>);
    sass(is_std_legacy_iterator_g<c_local_iterator_t>);
    sass(iterator_traits_gt<c_local_iterator_t>::tmp is_same_g<iterator_traits_gt<c_iterator_t>>);
};


// non-associative checks:

// if container value is EmplaceConsturctible:

FUNCTION_DETECTOR(emplace);

EXPR_CHECK_UNARY
(
        is_std_init_constructible_container,
        T(declval<name container_traits_gt<T>::iterator_t>(),
          declval<name container_traits_gt<T>::iterator_t>()),
        T(declval<ENV_STD::initializer_list<name container_traits_gt<T>::value_t>>())
);

EXPR_CHECK_UNARY
(
        is_std_init_key_compare_constructible_container,
        T(declval<name associative_container_traits_gt<T>::iterator_t>(),
          declval<name associative_container_traits_gt<T>::iterator_t>(),
          declval<name associative_container_traits_gt<T>::key_compare_t>())
  );

COND_CHECK_UNARY
(
        is_std_init_insertable_container,
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().insert(
                                declval<name container_traits_gt<T>::c_iterator_t>(),
                                declval<name container_traits_gt<T>::iterator_t>(),
                                declval<name container_traits_gt<T>::iterator_t>())),
                name container_traits_gt<T>::iterator_t>
        &&
        ENV_STD::is_same_v<
                decl(
                         declvalr<T>().insert(
                                 declval<name container_traits_gt<T>::c_iterator_t>(),
                                 declval<ENV_STD::initializer_list<name container_traits_gt<T>::value_t>>())),
                name container_traits_gt<T>::iterator_t>
);

COND_CHECK_UNARY
(
        is_std_init_place_assignable_container,
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().assign(
                                declval<name container_traits_gt<T>::iterator_t>(),
                                declval<name container_traits_gt<T>::iterator_t>())),
                void>
        &&
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().assign(
                                declval<ENV_STD::initializer_list<name container_traits_gt<T>::value_t>>())),
                void>
);

FUNCTION_DETECTOR(emplace_front);

FUNCTION_DETECTOR(emplace_back);


// if container value is CopyInsertable:

COND_CHECK_UNARY
(
        is_std_copyable_container,
        ENV_STD::is_same_v<decl(declvall<T>() = declvall<const T>()), T&>
);

COND_CHECK_UNARY
(
        is_std_init_copy_assignable_container,
        ENV_STD::is_same_v<
                decl(declvalr<T>() = declval<ENV_STD::initializer_list<name container_traits_gt<T>::value_t>>()),
                T&>
);

EXPR_CHECK_UNARY
(
        is_std_copy_n_constructible_container,
        T(declval<name container_traits_gt<T>::size_t>(),
          declvalr<const name container_traits_gt<T>::value_t>())
);

COND_CHECK_UNARY
(
        is_std_copy_insertable_container,
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().insert(
                                declval<name container_traits_gt<T>::c_iterator_t>(),
                                declvalr<const name container_traits_gt<T>::value_t>())),
                name container_traits_gt<T>::iterator_t>
);

COND_CHECK_UNARY
(
        is_std_copy_n_insertable_container,
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().insert(
                                declval<name container_traits_gt<T>::c_iterator_t>(),
                                declval<name container_traits_gt<T>::size_t>(),
                                declvalr<const name container_traits_gt<T>::value_t>())),
                name container_traits_gt<T>::iterator_t>
);

COND_CHECK_UNARY
(
        is_std_copy_n_place_assignable_container,
        ENV_STD::is_same_v<
                decl(
                        declvalr<T>().assign(
                                declval<name container_traits_gt<T>::size_t>(),
                                declvalr<const name container_traits_gt<T>::value_t>())),
                void>
);

COND_CHECK_UNARY
(
        is_std_push_front_copyable_container,
        ENV_STD::is_same_v<decl(declvalr<T>().push_front(declvalr<const name container_traits_gt<T>::value_t>())), void>
);

COND_CHECK_UNARY
(
        is_std_push_back_copyable_container,
        ENV_STD::is_same_v<decl(declvalr<T>().push_back(declvalr<const name container_traits_gt<T>::value_t>())), void>
);


// if container value is MoveInsertable

COND_CHECK_UNARY
(
        is_std_move_insertable_container,
        ENV_STD::is_same_v<decl(
                declvalr<T>().insert(
                        declval<name container_traits_gt<T>::c_iterator_t>(),
                        declvalr<name container_traits_gt<T>::value_t>())),
        name container_traits_gt<T>::iterator_t>
);

COND_CHECK_UNARY
(
        is_std_push_front_moveable_container,
        ENV_STD::is_same_v<decl(declvalr<T>().push_front(declvalr<name container_traits_gt<T>::value_t>())), void>
);

COND_CHECK_UNARY
(
        is_std_push_back_moveable_container,
        ENV_STD::is_same_v<decl(declvalr<T>().push_back(declvalr<name container_traits_gt<T>::value_t>())), void>
);


// if container value is EqualityComparable

COND_CHECK_UNARY
(
        is_std_equatable_container,
        ENV_STD::is_convertible_v<decl(declval<T>() == declval<T>()), bool> &&
        ENV_STD::is_convertible_v<decl(declval<T>() != declval<T>()), bool>
);


// associative checks:

// if (key_compare / hasher and key_equal) is CopyConstructible:

EXPR_CHECK_UNARY
(
        is_std_key_compare_constructible_container,
        T(declval<name associative_container_traits_gt<T>::key_compare_t>())
);

EXPR_CHECK_UNARY
(
        is_std_key_hash_equal_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>(),
          declval<name unordered_associative_container_traits_gt<T>::key_equal_t>())
);

// and value is EmplaceConstructible

EXPR_CHECK_UNARY
(
        is_std_init_key_hash_equal_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>(),
          declval<name unordered_associative_container_traits_gt<T>::key_equal_t>()),

        T(declval<ENV_STD::initializer_list<name unordered_associative_container_traits_gt<T>::value_t>>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>(),
          declval<name unordered_associative_container_traits_gt<T>::key_equal_t>())
);


// if hasher is CopyConstructible and key_equal is DefaultConstructible

EXPR_CHECK_UNARY
(
        is_std_key_hash_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>())
);

// and if value is Emplace constructible

EXPR_CHECK_UNARY
(
        is_std_init_key_hash_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>()),

        T(declval<ENV_STD::initializer_list<name unordered_associative_container_traits_gt<T>::value_t>>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>(),
          declval<name unordered_associative_container_traits_gt<T>::hasher_t>())
);


// if (key_compare / hasher and key_equal) is DefaultConstructible:

EXPR_CHECK_UNARY
(
        is_std_default_constructible_container,
        T()
);

EXPR_CHECK_UNARY
(
        is_std_buckets_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::size_t>())
);

// and if value is EmplaceConstructible:

EXPR_CHECK_UNARY
(
        is_std_init_buckets_constructible_container,
        T(declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::iterator_t>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>()),

        T(declval<ENV_STD::initializer_list<name unordered_associative_container_traits_gt<T>::value_t>>(),
          declval<name unordered_associative_container_traits_gt<T>::size_t>())
);

ENV_DETAIL_END


#endif // ENV_CONTAINER_TRAITS_REQUIREMENTS_HPP
