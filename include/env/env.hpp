#ifndef ENV_INCLUDED
#define ENV_INCLUDED


// Visibility -----------------------------------------------------------------

#if ENV_SHARED
    #if ENV_EXPORT
        #define ENV_VISIBLE HEDLEY_PUBLIC
        #define ENV_HIDDEN  HEDLEY_PRIVATE
    #elif ENV_IMPORT
        #define ENV_VISIBLE HEDLEY_IMPORT
        #define ENV_HIDDEN
    #endif
#else
    #define ENV_VISIBLE
    #define ENV_HIDDEN
#endif


/* ----------------------------------------------------------------------------
 * Env
 * ----------------------------------------------------------------------------
 */

namespace env
{
// Info -----------------------------------------------------------------------

ENV_VISIBLE bool check() noexcept;


// Allocators -----------------------------------------------------------------

// TODO: pre-allocator

namespace alloc
{
// custom allocator

template<typename T>
using allocator [[maybe_unused]] = mi_stl_allocator<T>;

struct ENV_VISIBLE heap
{
    template<typename... Tags>
    static void* allocate(std::size_t size, Tags...)
    {
        return mi_new(size);
    }

    static void deallocate(std::size_t, void* data)
    {
        mi_free(data);
    }
};

using memory_policy =
        ::immer::memory_policy<
                ::immer::heap_policy<heap>,
                ::immer::default_refcount_policy,
                ::immer::default_lock_policy>;


// preallocated allocator

template<typename T>
struct ENV_VISIBLE pre_allocator
{
};

struct ENV_VISIBLE pre_heap
{
};

using pre_memory_policy =
        ::immer::memory_policy<
                ::immer::heap_policy<pre_heap>,
                ::immer::default_refcount_policy,
                ::immer::default_lock_policy>;


// single threaded allocator

template<typename T>
struct ENV_VISIBLE threaded_allocator
{
};

struct ENV_VISIBLE threaded_heap
{
};

using threaded_memory_policy =
        ::immer::memory_policy<
                ::immer::heap_policy<threaded_heap>,
                ::immer::default_refcount_policy,
                ::immer::no_lock_policy>;

} // namespace alloc


// Initialization -------------------------------------------------------------

namespace
{
ENV_HIDDEN ::std::once_flag initialize_flag{};
ENV_HIDDEN ::std::once_flag finalize_flag{};

ENV_HIDDEN struct scoped_initializer
{
    inline scoped_initializer()
    {
        ::std::call_once(
                initialize_flag,
                [] {
                    mi_option_enable(mi_option_reserve_huge_os_pages);
                    mi_option_enable(mi_option_eager_commit);
                    mi_option_enable(mi_option_eager_region_commit);
                });
    }

    inline ~scoped_initializer()
    {
        ::std::call_once(finalize_flag, [] {});
    }
} initializer{}; // NOLINT(cert-err58-cpp)
} // namespace


// Std containers -------------------------------------------------------------

namespace std
{
using namespace ::std;

namespace alloc
{
template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using deque [[maybe_unused]] = deque<T, TAlloc>;

template<typename T, typename TSeq = deque<T>>
using queue [[maybe_unused]] = queue<T, TSeq>;

template<typename T, typename TSeq = deque<T>>
using stack [[maybe_unused]] = stack<T, TSeq>;


template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using list [[maybe_unused]] = list<T, TAlloc>;

template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using forward_list [[maybe_unused]] = forward_list<T, TAlloc>;


template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using vector [[maybe_unused]] = vector<T, TAlloc>;


template<typename T,
         typename TCompare = std::less<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using set [[maybe_unused]] = set<T, TCompare, TAlloc>;

template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using unordered_set [[maybe_unused]] = unordered_set<T, THash, TEquals, TAlloc>;

template<typename T,
         typename TCompare = std::less<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using map [[maybe_unused]] = map<T, TCompare, TAlloc>;

template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using unordered_map [[maybe_unused]] = unordered_map<T, THash, TEquals, TAlloc>;
} // namespace alloc
} // namespace std


// Immutable containers -------------------------------------------------------

namespace immut
{
using namespace ::immer;

namespace alloc
{
template<typename T, typename TMem = ::env::alloc::memory_policy>
using box [[maybe_unused]] = box<T, TMem>;


template<typename T, typename TMem = ::env::alloc::memory_policy>
using array [[maybe_unused]] = array<T, TMem>;

template<typename T,
         typename TMem = ::env::alloc::memory_policy,
         detail::rbts::bits_t Bits = default_bits,
         detail::rbts::bits_t BitsLeaf =
                 detail::rbts::derive_bits_leaf<T, TMem, Bits>>
using vector [[maybe_unused]] = vector<T, TMem, Bits, BitsLeaf>;

template<typename T,
         typename TMem = ::env::alloc::memory_policy,
         detail::rbts::bits_t Bits = default_bits,
         detail::rbts::bits_t BitsLeaf =
                 detail::rbts::derive_bits_leaf<T, TMem, Bits>>
using flex_vector [[maybe_unused]] = flex_vector<T, TMem, Bits, BitsLeaf>;


template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TMem = ::env::alloc::memory_policy,
         ::immer::detail::hamts::bits_t Bits = default_bits>
using set [[maybe_unused]] = set<T, THash, TEquals, TMem, Bits>;

template<typename TKey,
         typename TValue,
         typename THash = std::hash<TKey>,
         typename TEquals = std::equal_to<TKey>,
         typename TMem = ::env::alloc::memory_policy,
         ::immer::detail::hamts::bits_t Bits = default_bits>
using map [[maybe_unused]] = map<TKey, TValue, THash, TEquals, TMem, Bits>;
} // namespace alloc
} // namespace immut


// Syntactic sugar ------------------------------------------------------------

namespace literals
{
using namespace ::std::literals;
using namespace ::std::string_literals;
using namespace ::std::string_view_literals;
using namespace ::std::chrono_literals;

using namespace ::boost::hana::literals;
} // namespace literals

namespace placeholder
{
using ::boost::hana::_;
} // namespace placeholder

namespace syntax
{
using namespace literals;
using namespace placeholder;
} // namespace syntax


// Namespaces -----------------------------------------------------------------

namespace meta = ::boost::hana;
namespace json = ::nlohmann;

namespace trans
{
using namespace ::zug;
}
} // namespace env


/* ----------------------------------------------------------------------------
 * Extensions
 * ----------------------------------------------------------------------------
 */

// hack for templates using accessors

namespace boost::hana
{
template<typename TTag>
struct accessors_impl<
        TTag, when<struct_detail::is_valid<typename TTag::sample>::value>>
{
private:
    static constexpr auto sample_inner =
            TTag::sample::hana_accessors_impl::apply();

    template<typename T>
    static constexpr decltype(T::hana_accessors_impl::apply()) inner =
            T::hana_accessors_impl::apply();

    template<std::size_t... I, typename... T>
    static constexpr decltype(auto) _apply(
            detail::basic_tuple_impl<std::index_sequence<I...>, T...>
                    _sample_inner)
    {
        constexpr auto getter =
                [](auto i, auto&& x) {
                    using type = decltype(x);
                    using unqualified =
                            std::remove_reference_t<
                                    std::remove_cv_t<type>>;

                    return second(inner<unqualified>[i])(std::forward<type>(x));
                };

        return make_tuple(
                make_pair(first(detail::ebo_get<detail::bti<I>>(_sample_inner)),
                          partial(getter, size_c<I>))...);
    }

public:
    static constexpr decltype(auto) apply()
    {
        return _apply(sample_inner.storage_);
    }
};
} // namespace boost::hana


#endif // ENV_INCLUDED
