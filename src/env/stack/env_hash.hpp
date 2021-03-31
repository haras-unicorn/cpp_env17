#ifndef ENV_HASH_HPP
#define ENV_HASH_HPP


// literals

WHOLE_L(hash, h, ENV_STD::size_t);


ENV_DETAIL_BEGIN

typ(_id_t) =
ENV_STD::conditional_t<
        ENV_STD::atomic<uint_fast64_t>::is_always_lock_free,
        uint_fast64_t,
        ENV_STD::conditional_t<
                ENV_STD::atomic<uint_fast32_t>::is_always_lock_free,
                uint_fast32_t,
                ENV_STD::conditional_t<
                        ENV_STD::atomic<uint_fast16_t>::is_always_lock_free,
                        uint_fast16_t,
                        uint_fast8_t>>>;

ENV_DETAIL_END

WHOLE_L(id, id, detail::_id_t);

typ(atom_id_t) = ENV_STD::atomic<detail::_id_t>;


// std hash

tmp<name T> typ(hash_gt) = ENV_STD::hash<T>;


// hash

tmp<name T>
cmp_fn hash(const ENV::std_hashable_c<T>& subject) noex
{
    cmp ENV_STD::hash<ENV::unqualified_gt<T>> _hasher{ };
    ret _hasher(subject);
}

COND_TMP_UNARY
((ENV::has_hash_g < T > ) &&
 (!ENV::is_std_hashable_g < ENV::remove_qualifiers_gt < T >> ))
cmp_fn hash(const T& subject) noex
{
    ret subject.hash();
}

ENV_TEST_CASE("hash std hashable")
{
    REQUIRE_EQ(hash(1), hash(1));
    REQUIRE_NE(hash(1), hash(2));
}


// combine

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x

// MSVC has a hard time with variadic concepts... -
// https://repl.it/@Hrle/hash#main.cpp
// Here's the gist - this is good enough for combining hashes -
// just call this as much as you have to and you're good.

tmp<name TFirst, name TSecond>
cmp_fn hash(
        const hashable_c <TFirst>& _first,
        const hashable_c <TSecond>& _second) noex
{
    let seed = hash(_first);
    ret seed ^ (hash(_second) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

ENV_TEST_CASE("combine")
{
    let _21 = hash(2, 1);
    let _12 = hash(1, 2);
    nonce(_21);
    nonce(_12);
    REQUIRE_NE(_21, _12);
}


// hasher

strct hasher_t
{
    tmp<name T>
    cmp_fn op()(const hashable_c <T>& subject) const noex
    {
        ret hash(subject);
    }
}
inl cmp hasher{ };

ENV_TEST_CASE("hasher")
{
    REQUIRE_EQ(hasher(0), hasher(0));
    REQUIRE_NE(hasher(1), hasher(2));
}


#endif // ENV_HASH_HPP
