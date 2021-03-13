#ifndef FWA_CORE_HASH_HPP
#define FWA_CORE_HASH_HPP


// literals

FWA_NAMESPACE_DETAIL_BEGIN

typ(_id_t) =
FWA_STD::conditional_t<
        FWA_STD::atomic<uint_fast64_t>::is_always_lock_free, uint_fast64_t, FWA_STD::conditional_t<
                FWA_STD::atomic<uint_fast32_t>::is_always_lock_free, uint_fast32_t, FWA_STD::conditional_t<
                        FWA_STD::atomic<uint_fast16_t>::is_always_lock_free, uint_fast16_t,
                        uint_fast8_t>>>;

FWA_NAMESPACE_DETAIL_END

WHOLE_L(hash, h, FWA_STD::size_t);

WHOLE_L(id, id, detail::_id_t);


// std hash

tmp<name T> typ(hash_gt) = FWA_STD::hash<T>;

// std hashable

// has_hash is weird like this because the STL tries to get a reference to void if
// we try to instantiate FWA_STD::hash with it, so we have to check that first.
// I don't know if this is UB, though. It seems a bit sketchy.

EXPR_CHECK_UNARY(is_std_hashable, (COND_EXPR(!FWA_STD::is_same_v < T, void > ), hash_gt<T>{ }(declvalr<T>())));

COND_CONCEPT(std_hashable, (is_std_hashable_g<C>));

FWA_CORE_TEST_CASE("std hashable")
{
    REQUIRES(is_std_hashable_g<int>);
    REQUIRES_FALSE(is_std_hashable_g<void>);
}


// hash

tmp<name T>
cmp_fn hash(const FWA_CORE::std_hashable_c<T>& subject)
noex(noex(FWA_STD::hash<FWA_CORE::unqualified_gt<T>>{ }(subject))) -> hash_t
{
    cmp FWA_STD::hash<FWA_CORE::unqualified_gt<T>> _hasher{ };
    ret _hasher(subject);
}

FWA_CORE_TEST_CASE("hash std hashable")
{
    REQUIRE_EQ(hash(1), hash(1));
    REQUIRE_NE(hash(1), hash(2));
}


// hash member

// TODO: fix formatting

EXPR_CHECK_UNARY(has_hash, (declval <T > ().hash()));

EXPR_TMP_UNARY((has_hash_g<T>, COND_EXPR(!is_std_hashable_g<remove_qualifiers_gt < T> > )))

cmp_fn hash(const T& subject) noex(noex(subject.hash())) -> hash_t { ret subject.hash(); }


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
noex(noex(hash(_first), hash(_second))) -> hash_t
{
    let seed = hash(_first);
    ret seed ^ (hash(_second) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

FWA_CORE_TEST_CASE("combine")
{
    let _21 = hash(2, 1);
    let _12 = hash(1, 2);
    REQUIRE_NE(_21, _12);
}


// hasher

strct hasher_t
{
    tmp<name T>
    cmp_fn op()(const hashable_c<T>& subject) const noex(noex(hash(subject))) -> hash_t { ret hash(subject); }
}
inl cmp hasher{ };


FWA_CORE_TEST_CASE("hasher")
{
    REQUIRE_EQ(hasher(0), hasher(0));
    REQUIRE_NE(hasher(1), hasher(2));
}


#endif // FWA_CORE_HASH_HPP
