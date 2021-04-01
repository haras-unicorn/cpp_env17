#ifndef ENV_TYPECLASSES_HPP
#define ENV_TYPECLASSES_HPP


// data

COND_CHECK_UNARY
(
        is_data,
        (ENV_STD::is_object_v < T > ) &&
        (!is_indirect_g < T > )
);

COND_CONCEPT(data, is_data_g<C>);


// enum

COND_CONCEPT(enum, ENV_STD::is_enum_v < C >);


// regular

COND_CHECK_UNARY
(
        is_regular,
        (is_propagated_g<T>) &&
        (is_key_g < T > )
);


#endif // ENV_TYPECLASSES_HPP
