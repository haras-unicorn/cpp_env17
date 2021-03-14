#ifndef ENV_TOKENS_HPP
#define ENV_TOKENS_HPP


// basic

#define SKIP
#define EMPTY ()
#define COMMA ,
#define OPEN (
#define CLOSE )

#define SINGLE(_) _
#define PACK(...) __VA_ARGS__
#define ERASE(_)
#define CONSUME(...)

// strings

#define CAT_IMPL(_lhs, _rhs) _lhs##_rhs
#define CAT(_lhs, _rhs) CAT_IMPL(_lhs, _rhs)

#define INTER_IMPL(_lhs, _middle, _rhs) _lhs##_middle##_rhs
#define INTER(_lhs, _middle, _rhs) INTER_IMPL(_lhs, _middle, _rhs)

#define STRING_IMPL(...) #__VA_ARGS__
#define STRING_VAR(...) STRING_IMPL(__VA_ARGS__)
#define STRING(...) STRING_VAR(__VA_ARGS__)

// spread

#define SPREAD_IMPL(_tuple) SINGLE(PACK _tuple)
#define SPREAD(_tuple) SPREAD_IMPL(_tuple)


#endif // ENV_TOKENS_HPP
