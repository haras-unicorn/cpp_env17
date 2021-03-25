#ifndef ENV_TIE_HPP
#define ENV_TIE_HPP


ENV_DETAIL_BEGIN

tmp<name... TElements>
strct tie_vt : public ENV_STD::tuple<TElements...>
{
private:
    typ(_tie_t) = tie_vt<TElements...>;
    DECL_BASE(tuple, (ENV_STD::tuple<TElements...>));

    typ(_first_t) = name variadic_vt<TElements...>::tmp at_nt<0>;

    cmp_obj static rank_t _rank = rank_of_v<TElements...>;

    cmp_obj static flag_t _is_array = ENV_STD::conjunction_v<
            ENV_STD::is_same<_first_t, TElements>...>;

public:
    using _tuple_base_t::tuple;
    using _tuple_base_t::op=;

    DEFAULT_NOCON_LIFE(tie_vt, CMP);

    COND_TMP_UNARY(
            ((ENV_STD::is_assignable_v<_first_t, const T&>) && (_is_array)))
    callb inl op=(const array_ngt<scast<size_t>(_rank), T>& array) noex
    {
        _copy_array(array, ENV_STD::make_index_sequence<_rank>{ });
        ret *this;
    }

private:
    tmp<name T, ENV_STD::size_t... Indices>
    callb inl _copy_array(
            const array_ngt<scast<size_t>(_rank), T>& array,
            ENV_STD::index_sequence<Indices...>) noex
    {
        ((ENV_STD::get<Indices>(*this->_as_tuple_base()) = array[Indices]), ...);
    }
};

tmp<name T> cmp_fn tie_forward(T& arg) noex -> T& { ret arg; }

tmp<name... TVariables> cmp_fn tie_forward(
        tie_vt<TVariables...>&& tie) noex -> tie_vt<TVariables...>& { ret tie; }

ENV_DETAIL_END

COND_TMP_VARIADIC
(
        ENV_STD::conjunction_v <
        ENV_STD::disjunction <
        ENV::is_lvalue_ref_gs < TVar >,
        ENV::is_specialization_gs <
        ENV::unqualified_gt < TVar >,
        ENV::detail::tie_vt >>...>
)
cmp_fn tie(TVar&& ...variables) noex -> detail::tie_vt<TVar & ...>
{
    ret {detail::tie_forward<TVar>(variables)...};
}

let_cmp ignore{ENV_STD::ignore};

ENV_TEST_CASE("tie")
{
    mut a{0}, b{0}, c{0};
    mut chr{'a'};

    REQUIRE_EQT(decltype(tpl(1, 2)), detail::pair_ggt<int, int>);
    REQUIRE_EQT(decltype(tie(a, b)), detail::tie_vt<int&, int&>);
    tie(a, b) = tpl(1, 2);
    REQUIRE_EQT(decltype(tpl(1, 2, 3)), detail::array_ngt<3_s, int>);
    REQUIRE_EQT(decltype(tie(a, b, c)), detail::tie_vt<int&, int&, int&>);
    tie(a, b, c) = tpl(1, 2, 3);
    REQUIRE_EQT(decltype(tpl(4, 2, 'c')), detail::tuple_vt<int, int, char>);
    REQUIRE_EQT(decltype(tie(a, b, chr)), detail::tie_vt<int&, int&, char&>);
    tie(a, b, chr) = tpl(4, 2, 'c');
    REQUIRE_EQ(tpl(a, b, c), tpl(4, 2, 3));

    tie(ignore, tie(a, b), c) = tpl(1, tpl(2, 3), 5);
    REQUIRE_EQ(tpl(a, b, c), tpl(2, 3, 5));
}


#endif // ENV_TIE_HPP
