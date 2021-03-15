#ifndef ENV_COMPARISON_HPP
#define ENV_COMPARISON_HPP


// names and getter calls

#define IS_VALID_NAME is_valid
#define CHECK_IS_VALID(_type, _subject) (_subject.IS_VALID_NAME())

#define HASH_NAME hash
#define GET_HASH(_type, _subject) (ENV::hash(_subject))

#define EQUALS_NAME equals
#define CHECK_EQUALS(_type, _lhs, _rhs) (_lhs.EQUALS_NAME(_rhs))
#define CHECK_TEMPLATE_EQUALS(_type, _lhs, _rhs) (_lhs.EQUALS_NAME(_rhs))

#define HASH_EQ_NAME hash_eq
#define CHECK_HASH_EQ(_type, _lhs, _rhs) (_lhs.HASH_EQ_NAME(_rhs))
#define CHECK_TEMPLATE_HASH_EQ(_type, _lhs, _rhs) (_lhs.HASH_EQ_NAME(_rhs))

#define LESS_NAME less
#define CHECK_LESS(_type, _lhs, _rhs) (_lhs.LESS_NAME(_rhs))
#define CHECK_TEMPLATE_LESS(_type, _lhs, _rhs) (_lhs.LESS_NAME(_rhs))


// non tmp modular

// validity

#define DEFINE_NULLPTR_EQ_OPERATOR(_type, _pre, _post)                    \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::flag_t op == (ENV::nil_t) const _post \
    {                                                                     \
        ret !CHECK_IS_VALID(_type, (*this));                              \
    }

#define DEFINE_NULLPTR_NE_OPERATOR(_type, _pre, _post)                    \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::flag_t op != (ENV::nil_t) const _post \
    {                                                                     \
        ret CHECK_IS_VALID(_type, (*this));                               \
    }

#define DEFINE_IS_INVALID(_type, _pre, _post)                       \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::flag_t is_invalid() const _post \
    {                                                               \
        ret !CHECK_IS_VALID(_type, (*this));                        \
    }

#define DEFINE_BOOL_OPERATOR(_type, _pre, _post)                     \
    [[RETURN_ATTRIBUTES]] _pre inl explicit op ENV::flag_t() const _post \
    {                                                                \
        ret CHECK_IS_VALID(_type, (*this));                          \
    }

#define DEFINE_STATIC_IS_VALID(_type, _pre, _post)                                                   \
    [[RETURN_ATTRIBUTES]] _pre static inl ENV::flag_t IS_VALID_NAME(const SPREAD(_type) & subject) _post \
    {                                                                                                \
        ret CHECK_IS_VALID(_type, subject);                                                          \
    }

#define DECLARE_IS_VALID_FUNCTION(_type, _pre, _post) \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::flag_t IS_VALID_NAME() const _post

// hash

#define DEFINE_CALCULATE_HASH(_type, _pre, _post)                       \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::hash_t calculate_hash() const _post \
    {                                                                   \
        ret GET_HASH(_type, (*this));                                   \
    }

#define DEFINE_HASH_OPERATOR(_type, _pre, _post)                     \
    [[RETURN_ATTRIBUTES]] _pre inl explicit op ENV::hash_t() const _post \
    {                                                                \
        ret GET_HASH(_type, (*this));                                \
    }

#define DEFINE_STATIC_HASH(_type, _pre, _post)                                                   \
    [[RETURN_ATTRIBUTES]] _pre static inl ENV::hash_t HASH_NAME(const SPREAD(_type) & subject) _post \
    {                                                                                            \
        ret GET_HASH(_type, subject);                                                            \
    }

#define DECLARE_HASH_FUNCTION(_type, _pre, _post) \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::hash_t HASH_NAME() const _post


// possibly tmp modular

#define DECLARE_BINARY_CHECK(_name, _type, _pre, _post) \
    [[RETURN_ATTRIBUTES]] _pre inl ENV::flag_t              \
    _name(const SPREAD(_type) & rhs) const _post

#define DECLARE_STATIC_BINARY_CHECK(_name, _type_lhs, _type_rhs, _pre, _post) \
    [[RETURN_ATTRIBUTES]] _pre static inl ENV::flag_t                             \
    _name(const SPREAD(_type_lhs) & lhs, const SPREAD(_type_rhs) & rhs) _post

#define DECLARE_NO_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_BINARY_CHECK(_name, _type, _pre, _post)

#define DECLARE_TEMPLATE_BINARY_CHECK(_condition, _name, _type, _pre, _post) \
    COND_TMP_UNARY(_condition)                                               \
    DECLARE_BINARY_CHECK(_name, (T), _pre, _post)

