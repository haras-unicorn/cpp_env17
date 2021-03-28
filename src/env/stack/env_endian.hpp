#ifndef ENV_ENDIAN_HPP
#define ENV_ENDIAN_HPP


enum class endianness_t
{
    little = 0b01,
    big = 0b10,
    other = 0b00
};


cmp_fn is_little(endianness_t endian) noex -> bool
{
    ret underlying_cast(endianness_t::little) & underlying_cast(endian);
}

cmp_fn is_big(endianness_t endian) noex -> bool
{
    ret underlying_cast(endianness_t::big) & underlying_cast(endian);
}


ENV_TEST_CASE("endianness")
{
    REQUIRES(is_little(endianness_t::little));
    REQUIRES_FALSE(is_little(endianness_t::big));
    REQUIRES_FALSE(is_little(endianness_t::other));

    REQUIRES(is_big(endianness_t::big));
    REQUIRES_FALSE(is_big(endianness_t::little));
    REQUIRES_FALSE(is_big(endianness_t::other));
}


ENV_DETAIL_BEGIN
ENV_CLANG_SUPPRESS_PUSH("ConstantConditionsOC")
ENV_CLANG_SUPPRESS_PUSH("UnreachableCode")
ENV_CLANG_SUPPRESS_PUSH("-Wfour-char-constants")

// stolen from: https://stackoverflow.com/questions/1583791/constexpr-and-endianness

cmp_fn endianness() noex
{
    // converts chars to uint32_t on current platform
    nonced cmp uint32_t _native{'ABCD'};
    nonce(_native);

    // 41 42 43 44 = 'ABCD' hex ASCII code
    nonced cmp uint32_t _little{0x41424344u};
    nonce(_little);

    // 44 43 42 41 = 'DCBA' hex ASCII code
    nonced cmp uint32_t _big{0x44434241u};
    nonce(_big);

    ret (_native == _little) ? endianness_t::little :
        (_native == _big) ? endianness_t::big :
        endianness_t::other;
}

ENV_CLANG_SUPPRESS_POP
ENV_CLANG_SUPPRESS_POP
ENV_CLANG_SUPPRESS_POP
ENV_DETAIL_END


let_cmp endianness{detail::endianness()};

let_cmp is_little_endian{endianness == endianness_t::little};

let_cmp is_big_endian{endianness == endianness_t::big};

ENV_TEST_CASE("native endianness")
{
    REQUIRES(endianness != endianness_t::other);
    REQUIRES(is_little_endian != is_big_endian);
}


#endif // ENV_ENDIAN_HPP
