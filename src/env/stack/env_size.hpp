#ifndef ENV_SIZE
#define ENV_SIZE


WHOLE_UL(size, s, ENV_STD::size_t);

let_cmp empty{0_s}, single{1_s}, dual{2_s}, triple{3_s};

tmp<name T> let_cmp size_of_g = size_t{sizeof(T)};

ENV_TEST_CASE("size")
{
    REQUIRES(empty == 0);
    REQUIRES(single == 1);
    REQUIRES(dual == 2);
    REQUIRES(triple == 3);

    SUBCASE("size of")
    {
        REQUIRES(size_of_g<uint8_t> == 1);
        REQUIRES(size_of_g<uint16_t> == 2);
        REQUIRES(size_of_g<uint32_t> == 4);
        REQUIRES(size_of_g<uint64_t> == 8);
    }
}


WHOLE_UL(rank, r, ENV_STD::size_t);

let_cmp rank_zero{0_r}, rank_one{1_r}, rank_two{2_r}, rank_three{3_r};

tmp<name... T> let_cmp rank_of_v = rank_t{sizeof...(T)};

#if ENV_CPP >= 17

tmp<deduc... V> let_cmp rank_of_a = rank_t{sizeof...(V)};

#else  // ENV_CPP >= 17
tmp<int... V> let_cmp rank_of_a = rank_t{sizeof...(V)};
#endif // ENV_CPP >= 17

ENV_TEST_CASE("rank")
{
    REQUIRES(rank_zero == 0);
    REQUIRES(rank_one == 1);
    REQUIRES(rank_two == 2);
    REQUIRES(rank_three == 3);

    SUBCASE("rank of")
    {
        REQUIRES(rank_of_v<int, int, int> == 3);
        REQUIRES(rank_of_v<int, int> == 2);
        REQUIRES(rank_of_a<1, 2, 3> == 3);
        REQUIRES(rank_of_a<1, 2> == 2);
    }
}


WHOLE_UL(arity, a, ENV_STD::size_t);

let_cmp nullary{0_a}, unary{1_a}, binary{2_a}, ternary{3_a};

tmp<name T> let_cmp arity_of_g = arity_t{argument_count_g < callable_r < T >> };

ENV_TEST_CASE("arity")
{
    REQUIRES(nullary == 0);
    REQUIRES(unary == 1);
    REQUIRES(binary == 2);
    REQUIRES(ternary == 3);

    SUBCASE("arity of")
    {
        REQUIRES(arity_of_g<int(int, int, int)> == 3);
        REQUIRES(arity_of_g<void (*)(double, char)> == 2);
        REQUIRES(arity_of_g<ENV_STD::function < void(int)>> == 1);
        REQUIRES(arity_of_g<decl(&ENV_STD::function<void()>::op())> == 0);
    }
}


#endif // ENV_SIZE