#ifndef ENV_MEMBERS_HPP
#define ENV_MEMBERS_HPP


ENV_DETAIL_BEGIN

template<name T>
strct member_gs
{
    typ(is_member_t) = false_t;
};

template<name TMember, name THolder>
strct member_gs<TMember THolder::*> : public type_gt<TMember>
{
    typ(is_member_t) = true_t;

    typ(holder_t) = THolder;
};

ENV_DETAIL_END

COND_CHECK_UNARY(is_member, name detail::member_gs<T>::is_member_t);

COND_CONCEPT(member, is_member_gs<C>);


template<name T>
typ(member_gs) = detail::member_gs<member_c<T>>;

template<name T>
typ(member_type_gt) = name detail::member_gs<member_c<T>>::type;

template<name T>
typ(member_holder_gt) = name detail::member_gs<member_c<T>>::holder_t;


TEST_CASE("members")
{
    strct test
    {
        const int a;
    };
    REQUIRES(is_member_g<decl(&test::a)>);
    REQUIRE_EQT(member_type_gt<decl(&test::a)>, const int);
    REQUIRE_EQT(member_holder_gt<decl(&test::a)>, test);
}


// operators

// member access

OPERATOR_DETECTOR(arrow_operator, operator->, void* operator->(););
OPERATOR_DETECTOR(indirection_operator, operator*, int& operator*(););
OPERATOR_DETECTOR(subscript_operator, operator[], int& operator[](int););

// call

OPERATOR_DETECTOR(call_operator, operator(), void operator()(););

// assignment

OPERATOR_DETECTOR(assign_operator, operator=, void operator=(int););

// bits

OPERATOR_DETECTOR(bitand_operator, operator&, int operator&(int););
OPERATOR_DETECTOR(bitor_operator, operator|, int operator|(int););
OPERATOR_DETECTOR(bitxor_operator, operator^, int operator^(int););
OPERATOR_DETECTOR(bitnot_operator, operator~, int operator~(););

OPERATOR_DETECTOR(bitand_assign_operator, operator&=, int operator&=(int););
OPERATOR_DETECTOR(bitor_assign_operator, operator|=, int operator|=(int););
OPERATOR_DETECTOR(bitxor_assign_operator, operator^=, int operator^=(int););

COND_CHECK_UNARY(
        has_bit_operators,
        has_bitand_operator_gs<T>,
        has_bitor_operator_gs<T>,
        has_bitxor_operator_gs<T>,
        has_bitnot_operator_gs<T>,
        has_bitand_assign_operator_gs<T>,
        has_bitor_assign_operator_gs<T>,
        has_bitxor_assign_operator_gs<T>);


// base

COND_CHECK_UNARY(
        is_empty_base,
        ENV_STD::is_class<T>,
        ENV_STD::is_empty<T>,
        neg_vt<ENV_STD::is_final<T>>);

COND_CONCEPT(empty_base, is_empty_base_gs<C>);

ENV_TEST_CASE("empty base")
{
    strct empty_t{};
    strct unitary_t
    {
        int i;
    };
    strct final_empty_t final{};

    REQUIRES(is_empty_base_g<empty_t>);
    REQUIRES_FALSE(is_empty_base_g<final_empty_t>);
    REQUIRES_FALSE(is_empty_base_g<int>);
    REQUIRES_FALSE(is_empty_base_g<unitary_t>);
}


#endif // ENV_MEMBERS_HPP
