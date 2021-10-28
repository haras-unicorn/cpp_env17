#ifndef ENV_INCLUDED
#define ENV_INCLUDED


#include <env/macros.hpp>


#if ENV_SHARED || ENV_MODULE
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

/** 
 * @brief Main namespace with a bit of sugar.
 */
namespace env
{
/** 
 * @brief Check whether env is linking properly.
 */
ENV_VISIBLE bool check() noexcept;


namespace literals
{
using namespace ::std::literals;
using namespace ::std::string_literals;
using namespace ::std::string_view_literals;
using namespace ::std::chrono_literals;
} // namespace literals

namespace syntax
{
using namespace literals;
} // namespace syntax
} // namespace env

#endif // ENV_INCLUDED
