#ifndef ENV_TYPECLASSES_HPP
#define ENV_TYPECLASSES_HPP


// TODO: tests

// data

COND_CHECK_UNARY(
        is_data,
        ENV_STD::is_object<T>,
        neg_vt<is_indirect_gs<T>>);

COND_CONCEPT(data, is_data_gs<C>);


// enum

COND_CONCEPT(enum, ENV_STD::is_enum<C>);


// regular

COND_CHECK_UNARY(
        is_regular,
        is_propagated_gs<T>, is_key_gs<T>);


#endif // ENV_TYPECLASSES_HPP
