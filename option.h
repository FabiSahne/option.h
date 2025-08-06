/* ---------------------------------------------------------
 * option.h - a lightweight, macro-generated Option<T> for C
 * --------------------------------------------------------- */
#ifndef _OPTION_H
#define _OPTION_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    None,
    Some,
} Option_Tag;

#define OPTION(type, name)\
typedef struct {\
    Option_Tag tag;\
    union {\
        type some;\
    } payload;\
} Option_##name;\
\
static inline Option_##name option_##name##_some(type val) {\
    Option_##name o = {.tag = Some};\
    o.payload.some = val;\
    return o;\
}\
\
static inline Option_##name option_##name##_none() {\
    return (Option_##name){.tag = None};\
}\
static inline int option_##name##_is_some(const Option_##name* o) {\
    return o->tag == Some;\
}\
\
static inline int option_##name##_is_none(const Option_##name* o) {\
    return o->tag == None;\
}\
\
static inline type option_##name##_unwrap(Option_##name o) {\
    if (o.tag == None) {\
        fprintf(stderr, "option_" #name "_unwrap: called on a None value\n");\
        abort();\
    }\
    return o.payload.some;\
}\
static inline type* option_##name##_as_ptr(Option_##name* o) {\
    return o->tag == Some ? &o->payload.some : NULL;\
}\
\
static inline Option_##name option_##name##_and_then(Option_##name o, Option_##name (*f)(type)) {\
    return o.tag == Some ? f(o.payload.some) : o;\
}\
\
static inline Option_##name option_##name##_map(Option_##name o, type (*f)(type)) {\
    return o.tag == Some ? option_##name##_some(f(o.payload.some)) : o;\
}

OPTION(char*, str);
OPTION(char, char);
OPTION(unsigned char, unsigned_char);
OPTION(short, short);
OPTION(unsigned short, unsigned_short);
OPTION(int, int);
OPTION(unsigned int, unsigned_int);
OPTION(long, long);
OPTION(unsigned long, unsigned_long);
OPTION(long long, long_long);
OPTION(unsigned long long, unsigned_long_long);
OPTION(float, float);
OPTION(double, double);
OPTION(long double, long_double);

#endif //_OPTION_H