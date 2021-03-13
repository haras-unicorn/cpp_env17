#ifndef ENV_ENDIAN_HPP
#define ENV_ENDIAN_HPP

enum class endianness_t
{
    little = 0b01,
    big = 0b10,
    other = 0b00
};

FWA_NAMESPACE_DETAIL_BEGIN
FWA_CLANG_SUPPRESS_PUSH("ConstantConditionsOC")
FWA_CLANG_SUPPRESS_PUSH("UnreachableCode")

// stolen from: https://stackoverflow.com/questions/1583791/constexpr-and-endianness

cmp_fn endianness() noex
{
    // converts chars to uint32_t on current platform
    obj cmp uint32_t _native{'ABCD'};

    // 41 42 43 44 = 'ABCD' hex ASCII code
    obj cmp uint32_t _little{0x41424344u};

    // 44 43 42 41 = 'DCBA' hex ASCII code
    obj cmp uint32_t _big{0x44434241u};

    ret(_native == _little) ? endianness_t::little : (_native == _big) ? endianness_t::big
                                                                       : endianness_t::other;
}

FWA_CLANG_SUPPRESS_POP
FWA_CLANG_SUPPRESS_POP
FWA_NAMESPACE_DETAIL_END

let_cmp endianness{detail::endianness()};

let_cmp is_little_endian{endianness == endianness_t::little};

let_cmp is_big_endian{endianness == endianness_t::big};

ENV_TEST_CASE("endianness")
{
    REQUIRES(endianness != endianness_t::other);
    REQUIRES(is_little_endian != is_big_endian);
}

#endif // ENV_ENDIAN_HPP
