#ifndef ENV_GLUE_HPP
#define ENV_GLUE_HPP

#ifndef ENV_ALLOCATOR
#define ENV_ALLOCATOR ENV_STD::allocator
#endif // ENV_ALLOCATOR

#ifndef ENV_DELETER
#define ENV_DELETER ENV_STD::default_delete
#endif // ENV_ALLOCATOR

#endif // ENV_GLUE_HPP
