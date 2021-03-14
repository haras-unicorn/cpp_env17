#ifndef ENV_CASTING_HPP
#define ENV_CASTING_HPP


// bit cast

// stolen from: https://en.cppreference.com/w/cpp/numeric/bit_cast
// constexpr support needs compiler magic

COND_TMP_BINARY
(
        (sizeof(TLhs) == sizeof(TRhs)) &&
        (ENV_STD::is_trivially_copyable_v < TRhs > ) && (ENV_STD::is_trivially_copyable_v < TLhs > ) &&
        (ENV_STD::is_trivially_constructible_v < TLhs > )
)
fun inl bit_cast(const TRhs& from) noexcept -> TLhs
{
    TLhs to;
    ENV_STD::memcpy(&to, &from, sizeof(TLhs));
    return to;
}

ENV_TEST_CASE("bit cast")
{
    let u = bit_cast<uint8_t, int8_t>(-1);
    nonce(u);
    REQUIRE(u);
}


// underlying cast

tmp<name T> cmp_fn underlying_cast(enum_c <T> _enum) noex -> ENV_STD::underlying_type_t<T>
{
    ret scast<ENV_STD::underlying_type_t<T>>(_enum);
}

ENV_TEST_CASE("underlying cast")
{
    UNIQUE(my_size, ENV_STD::size_t);
    obj cmp my_size_t a{10};

    REQUIRES(underlying_cast(a) == 10);
}


// clamp cast

tmp<res_name, name T>
cmp_fn clamp_cast(arithmetic_c <T> from) noex -> deduc_res(T)
{
    let cmp max = ENV_STD::numeric_limits<T>::max();
    let cmp min = ENV_STD::numeric_limits<T>::min();

    let cmp res_max = scast<T>(ENV_STD::numeric_limits<TRes>::max());
    let cmp res_min = scast<T>(ENV_STD::numeric_limits<TRes>::min());


    if_cmp(ENV_STD::is_enum_v<T>) ret clamp_cast < res_t(T) > (underlying_cast(from));

    else if_cmp(ENV_STD::is_same_v<TRes, T> || ENV_STD::is_same_v<TRes, nores_t>) ret from;

    else if_cmp(ENV_STD::is_same_v<TRes, bool>) ret !!from;

    else if_cmp(is_unsigned_g<T>)
    {
        if_cmp (res_max >= max) ret scast<TRes>(from);
        else ret scast<TRes>((from > res_max) ? res_max : from);
    }
    else if_cmp(is_unsigned_g<TRes>)
    {
        if_cmp (res_max >= max) ret scast<TRes>((from > 0) ? from : 0);
        else ret scast<TRes>((from > res_max) ? res_max : (from < 0) ? 0 : from);
    }
    else
    {
        if_cmp (res_max >= max)
        {
            if_cmp (res_min <= min) ret scast<TRes>(from);
            else ret scast<TRes>((from < res_min) ? res_min : from);
        }
        else
        {
            if_cmp (res_min <= min) ret scast<TRes>((from > res_max) ? res_max : from);
            else ret scast<TRes>((from > res_max) ? res_max : (from < res_min) ? res_min : from);
        }
    }
}

TEST_CASE("clamp_cast")
{
    let cmp static u8max = ENV_STD::numeric_limits<uint8_t>::max();
    let cmp static i8max = ENV_STD::numeric_limits<int8_t>::max();
    let cmp static i8min = ENV_STD::numeric_limits<int8_t>::min();

    let cmp static u16max = ENV_STD::numeric_limits<uint16_t>::max();
    let cmp static i16min = ENV_STD::numeric_limits<int16_t>::min();


    SUBCASE("smaller")
    {
        SUBCASE("unsigned -> unsigned")
        {
            REQUIRES(clamp_cast<uint8_t>(uint8_t{1}) == 1);
            REQUIRES(clamp_cast<uint8_t>(uint16_t{1}) == 1);
        }
        SUBCASE("unsigned -> signed")
        {
            REQUIRES(clamp_cast<int8_t>(uint8_t{1}) == 1);
            REQUIRES(clamp_cast<int8_t>(uint16_t{1}) == 1);
        }
        SUBCASE("signed -> unsigned")
        {
            REQUIRES(clamp_cast<uint8_t>(int8_t{-1}) == 0);
            REQUIRES(clamp_cast<uint8_t>(int16_t{-1}) == 0);
        }
        SUBCASE("signed -> signed")
        {
            REQUIRES(clamp_cast<int8_t>(int8_t{-1}) == -1);
            REQUIRES(clamp_cast<int8_t>(int16_t{-1}) == -1);
        }
    }

    SUBCASE("bigger")
    {
        SUBCASE("unsigned -> unsigned")
        {
            REQUIRES(clamp_cast<uint8_t>(uint16_t{u16max}) == u8max);
        }
        SUBCASE("unsigned -> signed")
        {
            REQUIRES(clamp_cast<int8_t>(uint16_t{u16max}) == i8max);
        }
        SUBCASE("signed -> unsigned")
        {
            REQUIRES(clamp_cast<uint8_t>(int16_t{i16min}) == 0);
        }
        SUBCASE("signed -> signed")
        {
            REQUIRES(clamp_cast<int8_t>(int16_t{i16min}) == i8min);
        }
    }
}


#endif // ENV_CASTING_HPP
