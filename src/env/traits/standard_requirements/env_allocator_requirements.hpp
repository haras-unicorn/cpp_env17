#ifndef ENV_ALLOCATOR_REQUIREMENTS_HPP
#define ENV_ALLOCATOR_REQUIREMENTS_HPP


// this one gets its own file because of the sheer amount of requirements
// needed to satisfy it.
// notes: https://en.cppreference.com/w/cpp/named_req/Allocator

ENV_DETAIL_BEGIN

// supporting "fancy" pointers ->pointers that are not raw pointers
// (like memory mapped files)
// use std::pointer_traits to use them
// C++20 - convert them to raw pointers using std::to_address
COND_CHECK_UNARY(
        is_std_allocator_ptr,
        is_std_nullable_pointer_gs<T>,
        is_std_legacy_random_access_iterator_gs<T>,
        is_std_legacy_contiguous_iterator_gs<T>,

        /* this has some more restrictions */
        cond_gggt<ENV_STD::is_class<T>,
                  has_arrow_operator_gs<T>,
                  ENV_STD::is_pointer<T>>);


// using a custom struct because we want to make sure that
// TAlloc is not templated with it already
strct rebind_t{};

template<name T>
typ(rebound_alloc_gt) =
        name ENV_STD::allocator_traits<T>::tmp rebind_alloc<rebind_t>;

template<name T>
typ(rebound_traits_gt) =
        name ENV_STD::allocator_traits<T>::tmp rebind_traits<rebind_t>;


template<name TAlloc>
strct allocator_traits_gt : public ENV_STD::allocator_traits<TAlloc>
{
private:
    typ(std_traits_t) = ENV_STD::allocator_traits<TAlloc>;

public:
    typ(alloc_t) = name std_traits_t::allocator_type;

    typ(ptr_t) = name        std_traits_t::pointer;
    typ(c_ptr_t) = name      std_traits_t::const_pointer;
    typ(void_ptr_t) = name   std_traits_t::void_pointer;
    typ(c_void_ptr_t) = name std_traits_t::const_void_pointer;

    typ(value_t) = name std_traits_t::value_type;

    typ(size_t) = name       std_traits_t::size_type;
    typ(difference_t) = name std_traits_t::difference_type;


    /* if allocators are not equal you must deallocate using old allocator and
     * allocate using new on copy */
    typ(prop_on_copy_t) =
            name std_traits_t::propagate_on_container_copy_assignment;

    /* if false or undefined you must move the objects individually
     * if the allocators are not equal */
    typ(prop_on_move_t) =
            name std_traits_t::propagate_on_container_move_assignment;

    /* beware of allocators that don't define this or have this as false
     * in that case, it is undefined behaviour if the containers swap and
     * the allocators are not equal*/
    typ(prop_on_swap_t) =
            name std_traits_t::propagate_on_container_swap;


    // implies a stateless allocator
#if ENV_CPP >= 17
    typ(always_equal_t) =
            name std_traits_t::is_always_equal;
#else // ENV_CPP>= 17
    typ(always_equal_t) = ENV_STD::false_type;
#endif // ENV_CPP>= 17


    template<name TRebind>
    typ(rebind_alloc_gt) = name std_traits_t::tmp rebind_alloc<TRebind>;

    template<name TRebind>
    typ(rebind_traits_gt) = name std_traits_t::tmp rebind_traits<TRebind>;


    sass(is_std_allocator_ptr_gs<ptr_t>::value);
    sass(same_vt<decl(*declval<ptr_t>()), value_t&>::value);

    sass(is_std_allocator_ptr_gs<c_ptr_t>::value);
    sass(same_vt<decl(*declval<c_ptr_t>()), const value_t&>::value);
    sass(ENV_STD::is_convertible<ptr_t, c_ptr_t>::value);

    sass(is_std_nullable_pointer_gs<void_ptr_t>::value);
    sass(EXPR_COND(scast<ptr_t>(declval<void_ptr_t>()))::value);
    sass(ENV_STD::is_convertible<ptr_t, void_ptr_t>::value);
    sass(same_vt<
            void_ptr_t,
            name rebound_traits_gt<alloc_t>::void_pointer>::value);

    // all const void pointers from an allocator should be the same type -
    // impossible to check
    sass(is_std_nullable_pointer_gs<c_void_ptr_t>::value);
    sass(EXPR_COND(scast<c_ptr_t>(declval<c_void_ptr_t>()))::value);
    sass(ENV_STD::is_convertible<ptr_t, c_void_ptr_t>::value);
    sass(ENV_STD::is_convertible<c_ptr_t, c_void_ptr_t>::value);
    sass(ENV_STD::is_convertible<void_ptr_t, c_void_ptr_t>::value);
    sass(same_vt<
            c_void_ptr_t,
            name rebound_traits_gt<alloc_t>::const_void_pointer>::value);
    sass(neg_vt<
            ENV_STD::is_const<value_t>,
            ENV_STD::is_volatile<value_t>>::value);
    sass(ENV_STD::is_object<value_t>::value);

    // can represent the size of the largest object
    sass(ENV_STD::is_unsigned<size_t>::value);
    sass(neg_vt<ENV_STD::is_floating_point<size_t>>::value);

    // can represent the difference of any two pointers
    sass(ENV_STD::is_signed<difference_t>::value);
    sass(neg_vt<ENV_STD::is_floating_point<difference_t>>::value);

    // interestingly it is only optional if
    // the allocator takes more or less than one template type parameters
    sass(same_vt<
            name rebound_traits_gt<alloc_t>::tmp rebind_alloc<value_t>,
            alloc_t>::value);
};

