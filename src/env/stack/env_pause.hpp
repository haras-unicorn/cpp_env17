#ifndef ENV_PAUSE_HPP
#define ENV_PAUSE_HPP


callb inl pause() noex
{
#if ENV_X86 || ENV_X64
    _mm_pause();
#elif ENV_ARM || ENV_ARM64
    __yield(); // TODO: test somehow
#else // arch
#error "Unsupported computer architecture."
#endif // arch
}

ENV_TEST_CASE("pause")
{
    pause();
}


#endif // ENV_PAUSE_HPP
