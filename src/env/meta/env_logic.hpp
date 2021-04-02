#ifndef ENV_LOGIC_HPP
#define ENV_LOGIC_HPP


// TODO: tests


template<bool Value>
typ(bool_nt) = ENV_STD::bool_constant<Value>;

typ(false_t) = ENV_STD::false_type;

typ(true_t) = ENV_STD::true_type;


template<name... T>
typ(con_vt) = ENV_STD::conjunction<T...>;

template<name... T>
typ(neg_vt) = ENV_STD::negation<ENV_STD::disjunction<T...>>;

template<name... T>
typ(dis_vt) = ENV_STD::disjunction<T...>;


template<name THead, name... TRest>
typ(same_vt) = ENV_STD::conjunction<ENV_STD::is_same<THead, TRest>...>;

template<name THead, name... TRest>
typ(any_vt) = ENV_STD::disjunction<ENV_STD::is_same<THead, TRest>...>;


template<name TCond, name TTrue, name TFalse>
typ(cond_gggt) = ENV_STD::conditional_t<TCond::value, TTrue, TFalse>;

template<bool Cond, name TTrue, name TFalse>
typ(cond_nggt) = ENV_STD::conditional_t<Cond, TTrue, TFalse>;


template<name TIf, name... TThen>
typ(imply_vt) =
        ENV_STD::disjunction<
                ENV_STD::negation<TIf>,
                ENV_STD::conjunction<TThen...>>;


#endif // ENV_LOGIC_HPP