ENV_DETAIL_END


// for all of the operations there are numerous other requirements
// that need to be satisfied.
EXPR_CHECK_UNARY(
        is_std_allocator,
        TYPE_EXPR(detail::allocator_traits_gt<T>),

        /* may throw */
        nonce(detail::allocator_traits_gt<T>::allocate(
                declvall<T>(),
                declval<name detail::allocator_traits_gt<T>::size_t>())),

        /* this one is to aid locality */
        nonce(detail::allocator_traits_gt<T>::allocate(
                declvall<T>(),
                declval<name detail::allocator_traits_gt<T>::size_t>(),
                declval<name detail::allocator_traits_gt<T>::c_void_ptr_t>())),

        /* given pointer is gotten from allocate and is valid */
        nonce(detail::allocator_traits_gt<T>::deallocate(
                declvall<T>(),
                declval<name detail::allocator_traits_gt<T>::ptr_t>(),
                declval<name detail::allocator_traits_gt<T>::size_t>())),

        /* largest value passable to allocate */
        nonce(detail::allocator_traits_gt<T>::max_size(declvall<T>())),

        /* surprisingly the pointer could be from anywhere */
        nonce(detail::allocator_traits_gt<T>::construct(
                declvall<T>(),
                declval<detail::rebind_t*>())),

        /* surprisingly the pointer could be from anywhere */
        nonce(detail::allocator_traits_gt<T>::destroy(
                declvall<T>(),
                declval<detail::rebind_t*>())),

        /* this has some other restrictions and
         * it also satisfies CopyConstructible, but
         * that is just said as a note */
        T{declvall<T>()},
        declvall<T>() = declvall<T>(),
        T{declvall<detail::rebound_alloc_gt<T>>()},
        declvall<T>() = declvall<detail::rebound_alloc_gt<T>>(),

        /* declvalr is same as std::move */
        T{declvalr<T>()},
        declvall<T>() = declvalr<T>(),
        /* no requirement for move assignment */
        T{declvalr<detail::rebound_alloc_gt<T>>()},

        detail::allocator_traits_gt<T>::
                select_on_container_copy_construction(declvall<T>()),

        COND_EXPR(
                /* if the implications are true,
                 * the corresponding operations must not throw */
                imply_vt<name detail::allocator_traits_gt<T>::prop_on_copy_t,
                         is_std_copy_assignable_gs<T>>,
                imply_vt<name detail::allocator_traits_gt<T>::prop_on_move_t,
                         is_std_move_assignable_gs<T>>,
                imply_vt<name detail::allocator_traits_gt<T>::prop_on_swap_t,
                         is_std_swappable_gs<T>>)

        /* the standard then goes on to explain properties of
         * equivalently-valued pointer values
         * which are impossible to check at compile time (maybe in c++20?) */
);


#if ENV_CPP >= 17

COND_CHECK_UNARY(
        is_std_complete_allocator,
        is_std_allocator_gs<T>,

        is_complete_gs<name detail::allocator_traits_gt<T>::alloc_t>,

        /* value_type could be void, so it doesn't need to be complete */

        is_complete_gs<name detail::allocator_traits_gt<T>::ptr_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::c_ptr_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::void_ptr_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::c_void_ptr_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::difference_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::size_t>,

        is_complete_gs<name detail::allocator_traits_gt<T>::prop_on_copy_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::prop_on_move_t>,
        is_complete_gs<name detail::allocator_traits_gt<T>::prop_on_swap_t>,

        is_complete_gs<name detail::allocator_traits_gt<T>::always_equal_t>);

#endif // ENV_CPP>= 17

ENV_TEST_BEGIN

// notes: https://en.cppreference.com/w/cpp/container/vector

typ(test_allocator_t) =
        ENV_STD::vector<int>::allocator_type;

typ(test_alloc_traits_t) =
        ENV_STD::allocator_traits<test_allocator_t>;

typ(test_alloc_ptr_t) =
        ENV_STD::allocator_traits<
                ENV_STD::vector<int>::allocator_type>::pointer;

ENV_TEST_END

ENV_TEST_CASE("allocator")
{
    STD_REQUIRE(is_std_legacy_contiguous_iterator_g<test::test_alloc_ptr_t>);
    STD_REQUIRE(detail::is_std_allocator_ptr_g<test::test_alloc_ptr_t>);
    STD_REQUIRE(is_std_allocator_g<test::test_allocator_t>);
    STD_REQUIRE(is_std_complete_allocator_g<test::test_allocator_t>);
}


#endif // ENV_ALLOCATOR_REQUIREMENTS_HPP
