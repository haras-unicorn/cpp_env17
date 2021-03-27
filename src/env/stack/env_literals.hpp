#ifndef ENV_LITERALS_HPP
#define ENV_LITERALS_HPP


// numbers

COND_CHECK_UNARY
(
        is_number_literal,
        (ENV_STD::is_same_v < T, unsigned long long int >) ||
        (ENV_STD::is_same_v < T, long double >)
);

COND_CONCEPT(number_literal, (is_number_literal_g<C>));

COND_TMP
((res_name, name T),
 (is_number_literal_g<T> && is_arithmetic_g < TRes > ))
cmp_fn parse_literal(T literal) noex -> deduc_res(T)
{
    ret clamp_cast<TRes>(literal);
}


// atomic

COND_TMP
((res_name, name T),
 (is_number_literal_g<T>) && (is_atomic_g < TRes > ))
fun inl parse_literal(T literal) noex -> deduc_res(T)
{
    ret TRes{clamp_cast<atomic_value_gt<TRes>>(literal)};
}


// chars

ENV_DETAIL_BEGIN

tmp<name T> let_cmp is_char_literal_g =
        ENV_STD::is_same_v<T, char> ||
        ENV_STD::is_same_v<T, wchar_t> ||
        #if ENV_CPP >= 20
        ENV_STD::is_same_v<T, char8_t> || // this is why this is in detail
        #endif // ENV_CPP >= 20
        ENV_STD::is_same_v<T, char16_t> ||
        ENV_STD::is_same_v<T, char32_t>;

ENV_DETAIL_END

COND_CHECK_UNARY(is_char_literal, (detail::is_char_literal_g<T>));

COND_CONCEPT(char_literal, (detail::is_char_literal_g<C>));

COND_TMP((res_name, name T), (is_char_literal_g<T>))
cmp_fn parse_literal(T literal) noex -> deduc_res(T)
{
    ret clamp_cast<TRes>(literal);
}


// labels

ENV_DETAIL_BEGIN

tmp<name T> let_cmp is_label_literal_g =
        ENV_STD::is_same_v<T, const char*> ||
        ENV_STD::is_same_v<T, const wchar_t*> ||
        #if ENV_CPP >= 20
        ENV_STD::is_same_v<TLiteral, const char8_t *> || // this is why this is in detail
        #endif // ENV_CPP >= 20
        ENV_STD::is_same_v<T, const char16_t*> ||
        ENV_STD::is_same_v<T, const char32_t*>;

ENV_DETAIL_END

COND_CHECK_UNARY(is_label_literal, (detail::is_label_literal_g<T>));

COND_CONCEPT(label_literal, (detail::is_label_literal_g<C>));

COND_TMP((res_name, name T),
         (is_label_literal_g<T> &&
          ENV_STD::is_constructible_v < TRes, T, ENV_STD::size_t >))
cmp_fn parse_literal(T literal, ENV_STD::size_t size) noex -> deduc_res(T)
{
    ret res_con(literal, size);
}


// declarations

#define WHOLE_L(_name, _suffix, ...)                                                      \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                                \
        cmp_fn op "" CAT(_, _suffix)(unsigned long long int literal) noex->CAT(_name, _t) \
        {                                                                                 \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal));      \
        }                                                                                 \
        SEMI

#define ATOMIC_L(_name, _suffix, ...)                                                      \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                                 \
        fun inl op "" CAT(_, _suffix)(unsigned long long int literal) noex->CAT(_name, _t) \
        {                                                                                  \
                ret ENV::parse_literal<__VA_ARGS__>(literal);                              \
        }                                                                                  \
        SEMI

#define FLOATING_L(_name, _suffix, ...)                                              \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                           \
        cmp_fn op "" CAT(_, _suffix)(long double literal) noex->CAT(_name, _t)       \
        {                                                                            \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal)); \
        }                                                                            \
        SEMI

#define CHAR_L(_name, _suffix, _literal, ...)                                        \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                           \
        cmp_fn op "" CAT(_, _suffix)(_literal literal) noex->CAT(_name, _t)          \
        {                                                                            \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal)); \
        }                                                                            \
        SEMI

#define TEXT_L(_name, _suffix, _literal, ...)                                                     \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                                        \
        cmp_fn op "" CAT(_, _suffix)(_literal literal, ENV_STD::size_t size) noex->CAT(_name, _t) \
        {                                                                                         \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal, size));        \
        }                                                                                         \
        SEMI

#define RT_TEXT_L(_name, _suffix, _literal, ...)                                              \
        typ(CAT(_name, _t)) = __VA_ARGS__;                                                    \
        fun inl op "" CAT(_, _suffix)(_literal literal, ENV_STD::size_t size)->CAT(_name, _t) \
        {                                                                                     \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal, size));    \
        }                                                                                     \
        SEMI

