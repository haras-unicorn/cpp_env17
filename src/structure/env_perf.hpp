#ifndef ENV_PERF_HPP
#define ENV_PERF_HPP

// stolen from: https://www.youtube.com/watch?t=56m34s&v=nXaxk27zwlk

ENV_NAMESPACE_BEGIN

ENV_NAMESPACE_TEST_BEGIN

[[maybe_unused]] void escape([[maybe_unused]] void *_this) noexcept
{
#if ENV_GCC || ENV_CLANG // TODO: check if clang has this extension
    asm volatile(""
                 :
                 : "g"(_this)
                 : "memory");
#else // MSVC - just don't...
//    __asm [[maybe_unused]] auto volatile _copy = _this;
#endif
}

[[maybe_unused]] void clobber() noexcept
{
#if ENV_GCC || ENV_CLANG // TODO: check if clang has this extension
    asm volatile(""
                 :
                 :
                 : "memory");
#else // MSVC - just don't...
//    __asm [[maybe_unused]] int volatile _i{};
#endif
}

ENV_NAMESPACE_TEST_END

ENV_NAMESPACE_END

#endif // ENV_PERF_HPP
