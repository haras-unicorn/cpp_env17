#ifndef ENV_CASTING_HPP
#define ENV_CASTING_HPP


// bit cast

// stolen from: https://en.cppreference.com/w/cpp/numeric/bit_cast
// constexpr support needs compiler magic

COND_TMP_BINARY
(
        (sizeof(TLhs) == sizeof(TRhs)) &&
        (ENV_STD::is_trivially_copyable_v < TRhs > ) &&
        (ENV_STD::is_trivially_copyable_v < TLhs > ) &&
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

tmp<name T> cmp_fn underlying_cast(enum_c <T> _enum) noex ->
ENV_STD::underlying_type_t<T>
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
cmp_fn clamp_cast(arithmetic_c <T> _from) noex -> deduc_res(T)
{
    typ(_to_t) = res_t(T);
    typ(_from_t) = T;
    typ(_max_t) = long double;

    let cmp _max_from = scast<_max_t>(ENV_STD::numeric_limits<_from_t>::max());
    let cmp _min_from = scast<_max_t>(ENV_STD::numeric_limits<_from_t>::min());

    let cmp _max_to = scast<_max_t>(ENV_STD::numeric_limits<_to_t>::max());
    let cmp _min_to = scast<_max_t>(ENV_STD::numeric_limits<_to_t>::min());

    let cmp _max = scast<_from_t>(ENV_STD::numeric_limits<_to_t>::max());
    let cmp _min = scast<_from_t>(ENV_STD::numeric_limits<_to_t>::min());


    if_cmp(ENV_STD::is_same_v<_to_t, _from_t>)
    {
        ret _from;
    }
    else if_cmp(ENV_STD::is_same_v<_to_t, bool>)
    {
        ret !!_from;
    }
    else if_cmp(ENV_STD::is_enum_v<_from_t>)
    {
        ret clamp_cast < _to_t > (underlying_cast(_from));
    }
    else if_cmp(ENV_STD::is_enum_v<_to_t>)
    {
        typ(_clamped_t) = ENV_STD::underlying_type_t<_to_t>;
        let _clamped = ENV::clamp_cast<_clamped_t>(_from);

        ret scast<_to_t>(_clamped);
    }
    else
    {
        if_cmp (_max_to >= _max_from)
        {
            if_cmp (_min_to <= _min_from)
            {
                ret scast<_to_t>(_from);
            }
            else
            {
                ret scast<_to_t>((_from < _min) ? _min : _from);
            }
        }
        else
        {
            if_cmp (_min_to <= _min_from)
            {
                ret scast<_to_t>((_from > _max) ? _max : _from);
            }
            else
            {
                ret scast<_to_t>(
                        (_from > _max) ? _max :
                        (_from < _min) ? _min :
                        _from);
            }
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
            REQUIRES(clamp_cast<uint8_t>(int8_t{1}) == 1);
            REQUIRES(clamp_cast<uint8_t>(int16_t{1}) == 1);
            REQUIRES(clamp_cast<uint8_t>(int8_t{-1}) == 0);
            REQUIRES(clamp_cast<uint8_t>(int16_t{-1}) == 0);
        }
        SUBCASE("signed -> signed")
        {
            REQUIRES(clamp_cast<int8_t>(int8_t{1}) == 1);
            REQUIRES(clamp_cast<int8_t>(int16_t{1}) == 1);
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

    SUBCASE("enum")
    {
        enm my_size_t : ENV_STD::size_t { };
        REQUIRES(clamp_cast<my_size_t>(2) == my_size_t{2});
        REQUIRES(clamp_cast<my_size_t>(-2) == my_size_t{0});
        REQUIRES(clamp_cast<int>(my_size_t{2}) == 2);
        REQUIRES(clamp_cast<int>(my_size_t{0}) == 0);
    }

    SUBCASE("bool")
    {
        REQUIRES(clamp_cast<bool>(10) == true);
        REQUIRES(clamp_cast<bool>(0) == false);
        REQUIRES(clamp_cast<int>(true) == 1);
        REQUIRES(clamp_cast<int>(false) == 0);
    }
}


#endif // ENV_CASTING_HPP