#define WHOLE_UL(_name, _suffix, ...)                                                     \
        UNIQUE(_name, __VA_ARGS__);                                                       \
        cmp_fn op "" CAT(_, _suffix)(unsigned long long int literal) noex->CAT(_name, _t) \
        {                                                                                 \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal));      \
        }                                                                                 \
        SEMI

#define FLOATING_UL(_name, _suffix, ...)                                             \
        UNIQUE(_name, __VA_ARGS__);                                                  \
        cmp_fn op "" CAT(_, _suffix)(long double literal) noex->CAT(_name, _t)       \
        {                                                                            \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal)); \
        }                                                                            \
        SEMI

#define CHAR_UL(_name, _suffix, _literal, ...)                                       \
        UNIQUE(_name, __VA_ARGS__);                                                  \
        cmp_fn op "" CAT(_, _suffix)(_literal literal) noex->CAT(_name, _t)          \
        {                                                                            \
                ret scast<CAT(_name, _t)>(ENV::parse_literal<__VA_ARGS__>(literal)); \
        }                                                                            \
        SEMI


// real

// unsigned

WHOLE_L(u8, u8, uint8_t);

WHOLE_L(u16, u16, uint16_t);

WHOLE_L(u32, u32, uint32_t);

WHOLE_L(u64, u64, uint64_t);

// signed

WHOLE_L(s8, s8, int8_t);

WHOLE_L(s16, s16, int16_t);

WHOLE_L(s32, s32, int32_t);

WHOLE_L(s64, s64, int64_t);

// floating

FLOATING_L(f, f, float);

FLOATING_L(pf, pf, double);

FLOATING_L(hpf, hpf, long double);

// chars

CHAR_L(c, c, char, char);

CHAR_L(wc, wc, wchar_t, wchar_t);

#if ENV_CPP >= 20
CHAR_L(c8, c8, char8_t, char8_t);
#endif // ENV_CPP >= 20

CHAR_L(c16, c16, char16_t, char16_t);

CHAR_L(c32, c32, char32_t, char32_t);

// text

#if ENV_CPP >= 17

TEXT_L(l, l, const char *, ENV_STD::string_view);

TEXT_L(lw, lw, const wchar_t *, ENV_STD::wstring_view);

#if ENV_CPP >= 20
TEXT(l8, l8, const char8_t *, ENV_STD::u8string_view);
#endif // ENV_CPP >= 20

TEXT_L(l16, l16, const char16_t *, ENV_STD::u16string_view);

TEXT_L(l32, l32, const char32_t *, ENV_STD::u32string_view);

#else // ENV_CPP >= 17

TEXT_L(l, l, const char *, const char *);

TEXT_L(lw, lw, const wchar_t *, const wchar_t *);

TEXT_L(l16, l16, const char16_t *, const char16_t *);

TEXT_L(l32, l32, const char32_t *, const char16_t *);

#endif // ENV_CPP >= 17


// idealistic

WHOLE_L(natural, n, uintmax_t);

let_cmp zero{0_n}, one{1_n}, two{2_n}, three{3_n};

WHOLE_L(whole, w, intmax_t);

let_cmp negative_zero{-0_w},
        negative_one{-1_w},
        negative_two{-2_w},
        negative_three{-3_w};

FLOATING_L(rational, r, long double);

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286_r
#define E 2.718281828459045235360287471352662497757247093699959574966967627724076630353_r

let_cmp pi{PI}, e{E};

CHAR_L(char, c, char32_t, char32_t);

TEXT_L(label, l, const char32_t *, ENV_STD::u32string_view);


// special

WHOLE_L(flag, f, bool);

let_cmp
        truthy{0_f},
        falsy{1_f},
        on{truthy}, off{falsy},
        yes{truthy}, no{falsy},
        active{truthy}, inactive{falsy};

WHOLE_L(byte, b, ENV_STD::byte);


// atomic

ATOMIC_L(atomic_flag, af, ENV_STD::atomic_flag);

// on most architectures 16 bit atomics are lock free
ATOMIC_L(atomic_natural, an, ENV_STD::atomic < uint16_t >);

// on most architectures 16 bit atomics are lock free
ATOMIC_L(atomic_whole, aw, ENV_STD::atomic < int16_t >);


ENV_TEST_CASE("atomic literal")
{
    REQUIRES_FALSE(is_arithmetic_g<atomic_flag_t>);
    obj bool res = (1_af).test_and_set();
    REQUIRE(res);
    res = (0_af).test_and_set();
    REQUIRE_FALSE(res);
}


#endif // ENV_LITERALS_HPP
