#ifndef ENV_MEMBERS_HPP
#define ENV_MEMBERS_HPP

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name T>
    strct member_gs
{
    cmp_obj static bool is_member{false};
};

tmp<name TMember, name THolder>
    strct member_gs<TMember THolder::*> : public type_gt<TMember>
{
    cmp_obj static bool is_member{true};

    typ(holder_t) = THolder;
};

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY(is_member, detail::member_gs<T>::is_member);

COND_CONCEPT(member, is_member_g<C>);

tmp<name T> typ(member_gs) = detail::member_gs<member_c<T>>;

tmp<name T> typ(member_type_gt) = name detail::member_gs<member_c<T>>::type;

tmp<name T> typ(member_holder_gt) = name detail::member_gs<member_c<T>>::holder_t;

TEST_CASE("members")
{
    strct test { const int a; };
    REQUIRES(is_member_g<decl(&test::a)>);
    REQUIRE_EQT(member_type_gt<decl(&test::a)>, const int);
    REQUIRE_EQT(member_holder_gt<decl(&test::a)>, test);
}

// operators

// member access

OPERATOR_DETECTOR(arrow_operator, operator->, void *operator->(););
OPERATOR_DETECTOR(subscript_operator, operator[], int &operator[](int););

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
    has_bitand_operator_g<T> &&
        has_bitor_operator_g<T> &&
            has_bitxor_operator_g<T> &&
                has_bitnot_operator_g<T> &&

                    has_bitand_assign_operator_g<T> &&
                        has_bitor_assign_operator_g<T> &&
                            has_bitxor_assign_operator_g<T>);

#endif // ENV_MEMBERS_HPP
