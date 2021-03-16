#ifndef ENV_INDIRECTION_HPP
#define ENV_INDIRECTION_HPP


// indirect struct

ENV_DETAIL_BEGIN

tmp<name T, name = requirement_t>
strct indirect_gs
{
    nonced cmp_obj_p static bool is_indirect{false};
};

tmp<name T>
strct indirect_gs<T, ENV::require_nt<ENV_STD::is_object_v<ENV_STD::remove_reference_t<decl(*declval<T>())>>>>
{
    nonced cmp_obj_p static bool is_indirect{true};
    typ_a(subject_t, maybe_unused) = ENV_STD::remove_reference_t<decl(*declval<T>())>;
};

tmp<name T>
strct indirect_gs<T[]> : public indirect_gs<T*> { };

#if ENV_CPP >= 17

tmp<name T, deduc Extent>
strct indirect_gs<T[Extent]> : public indirect_gs<T*> { };

#else // ENV_CPP >= 17

tmp<name T, int Extent>
strct indirect_gs<T[Extent]> : public indirect_gs<T*> {};

#endif // ENV_CPP >= 17

ENV_DETAIL_END


// indirect

COND_CHECK_UNARY(is_indirect, detail::indirect_gs<T>::is_indirect);

COND_CONCEPT(indirect, is_indirect_g<C>);

ENV_TEST_CASE("indirect")
{
    REQUIRES(is_indirect_g<int*>);
    REQUIRES(is_indirect_g<const int[]>);
    REQUIRES(is_indirect_g<volatile int[3]>);
    REQUIRES(is_indirect_g<ENV_STD::unique_ptr < int>>);
    REQUIRES_FALSE(is_indirect_g<int&&>);
    REQUIRES(ENV_STD::is_function_v < ENV_STD::remove_pointer_t < void(*)() >>);
    REQUIRES_FALSE(is_indirect_g<void (*)()>);
    REQUIRES_FALSE(is_indirect_g<volatile int&>);
    REQUIRES_FALSE(is_indirect_g<const int>);
}


// points_to

COND_CHECK_BINARY(points_to, ENV_STD::is_same_v < name detail::indirect_gs<TLhs>::subject_t, TRhs >);

COND_CONCEPT_UNARY(pointing_to, points_to_g<C, T>);

ENV_TEST_CASE("points to")
{
    REQUIRES(points_to_g < int *, int >);
    REQUIRES(points_to_g<const int[], const int>);
    REQUIRES(points_to_g<volatile int[3], volatile int>);
    REQUIRES(points_to_g < ENV_STD::unique_ptr < int >, int >);
    REQUIRES_FALSE(points_to_g < int *, double >);
    REQUIRES_FALSE(points_to_g<const int[], const double>);
    REQUIRES_FALSE(points_to_g<volatile int[3], volatile double>);
    REQUIRES_FALSE(points_to_g < ENV_STD::unique_ptr < int >, double >);
    REQUIRES_FALSE(points_to_g < int &&, int >);
    REQUIRES_FALSE(points_to_g<volatile int&, int>);
    REQUIRES_FALSE(points_to_g<const int, int>);
}


// subject

tmp<name T> typ(subject_gt) = name detail::indirect_gs<T>::subject_t;

COND_CONCEPT_UNARY(subject_of, points_to_g<T, C>);

ENV_TEST_CASE("points to")
{
    REQUIRE_EQT(subject_gt < int * >, int);
    REQUIRE_EQT(subject_gt<const int[]>, const int);
    REQUIRE_EQT(subject_gt<volatile int[3]>, volatile int);
    REQUIRE_EQT(subject_gt < ENV_STD::unique_ptr < int >>, int);
}


#endif //ENV_ENV_INDIRECTION_HPP
