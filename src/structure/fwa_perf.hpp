#ifndef FWA_CORE_PERF_HPP
#define FWA_CORE_PERF_HPP


// stolen from: https://www.youtube.com/watch?t=56m34s&v=nXaxk27zwlk

FWA_NAMESPACE_CORE_BEGIN

FWA_NAMESPACE_TEST_BEGIN

[[maybe_unused]] void escape([[maybe_unused]] void* _this) noexcept
{
#if FWA_GCC || FWA_CLANG // TODO: check if clang has this extension
    asm volatile("" : : "g"(_this) : "memory");
#else // MSVC - just don't...
//    __asm [[maybe_unused]] auto volatile _copy = _this;
#endif
}

[[maybe_unused]] void clobber() noexcept
{
#if FWA_GCC || FWA_CLANG // TODO: check if clang has this extension
    asm volatile("" : : : "memory");
#else // MSVC - just don't...
//    __asm [[maybe_unused]] int volatile _i{};
#endif
}

FWA_NAMESPACE_TEST_END

FWA_NAMESPACE_CORE_END


#endif // FWA_CORE_PERF_HPP