#define DECLARE_STATIC_TEMPLATE_BINARY_CHECK(_condition, _name, _type, _pre, _post) \
    COND_TMP_BINARY(_condition)                                                     \
    DECLARE_STATIC_BINARY_CHECK(_name, (TLhs), (TRhs), _pre, _post)

#define HASH_EQ_TEMPLATE_CONDITION(_type) ENV::are_hash_eq_compatible_g<SPREAD(_type), T>

#define STATIC_HASH_EQ_TEMPLATE_CONDITION(_type) ENV::are_hash_eq_compatible_g<TLhs, TRhs>

#define EQUALITY_TEMPLATE_CONDITION(_type) ENV::are_equality_compatible_g<SPREAD(_type), T>

#define STATIC_EQUALITY_TEMPLATE_CONDITION(_type) ENV::are_equality_compatible_g<TLhs, TRhs>

#define COMPARISON_TEMPLATE_CONDITION(_type) ENV::are_comparison_compatible_g<SPREAD(_type), T>

#define STATIC_COMPARISON_TEMPLATE_CONDITION(_type) ENV::are_comparison_compatible_g<TLhs, TRhs>

#define DECLARE_HASH_EQ_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_TEMPLATE_BINARY_CHECK(                                       \
        (HASH_EQ_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

#define DECLARE_HASH_EQ_STATIC_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_STATIC_TEMPLATE_BINARY_CHECK(                                       \
        (STATIC_HASH_EQ_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

#define DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_TEMPLATE_BINARY_CHECK(                                        \
        (EQUALITY_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

#define DECLARE_EQUALITY_STATIC_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_STATIC_TEMPLATE_BINARY_CHECK(                                        \
        (STATIC_EQUALITY_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

#define DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_TEMPLATE_BINARY_CHECK(                                          \
        (COMPARISON_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

#define DECLARE_COMPARISON_STATIC_TEMPLATE_BINARY_CHECK(_name, _type, _pre, _post) \
    DECLARE_STATIC_TEMPLATE_BINARY_CHECK(                                          \
        (STATIC_COMPARISON_TEMPLATE_CONDITION(_type)),                             \
        _name, _type, _pre, _post)

// hash

#define DEFINE_HASH_EQ(_type, _pre, _post)                    \
    DECLARE_BINARY_CHECK(HASH_EQ_NAME, _type, _pre, _post)    \
    {                                                         \
        ret GET_HASH(_type, (*this)) == GET_HASH(_type, rhs); \
    }

#define DEFINE_HASH_IS_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(hash_is_equal_to, _type, _pre, _post) \
    {                                                          \
        ret CHECK_HASH_EQ(_type, (*this), rhs);                \
    }

#define DEFINE_HASH_IS_NOT_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(hash_is_not_equal_to, _type, _pre, _post) \
    {                                                              \
        ret !CHECK_HASH_EQ(_type, (*this), rhs);                   \
    }

#define DEFINE_STATIC_HASH_EQ(_type, _pre, _post)                        \
    DECLARE_STATIC_BINARY_CHECK(HASH_EQ_NAME, _type, _type, _pre, _post) \
    {                                                                    \
        ret CHECK_HASH_EQ(_type, lhs, rhs);                              \
    }

#define DEFINE_TEMPLATE_HASH_EQ(_type, _pre, _post)                         \
    DECLARE_HASH_EQ_TEMPLATE_BINARY_CHECK(HASH_EQ_NAME, _type, _pre, _post) \
    {                                                                       \
        ret GET_HASH(_type, (*this)) == GET_HASH(_type, rhs);               \
    }

#define DEFINE_TEMPLATE_HASH_IS_EQUAL_TO(_type, _pre, _post)                    \
    DECLARE_HASH_EQ_TEMPLATE_BINARY_CHECK(hash_is_equal_to, _type, _pre, _post) \
    {                                                                           \
        ret CHECK_TEMPLATE_HASH_EQ(_type, (*this), rhs);                        \
    }

#define DEFINE_TEMPLATE_HASH_IS_NOT_EQUAL_TO(_type, _pre, _post)                    \
    DECLARE_HASH_EQ_TEMPLATE_BINARY_CHECK(hash_is_not_equal_to, _type, _pre, _post) \
    {                                                                               \
        ret !CHECK_TEMPLATE_HASH_EQ(_type, (*this), rhs);                           \
    }

#define DEFINE_STATIC_TEMPLATE_HASH_EQ(_type, _pre, _post)                         \
    DECLARE_HASH_EQ_STATIC_TEMPLATE_BINARY_CHECK(HASH_EQ_NAME, _type, _pre, _post) \
    {                                                                              \
        ret CHECK_TEMPLATE_HASH_EQ(_type, lhs, rhs);                               \
    }

// equality

#define DEFINE_EQ_OPERATOR(_type, _pre, _post)      \
    DECLARE_BINARY_CHECK(op ==, _type, _pre, _post) \
    {                                               \
        ret CHECK_EQUALS(_type, (*this), rhs);      \
    }

#define DEFINE_NE_OPERATOR(_type, _pre, _post)      \
    DECLARE_BINARY_CHECK(op !=, _type, _pre, _post) \
    {                                               \
        ret !CHECK_EQUALS(_type, (*this), rhs);     \
    }

#define DEFINE_IS_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_equal_to, _type, _pre, _post) \
    {                                                     \
        ret CHECK_EQUALS(_type, (*this), rhs);            \
    }

#define DEFINE_IS_NOT_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_not_equal_to, _type, _pre, _post) \
    {                                                         \
        ret !CHECK_EQUALS(_type, (*this), rhs);               \
    }

#define DEFINE_STATIC_EQUALS(_type, _pre, _post)                        \
    DECLARE_STATIC_BINARY_CHECK(EQUALS_NAME, _type, _type, _pre, _post) \
    {                                                                   \
        ret CHECK_EQUALS(_type, lhs, rhs);                              \
    }

#define DECLARE_EQUALS_FUNCTION(_type, _pre, _post) \
    DECLARE_BINARY_CHECK(EQUALS_NAME, _type, _pre, _post)

#define DEFINE_TEMPLATE_EQ_OPERATOR(_type, _pre, _post)               \
    DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(op ==, _type, _pre, _post) \
    {                                                                 \
        ret CHECK_TEMPLATE_EQUALS(_type, (*this), rhs);               \
    }

#define DEFINE_TEMPLATE_NE_OPERATOR(_type, _pre, _post)               \
    DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(op !=, _type, _pre, _post) \
    {                                                                 \
        ret !CHECK_TEMPLATE_EQUALS(_type, (*this), rhs);              \
    }

#define DEFINE_TEMPLATE_IS_EQUAL_TO(_type, _pre, _post)                     \
    DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(is_equal_to, _type, _pre, _post) \
    {                                                                       \
        ret CHECK_TEMPLATE_EQUALS(_type, (*this), rhs);                     \
    }

#define DEFINE_TEMPLATE_IS_NOT_EQUAL_TO(_type, _pre, _post)                     \
    DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(is_not_equal_to, _type, _pre, _post) \
    {                                                                           \
        ret !CHECK_TEMPLATE_EQUALS(_type, (*this), rhs);                        \
    }

#define DEFINE_TEMPLATE_STATIC_EQUALS(_type, _pre, _post)                          \
    DECLARE_EQUALITY_STATIC_TEMPLATE_BINARY_CHECK(EQUALS_NAME, _type, _pre, _post) \
    {                                                                              \
        ret CHECK_TEMPLATE_EQUALS(_type, lhs, rhs);                                \
    }

#define DECLARE_TEMPLATE_EQUALS_FUNCTION(_type, _pre, _post) \
    DECLARE_EQUALITY_TEMPLATE_BINARY_CHECK(EQUALS_NAME, _type, _pre, _post)

// comparison

#define DEFINE_LESS_OPERATOR(_type, _pre, _post)   \
    DECLARE_BINARY_CHECK(op <, _type, _pre, _post) \
    {                                              \
        ret CHECK_LESS(_type, (*this), rhs);       \
    }

#define DEFINE_IS_LESS_THAN(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_less_than, _type, _pre, _post) \
    {                                                      \
        ret CHECK_LESS(_type, (*this), rhs);               \
    }

#define DEFINE_GREATER_OPERATOR(_type, _pre, _post) \
    DECLARE_BINARY_CHECK(op >, _type, _pre, _post)  \
    {                                               \
        ret CHECK_LESS(_type, rhs, (*this));        \
    }

#define DEFINE_IS_GREATER_THAN(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_greater_than, _type, _pre, _post) \
    {                                                         \
        ret CHECK_LESS(_type, rhs, (*this));                  \
    }

#define DEFINE_LESS_OR_EQUAL_OPERATOR(_type, _pre, _post) \
    DECLARE_BINARY_CHECK(op <=, _type, _pre, _post)       \
    {                                                     \
        ret !CHECK_LESS(_type, rhs, (*this));             \
    }

#define DEFINE_IS_LESS_THAN_OR_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_less_than_or_equal_to, _type, _pre, _post) \
    {                                                                  \
        ret !CHECK_LESS(_type, rhs, (*this));                          \
    }

#define DEFINE_GREATER_OR_EQUAL_OPERATOR(_type, _pre, _post) \
    DECLARE_BINARY_CHECK(op >=, _type, _pre, _post)          \
    {                                                        \
        ret !CHECK_LESS(_type, (*this), rhs);                \
    }

#define DEFINE_IS_GREATER_THAN_OR_EQUAL_TO(_type, _pre, _post)            \
    DECLARE_BINARY_CHECK(is_greater_than_or_equal_to, _type, _pre, _post) \
    {                                                                     \
        ret !CHECK_LESS(_type, (*this), rhs);                             \
    }

#define DEFINE_STATIC_LESS(_type, _pre, _post)                        \
    DECLARE_STATIC_BINARY_CHECK(LESS_NAME, _type, _type, _pre, _post) \
    {                                                                 \
        ret CHECK_LESS(_type, lhs, rhs);                              \
    }

#define DECLARE_LESS_FUNCTION(_type, _pre, _post) \
    DECLARE_BINARY_CHECK(LESS_NAME, _type, _pre, _post)

#define DEFINE_TEMPLATE_LESS_OPERATOR(_type, _pre, _post)              \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(op <, _type, _pre, _post) \
    {                                                                  \
        ret CHECK_TEMPLATE_LESS(_type, (*this), rhs);                  \
    }

#define DEFINE_TEMPLATE_IS_LESS_THAN(_type, _pre, _post)                       \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(is_less_than, _type, _pre, _post) \
    {                                                                          \
        ret CHECK_TEMPLATE_LESS(_type, (*this), rhs);                          \
    }

#define DEFINE_TEMPLATE_GREATER_OPERATOR(_type, _pre, _post)           \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(op >, _type, _pre, _post) \
    {                                                                  \
        ret CHECK_TEMPLATE_LESS(_type, rhs, (*this));                  \
    }

#define DEFINE_TEMPLATE_IS_GREATER_THAN(_type, _pre, _post)                       \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(is_greater_than, _type, _pre, _post) \
    {                                                                             \
        ret CHECK_TEMPLATE_LESS(_type, rhs, (*this));                             \
    }

#define DEFINE_TEMPLATE_LESS_OR_EQUAL_OPERATOR(_type, _pre, _post)      \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(op <=, _type, _pre, _post) \
    {                                                                   \
        ret !CHECK_TEMPLATE_LESS(_type, rhs, (*this));                  \
    }

#define DEFINE_TEMPLATE_IS_LESS_THAN_OR_EQUAL_TO(_type, _pre, _post)                       \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(is_less_than_or_equal_to, _type, _pre, _post) \
    {                                                                                      \
        ret !CHECK_TEMPLATE_LESS(_type, rhs, (*this));                                     \
    }

#define DEFINE_TEMPLATE_GREATER_OR_EQUAL_OPERATOR(_type, _pre, _post)   \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(op >=, _type, _pre, _post) \
    {                                                                   \
        ret !CHECK_TEMPLATE_LESS(_type, (*this), rhs);                  \
    }

#define DEFINE_TEMPLATE_IS_GREATER_THAN_OR_EQUAL_TO(_type, _pre, _post)                       \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(is_greater_than_or_equal_to, _type, _pre, _post) \
    {                                                                                         \
        ret !CHECK_TEMPLATE_LESS(_type, (*this), rhs);                                        \
    }

#define DEFINE_TEMPLATE_STATIC_LESS(_type, _pre, _post)                            \
    DECLARE_COMPARISON_STATIC_TEMPLATE_BINARY_CHECK(LESS_NAME, _type, _pre, _post) \
    {                                                                              \
        ret CHECK_TEMPLATE_LESS(_type, lhs, rhs);                                  \
    }

#define DECLARE_TEMPLATE_LESS_FUNCTION(_type, _pre, _post) \
    DECLARE_COMPARISON_TEMPLATE_BINARY_CHECK(LESS_NAME, _type, _pre, _post)


// This is too much to test and most of the untested stuff is just boilerplate anyway.

ENV_CLANG_SUPPRESS_PUSH("OCUnusedMacroInspection")


// non tmp compound

// validity

#define ELABORATE_VALIDITY_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_NULLPTR_EQ_OPERATOR(_type, _pre, _post)                           \
    DEFINE_NULLPTR_NE_OPERATOR(_type, _pre, _post)                           \
    DEFINE_IS_INVALID(_type, _pre, _post)                                    \
    DEFINE_BOOL_OPERATOR(_type, _pre, _post)                                 \
    DEFINE_STATIC_IS_VALID(_type, _pre_static, _post_static)                 \
    DECLARE_IS_VALID_FUNCTION(_type, _pre, _post)

#define ELABORATE_VALIDITY(_pre, _post, _pre_static, _post_static) \
    ELABORATE_VALIDITY_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_VALIDITY_ON(_type) ELABORATE_VALIDITY_ON(_type, cmp, noex, cmp, noex)
#define NOEX_VALIDITY_ON(_type) ELABORATE_VALIDITY_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_VALIDITY_ON(_type) ELABORATE_VALIDITY_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_VALIDITY_ON(_type) ELABORATE_VALIDITY_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_VALIDITY CMP_VALIDITY_ON((_this_t))
#define NOEX_VALIDITY NOEX_VALIDITY_ON((_this_t))
#define SUPER_VALIDITY SUPER_VALIDITY_ON((_this_t))
#define SUB_VALIDITY SUB_VALIDITY_ON((_this_t))

// hash

#define ELABORATE_HASH_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_CALCULATE_HASH(_type, _pre, _post)                            \
    DEFINE_HASH_OPERATOR(_type, _pre, _post)                             \
    DEFINE_STATIC_HASH_EQ(_type, _pre, _post)                            \
    DEFINE_HASH_EQ(_type, _pre, _post)                                   \
    DEFINE_HASH_IS_EQUAL_TO(_type, _pre, _post)                          \
    DEFINE_HASH_IS_NOT_EQUAL_TO(_type, _pre, _post)                      \
    DEFINE_STATIC_HASH(_type, _pre_static, _post_static)                 \
    DECLARE_HASH_FUNCTION(_type, _pre, _post)

#define ELABORATE_HASH(_pre, _post, _pre_static, _post_static) \
    ELABORATE_HASH_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_HASH_ON(_type) ELABORATE_HASH_ON(_type, cmp, noex, cmp, noex)
#define NOEX_HASH_ON(_type) ELABORATE_HASH_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_HASH_ON(_type) ELABORATE_HASH_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_HASH_ON(_type) ELABORATE_HASH_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_HASH CMP_HASH_ON((_this_t))
#define NOEX_HASH NOEX_HASH_ON((_this_t))
#define SUPER_HASH SUPER_HASH_ON((_this_t))
#define SUB_HASH SUB_HASH_ON((_this_t))


// possibly tmp compound

// hash

#define ELABORATE_TMP_HASH_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_CALCULATE_HASH(_type, _pre, _post)                                \
    DEFINE_HASH_OPERATOR(_type, _pre, _post)                                 \
    DEFINE_STATIC_TEMPLATE_HASH_EQ(_type, _pre, _post)                       \
    DEFINE_TEMPLATE_HASH_EQ(_type, _pre, _post)                              \
    DEFINE_TEMPLATE_HASH_IS_EQUAL_TO(_type, _pre, _post)                     \
    DEFINE_TEMPLATE_HASH_IS_NOT_EQUAL_TO(_type, _pre, _post)                 \
    DEFINE_STATIC_HASH(_type, _pre_static, _post_static)                     \
    DECLARE_HASH_FUNCTION(_type, _pre, _post)

#define ELABORATE_TMP_HASH(_pre, _post, _pre_static, _post_static) \
    ELABORATE_TMP_HASH_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_TMP_HASH_ON(_type) ELABORATE_TMP_HASH_ON(_type, cmp, noex, cmp, noex)
#define NOEX_TMP_HASH_ON(_type) ELABORATE_TMP_HASH_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_TMP_HASH_ON(_type) ELABORATE_TMP_HASH_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_TMP_HASH_ON(_type) ELABORATE_TMP_HASH_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_TMP_HASH CMP_TMP_HASH_ON((_this_t))
#define NOEX_TMP_HASH NOEX_TMP_HASH_ON((_this_t))
#define SUPER_TMP_HASH SUPER_TMP_HASH_ON((_this_t))
#define SUB_TMP_HASH SUB_TMP_HASH_ON((_this_t))

// equality

#define ELABORATE_EQUALITY_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_EQ_OPERATOR(_type, _pre, _post)                                   \
    DEFINE_NE_OPERATOR(_type, _pre, _post)                                   \
    DEFINE_IS_EQUAL_TO(_type, _pre, _post)                                   \
    DEFINE_IS_NOT_EQUAL_TO(_type, _pre, _post)                               \
    DEFINE_STATIC_EQUALS(_type, _pre_static, _post_static)                   \
    DECLARE_EQUALS_FUNCTION(_type, _pre, _post)

#define ELABORATE_EQUALITY(_pre, _post, _pre_static, _post_static) \
    ELABORATE_EQUALITY_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_EQUALITY_ON(_type) ELABORATE_EQUALITY_ON(_type, cmp, noex, cmp, noex)
#define NOEX_EQUALITY_ON(_type) ELABORATE_EQUALITY_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_EQUALITY_ON(_type) ELABORATE_EQUALITY_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_EQUALITY_ON(_type) ELABORATE_EQUALITY_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_EQUALITY CMP_EQUALITY_ON((_this_t))
#define NOEX_EQUALITY NOEX_EQUALITY_ON((_this_t))
#define SUPER_EQUALITY SUPER_EQUALITY_ON((_this_t))
#define SUB_EQUALITY SUB_EQUALITY_ON((_this_t))

#define ELABORATE_TMP_EQUALITY_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_TEMPLATE_EQ_OPERATOR(_type, _pre, _post)                              \
    DEFINE_TEMPLATE_NE_OPERATOR(_type, _pre, _post)                              \
    DEFINE_TEMPLATE_IS_EQUAL_TO(_type, _pre, _post)                              \
    DEFINE_TEMPLATE_IS_NOT_EQUAL_TO(_type, _pre, _post)                          \
    DEFINE_TEMPLATE_STATIC_EQUALS(_type, _pre_static, _post_static)              \
    DECLARE_TEMPLATE_EQUALS_FUNCTION(_type, _pre, _post)

#define ELABORATE_TMP_EQUALITY(_pre, _post, _pre_static, _post_static) \
    ELABORATE_TMP_EQUALITY_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_TMP_EQUALITY_ON(_type) ELABORATE_TMP_EQUALITY_ON(_type, cmp, noex, cmp, noex)
#define NOEX_TMP_EQUALITY_ON(_type) ELABORATE_TMP_EQUALITY_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_TMP_EQUALITY_ON(_type) ELABORATE_TMP_EQUALITY_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_TMP_EQUALITY_ON(_type) ELABORATE_TMP_EQUALITY_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_TMP_EQUALITY CMP_TMP_EQUALITY_ON((_this_t))
#define NOEX_TMP_EQUALITY NOEX_TMP_EQUALITY_ON((_this_t))
#define SUPER_TMP_EQUALITY SUPER_TMP_EQUALITY_ON((_this_t))
#define SUB_TMP_EQUALITY SUB_TMP_EQUALITY_ON((_this_t))

// comparison

#define ELABORATE_COMPARISON_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_LESS_OPERATOR(_type, _pre, _post)                                   \
    DEFINE_IS_LESS_THAN(_type, _pre, _post)                                    \
    DEFINE_GREATER_OPERATOR(_type, _pre, _post)                                \
    DEFINE_IS_GREATER_THAN(_type, _pre, _post)                                 \
    DEFINE_LESS_OR_EQUAL_OPERATOR(_type, _pre, _post)                          \
    DEFINE_IS_LESS_THAN_OR_EQUAL_TO(_type, _pre, _post)                        \
    DEFINE_GREATER_OR_EQUAL_OPERATOR(_type, _pre, _post)                       \
    DEFINE_IS_GREATER_THAN_OR_EQUAL_TO(_type, _pre, _post)                     \
    DEFINE_STATIC_LESS(_type, _pre_static, _post_static)                       \
    DECLARE_LESS_FUNCTION(_type, _pre, _post)

#define ELABORATE_COMPARISON(_pre, _post, _pre_static, _post_static) \
    ELABORATE_COMPARISON_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_COMPARISON_ON(_type) ELABORATE_COMPARISON_ON(_type, cmp, noex, cmp, noex)
#define NOEX_COMPARISON_ON(_type) ELABORATE_COMPARISON_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_COMPARISON_ON(_type) ELABORATE_COMPARISON_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_COMPARISON_ON(_type) ELABORATE_COMPARISON_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_COMPARISON CMP_COMPARISON_ON((_this_t))
#define NOEX_COMPARISON NOEX_COMPARISON_ON((_this_t))
#define SUPER_COMPARISON SUPER_COMPARISON_ON((_this_t))
#define SUB_COMPARISON SUB_COMPARISON_ON((_this_t))

#define ELABORATE_TMP_COMPARISON_ON(_type, _pre, _post, _pre_static, _post_static) \
    DEFINE_TEMPLATE_LESS_OPERATOR(_type, _pre, _post)                              \
    DEFINE_TEMPLATE_IS_LESS_THAN(_type, _pre, _post)                               \
    DEFINE_TEMPLATE_GREATER_OPERATOR(_type, _pre, _post)                           \
    DEFINE_TEMPLATE_IS_GREATER_THAN(_type, _pre, _post)                            \
    DEFINE_TEMPLATE_LESS_OR_EQUAL_OPERATOR(_type, _pre, _post)                     \
    DEFINE_TEMPLATE_IS_LESS_THAN_OR_EQUAL_TO(_type, _pre, _post)                   \
    DEFINE_TEMPLATE_GREATER_OR_EQUAL_OPERATOR(_type, _pre, _post)                  \
    DEFINE_TEMPLATE_IS_GREATER_THAN_OR_EQUAL_TO(_type, _pre, _post)                \
    DEFINE_TEMPLATE_STATIC_LESS(_type, _pre_static, _post_static)                  \
    DECLARE_TEMPLATE_LESS_FUNCTION(_type, _pre, _post)

#define ELABORATE_TMP_COMPARISON(_pre, _post, _pre_static, _post_static) \
    ELABORATE_TMP_COMPARISON_ON((_this_t), _pre, _post, _pre_static, _post_static)

#define CMP_TMP_COMPARISON_ON(_type) \
    ELABORATE_TMP_COMPARISON_ON(_type, cmp, noex, cmp, noex)
#define NOEX_TMP_COMPARISON_ON(_type) \
    ELABORATE_TMP_COMPARISON_ON(_type, SKIP, noex, SKIP, noex)
#define SUPER_TMP_COMPARISON_ON(_type) \
    ELABORATE_TMP_COMPARISON_ON(_type, virtual, SKIP, SKIP, SKIP)
#define SUB_TMP_COMPARISON_ON(_type) \
    ELABORATE_TMP_COMPARISON_ON(_type, SKIP, override, SKIP, SKIP)

#define CMP_TMP_COMPARISON CMP_TMP_COMPARISON_ON((_this_t))
#define NOEX_TMP_COMPARISON NOEX_TMP_COMPARISON_ON((_this_t))
#define SUPER_TMP_COMPARISON SUPER_TMP_COMPARISON_ON((_this_t))
#define SUB_TMP_COMPARISON SUB_TMP_COMPARISON_ON((_this_t))


// compatibility

#define COND_HASH_EQ_COMPAT(...)                                         \
        COND_CLASS_CHECK_UNARY(is_hash_eq_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_hash_eq_compatible_with_gs

#define COND_EQUALITY_COMPAT(...)                                         \
        COND_CLASS_CHECK_UNARY(is_equality_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_equality_compatible_with_gs

#define COND_COMPARISON_COMPAT(...)                                         \
        COND_CLASS_CHECK_UNARY(is_comparison_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_comparison_compatible_with_gs

#define ELABORATE_COND_COMPAT(_hash_condition, _equality_condition, _comparison_condition) \
        COND_HASH_EQ_COMPAT(_hash_condition);                                              \
        COND_EQUALITY_COMPAT(_equality_condition);                                         \
        COND_COMPARISON_COMPAT(_comparison_condition)

#define COND_COMPAT(...) ELABORATE_COND_COMPAT(PACK(__VA_ARGS__), PACK(__VA_ARGS__), PACK(__VA_ARGS__))


#define EXPR_HASH_EQ_COMPAT(...)                                         \
        EXPR_CLASS_CHECK_UNARY(is_hash_eq_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_hash_eq_compatible_with_gs

#define EXPR_EQUALITY_COMPAT(...)                                         \
        EXPR_CLASS_CHECK_UNARY(is_equality_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_equality_compatible_with_gs

#define EXPR_COMPARISON_COMPAT(...)                                         \
        EXPR_CLASS_CHECK_UNARY(is_comparison_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_comparison_compatible_with_gs

#define ELABORATE_EXPR_COMPAT(_hash_expression, _equality_expression, _comparison_expression) \
        EXPR_HASH_EQ_COMPAT(_hash_expression);                                                \
        EXPR_EQUALITY_COMPAT(_equality_expression);                                           \
        EXPR_COMPARISON_COMPAT(_comparison_expression)

#define EXPR_COMPAT(...) ELABORATE_EXPR_COMPAT(PACK(__VA_ARGS__), PACK(__VA_ARGS__), PACK(__VA_ARGS__))


#define TYPE_HASH_EQ_COMPAT(...)                                         \
        TYPE_CLASS_CHECK_UNARY(is_hash_eq_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_hash_eq_compatible_with_gs

#define TYPE_EQUALITY_COMPAT(...)                                         \
        TYPE_CLASS_CHECK_UNARY(is_equality_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_equality_compatible_with_gs

#define TYPE_COMPARISON_COMPAT(...)                                         \
        TYPE_CLASS_CHECK_UNARY(is_comparison_compatible_with, __VA_ARGS__); \
        tmp<name, name> friend struct is_comparison_compatible_with_gs

#define ELABORATE_TYPE_COMPAT(_hash_type, _equality_type, _comparison_type) \
        TYPE_HASH_EQ_COMPAT(_hash_type);                                    \
        TYPE_EQUALITY_COMPAT(_equality_type);                               \
        TYPE_COMPARISON_COMPAT(_comparison_type)

#define TYPE_COMPAT(...) ELABORATE_TYPE_COMPAT(PACK(__VA_ARGS__), PACK(__VA_ARGS__), PACK(__VA_ARGS__))


#define COMPAT(_name)                                                    \
        nonced cmp_obj_p bool static _name{true};              \
        COND_COMPAT(T::_name)


ENV_CLANG_SUPPRESS_POP


// traits

COND_CHECK_BINARY(are_hash_eq_compatible, (TLhs::tmp is_hash_eq_compatible_with_g < TRhs >));

COND_CHECK_BINARY(are_equality_compatible, (TLhs::tmp is_equality_compatible_with_g < TRhs >));

COND_CHECK_BINARY(are_comparison_compatible, (TLhs::tmp is_comparison_compatible_with_g < TRhs >));


// tests

ENV_TEST_BEGIN

cls comparable_t
{
    DECL_THIS(comparable_t);
    DECL((int), value);

    con cmp comparable_t(_value_t value) noex: _value{value} { }

    MEM_CMP_GETTER(value);

    CMP_VALIDITY { ret get_value() >= 0; };
    CMP_HASH { ret static_cast<ENV::hash_t>(get_value()); };
    CMP_EQUALITY { ret get_value() == rhs.get_value(); };
    CMP_COMPARISON { ret get_value() < rhs.get_value(); };
};

ENV_TEST_END

ENV_TEST_CASE("comparison")
{
    cmp test::comparable_t comparable_small{10};
    cmp test::comparable_t comparable_big{20};
    nonce(comparable_big);

    SUBCASE("validity")
    {
        REQUIRE(static_cast<bool>(comparable_small));
        REQUIRE(comparable_small.is_valid());
        let null_result = comparable_small != nil;
        nonce(null_result);
        REQUIRE(null_result);
    }

    SUBCASE("hash")
    {
        REQUIRE_FALSE(comparable_small.hash_is_equal_to(comparable_big));
        REQUIRE_NE(comparable_small.hash(), hasher(comparable_big));
        REQUIRE_EQ(comparable_small.hash(), hash(comparable_small));
    }

    SUBCASE("equality")
    {
        REQUIRE_EQ(comparable_small, comparable_small);
        REQUIRE_NE(comparable_small, comparable_big);
    }

    SUBCASE("comparison")
    {
        REQUIRE_GT(comparable_big, comparable_small);
        REQUIRE_LT(comparable_small, comparable_big);
        REQUIRE(comparable_small.is_less_than_or_equal_to(comparable_big));
        REQUIRES(comparable_small.is_greater_than_or_equal_to(comparable_small));
    }
}


// tmp tests

ENV_TEST_BEGIN

COND_TMP((name TValue), ENV_STD::is_integral_v < TValue >)
cls tmp_comparable_gt
{
    DECL_THIS(tmp_comparable_gt);
    DECL((TValue), value);

    con cmp tmp_comparable_gt(TValue value) noex: _value{value} { }

    MEM_CMP_REF_GETTER(value);


    COMPAT(is_tmp_comparable);
    CMP_VALIDITY { ret get_value() >= _value_t{0}; }
    CMP_TMP_HASH { ret scast<hash_t>(get_value()); } // hash not constexpr
    CMP_TMP_EQUALITY { ret get_value() == static_cast<_value_t>(rhs.get_value()); }
    CMP_TMP_COMPARISON { ret get_value() < static_cast<_value_t>(rhs.get_value()); }
};

ENV_TEST_END

ENV_TEST_CASE("template comparison")
{
    cmp test::tmp_comparable_gt<int> comparable_int{30};
    cmp test::tmp_comparable_gt<char> comparable_char{20};

    SUBCASE("validity")
    {
        REQUIRES(static_cast<bool>(comparable_int));
        REQUIRES(comparable_int.is_valid());
        let null_result = comparable_int != nil;
        REQUIRES(null_result);
    }

    SUBCASE("hash")
    {
        REQUIRES_FALSE(comparable_int.hash_is_equal_to(comparable_char));
        REQUIRES(comparable_int.hash() != comparable_char.hash());
        REQUIRES(comparable_int.hash() == comparable_int.hash());
    }

    SUBCASE("equality")
    {
        REQUIRES(comparable_int == comparable_int);
        REQUIRES(comparable_int != comparable_char);
    }

    SUBCASE("comparison")
    {
        REQUIRES(comparable_int > comparable_char);
        REQUIRES(comparable_char < comparable_int);
        REQUIRES(comparable_char.is_less_than_or_equal_to(comparable_int));
        REQUIRES(comparable_int >= comparable_int);
    }
}


#endif // ENV_COMPARISON_HPP
