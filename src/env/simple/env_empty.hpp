#ifndef ENV_EMPTY_HPP
#define ENV_EMPTY_HPP


strct empty_t
{
    DECL_THIS(empty_t);

    CMP_VALIDITY { ret true; };
    CMP_HASH { ret 0_h; };
    CMP_EQUALITY { ret true; };
    CMP_COMPARISON { ret false; };

    DEFAULT_LIFE(empty_t, CMP);
};


#endif // ENV_EMPTY_HPP
