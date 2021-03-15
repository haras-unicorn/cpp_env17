#ifndef ENV_HASH_HPP
#define ENV_HASH_HPP


// literals

WHOLE_L(hash, h, ENV_STD::size_t);


ENV_DETAIL_BEGIN

typ(_id_t) =
ENV_STD::conditional_t<
        ENV_STD::atomic<uint_fast64_t>::is_always_lock_free, uint_fast64_t, ENV_STD::conditional_t<
                ENV_STD::atomic<uint_fast32_t>::is_always_lock_free, uint_fast32_t, ENV_STD::conditional_t<
                        ENV_STD::atomic<uint_fast16_t>::is_always_lock_free, uint_fast16_t,
                        uint_fast8_t>>>;

ENV_DETAIL_END

WHOLE_L(id, id, detail::_id_t);

typ(atom_id_t) = ENV_STD::atomic<detail::_id_t>;


// std hash

tmp<name T> typ(hash_gt) = ENV_STD::hash<T>;

// std hashable

// has_hash is weird like this because the STL tries to get a reference to void if
// we try to instantiate ENV_STD::hash with it, so we have to check that first.
// I don't know if this is UB, though. It seems a bit sketchy.

EXPR_CHECK_UNARY(is_std_hashable, (COND_EXPR(!ENV_STD::is_same_v < T, void > ), hash_gt<T>{ }(declvalr<T>())));

COND_CONCEPT(std_hashable, (is_std_hashable_g<C>));

ENV_TEST_CASE("std hashable")
{
    REQUIRES(is_std_hashable_g<int>);
    REQUIRES_FALSE(is_std_hashable_g<void>);
}


// hash

tmp<name T>
cmp_fn hash(const ENV::std_hashable_c<T>& subject)
noexpr(ENV_STD::hash < ENV::unqualified_gt < T >> { }(subject)) -> hash_t
{
    cmp ENV_STD::hash<ENV::unqualified_gt<T>> _hasher{ };
    ret _hasher(subject);
}

ENV_TEST_CASE("hash std hashable")
{
    REQUIRE_EQ(hash(1), hash(1));
    REQUIRE_NE(hash(1), hash(2));
}


// hash member

EXPR_CHECK_UNARY(has_hash, (declval<T>().hash()));

COND_TMP_UNARY(ENV::has_hash_g < T > && !ENV::is_std_hashable_g < ENV::remove_qualifiers_gt < T >>)
cmp_fn hash(const T& subject) noexpr(subject.hash()) -> hash_t { ret subject.hash(); }


// hashable

EXPR_CHECK_UNARY(is_hashable, (has_hash_g<T> || is_std_hashable_g<T>));

COND_CONCEPT(hashable, (is_hashable_g<C>));


// key

COND_CHECK_UNARY(is_key, (is_hashable_g<T> && is_equatable_g < T > ));

COND_CONCEPT(key, (is_key_g<C>));


// combine

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x

// MSVC has a hard time with variadic concepts... - https://repl.it/@Hrle/hash#main.cpp
// Here's the gist - this is good enough for combining hashes - just call this as much as you have to and you're good.

tmp<name TFirst, name TSecond>
cmp_fn hash(const hashable_c<TFirst>& _first, const hashable_c<TSecond>& _second)
noexpr(nonce(hash(_first)), nonce(hash(_second))) -> hash_t
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
    cmp_fn op()(const hashable_c<T>& subject) const noexpr(hash(subject)) -> hash_t { ret hash(subject); }
}
inl cmp hasher{ };

ENV_TEST_CASE("hasher")
{
    REQUIRE_EQ(hasher(0), hasher(0));
    REQUIRE_NE(hasher(1), hasher(2));
}


#endif // ENV_HASH_HPP
