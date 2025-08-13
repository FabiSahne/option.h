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
    type payload;\
} Option_##name;\
\
static inline Option_##name option_##name##_some(type val) {\
    Option_##name o = {.tag = Some};\
    o.payload = val;\
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
static inline int option_##name##_is_some_and(const Option_##name* o, int (*f)(const type*)) {\
    return o->tag == Some && f((const type*)&o->payload);\
}\
\
static inline int option_##name##_is_none_or(const Option_##name* o, int (*f)(const type*)) {\
    return o->tag == None || f((const type*)&o->payload);\
}\
\
static inline type option_##name##_unwrap(Option_##name o) {\
    if (o.tag == None) {\
        fprintf(stderr, "option_" #name "_unwrap: called on a None value\n");\
        abort();\
    }\
    return o.payload;\
}\
\
static inline type option_##name##_unwrap_or(Option_##name o, type fallback) {\
    return o.tag == Some ? o.payload : fallback;\
}\
\
static inline type option_##name##_unwrap_or_else(Option_##name o, type (*f)()) {\
    return o.tag == Some ? o.payload : f();\
}\
\
static inline type option_##name##_unwrap_or_zeroed(Option_##name o) {\
    return o.tag == Some ? o.payload : (type) { 0 };\
}\
\
static inline type option_##name##_expect(Option_##name o, char* msg) {\
    if (o.tag == None) {\
        fprintf(stderr, "option_" #name "_unwrap: called on a None value: %s\n", msg);\
        abort();\
    }\
    return o.payload;\
}\
\
static inline type* option_##name##_as_ptr(Option_##name* o) {\
    return o->tag == Some ? &o->payload : NULL;\
}\
\
static inline Option_##name option_##name##_and(Option_##name o, Option_##name other) {\
    return o.tag == Some ? other : o;\
}\
\
static inline Option_##name option_##name##_and_then(Option_##name o, Option_##name (*f)(type)) {\
    return o.tag == Some ? f(o.payload) : o;\
}\
\
static inline Option_##name option_##name##_or(Option_##name o, Option_##name other) {\
    return o.tag == Some ? o : other;\
}\
\
static inline Option_##name option_##name##_or_else(Option_##name o, Option_##name (*f)()) {\
    return o.tag == Some ? o : f();\
}\
\
static inline Option_##name option_##name##_map(Option_##name o, type (*f)(type)) {\
    return o.tag == Some ? option_##name##_some(f(o.payload)) : o;\
}\
\
static inline Option_##name option_##name##_filter(Option_##name o, int (*f)(const type*)) {\
    if (o.tag == None || !f((const type*)&o.payload)) {\
        return option_##name##_none();\
    } else {\
        return o;\
    }\
}\
\
static inline type* option_##name##_insert(Option_##name* o, type value) {\
    o->tag = Some;\
    o->payload = value;\
    return &o->payload;\
}\
\
static inline type* option_##name##_get_or_insert(Option_##name* o, type value) {\
    if (o->tag == Some)\
        return &o->payload;\
    return option_##name##_insert(o, value);\
}\
\
static inline Option_##name option_##name##_take(Option_##name* o) {\
    Option_##name copy = *o;\
    o->tag = None;\
    return copy;\
}\
\
static inline Option_##name option_##name##_take_if(Option_##name* o, int (*f)(const type*)) {\
    return option_##name##_is_some_and(o, f) ? option_##name##_take(o) : option_##name##_none();\
}\
\
static inline Option_##name option_##name##_replace(Option_##name* o, type value) {\
    Option_##name copy = *o;\
    o->tag = Some;\
    o->payload = value;\
    return copy;\
}\
\
static inline int option_##name##_eq_with(const Option_##name* o, const Option_##name* other, int (*eq)(const type*, const type*)) {\
    return o->tag == Some && other->tag == Some ? eq((const type*)&o->payload, (const type*)&other->payload) : false;\
}\
\
static inline int option_##name##_cmp_with(const Option_##name* o, const Option_##name* other, int (*cmp)(const type*, const type*)) {\
    if (o->tag == Some) {\
        if (other->tag == Some) {\
            return cmp((const type*)&o->payload, (const type*)&other->payload);\
        } else {\
            return 1;\
        }\
    } else {\
        if (other->tag == Some) {\
            return -1;\
        } else {\
            return 0;\
        }\
    }\
}\

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

#define DEBUG
#ifdef DEBUG
typedef struct {
    int y;
    char* str;
} test_t;

OPTION(test_t, test);
#endif

#endif //_OPTION_H