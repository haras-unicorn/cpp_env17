#ifndef ENV_INDIRECTION_HPP
#define ENV_INDIRECTION_HPP


// indirect

COND_CHECK_UNARY
(
        is_indirect,
        ENV_STD::is_object_v < ENV_STD::remove_reference_t <
        decl(*declvall<T>()) >>
);

COND_CONCEPT(indirect, is_indirect_g<C>);

ENV_TEST_CASE("indirect")
{
    REQUIRES
    (is_indirect_g<int*>);
    REQUIRES
    (is_indirect_g<const int[]>);
    REQUIRES
    (is_indirect_g<volatile int[3]>);
    REQUIRES
    (is_indirect_g<ENV_STD::unique_ptr < int>>);
    REQUIRES_FALSE
    (is_indirect_g<int&&>);
    REQUIRES
    (ENV_STD::is_function_v < ENV_STD::remove_pointer_t < void(*)() >>);
    REQUIRES_FALSE
    (ENV_STD::is_object_v < void() >);
    REQUIRE_EQT
    (void(), ENV_STD::remove_reference_t < void(&)() >);
    // This is one of those things where a fine reading of the standard matters.
    // The standard requires
    // C++ standard: https://en.cppreference.com/w/cpp/language/pointer
    // MSVC: REQUIRE_EQT
    // (void(*)(), decl(*declval<void (*)()>()));
    REQUIRES_FALSE
    (ENV_STD::is_object_v < ENV_STD::remove_reference_t < void(&)() >>);
    REQUIRES_FALSE
    (is_indirect_g<void (*)()>);
    REQUIRES_FALSE
    (is_indirect_g<volatile int&>);
    REQUIRES_FALSE
    (is_indirect_g<const int>);
}


// subject

tmp<name T>
typ(subject_gt) = ENV_STD::remove_reference_t<decl(*declvall<indirect_r<T>>())>;

COND_CONCEPT_UNARY(subject_of, ENV_STD::is_same_v < C, subject_gt<T>>);

ENV_TEST_CASE("points to")
{
    REQUIRE_EQT(subject_gt < int * >, int);
    REQUIRE_EQT(subject_gt<const int[]>, const int);
    REQUIRE_EQT(subject_gt<volatile int[3]>, volatile int);
    REQUIRE_EQT(subject_gt < ENV_STD::unique_ptr < int >>, int);
}


// points_to

TYPE_CHECK_BINARY(points_to, subject_of_r<TRhs, TLhs>);

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


#endif //ENV_ENV_INDIRECTION_HPP
