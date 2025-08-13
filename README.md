# Option\<T\> for C

A lightweight, macro-generated Option\<T\> implementation for C that provides Rust-like optional value handling.

- [Option\<T\> for C](#optiont-for-c)
  - [Overview](#overview)
  - [Features](#features)
  - [Quick Start](#quick-start)
  - [API Reference](#api-reference)
    - [Creating Option Values](#creating-option-values)
      - [`Option_<type> option_<type>_some(value)`](#option_type-option_type_somevalue)
      - [`Option_<type> option_<type>_none()`](#option_type-option_type_none)
    - [Checking Option State](#checking-option-state)
      - [`int option_<type>_is_some(option*)`](#int-option_type_is_someoption)
      - [`int option_<type>_is_some_and(option*, int (*f)(<type>*))`](#int-option_type_is_some_andoption-int-ftype)
      - [`int option_<type>_is_none(option*)`](#int-option_type_is_noneoption)
      - [`int option_<type>_is_none_or(option*, int (*f)(<type>*))`](#int-option_type_is_none_oroption-int-ftype)
    - [Extracting Values](#extracting-values)
      - [`<type> option_<type>_unwrap(option)`](#type-option_type_unwrapoption)
      - [`<type> option_<type>_unwrap_or(option, <type> fallback)`](#type-option_type_unwrap_oroption-type-fallback)
      - [`<type> option_<type>_unwrap_or_else(option, <type> (*f)())`](#type-option_type_unwrap_or_elseoption-type-f)
      - [`<type> option_<type>_unwrap_or_zeroed(option)`](#type-option_type_unwrap_or_zeroedoption)
      - [`<type> option_<type>_unwrap_unchecked(option)`](#type-option_type_unwrap_uncheckedoption)
        - [Safety](#safety)
      - [`<type>* option_<type>_as_ptr(option*)`](#type-option_type_as_ptroption)
    - [Functional Operations](#functional-operations)
      - [`Option_<type> option_<type>_and(option, option)`](#option_type-option_type_andoption-option)
      - [`Option_<type> option_<type>_and_then(option, option (*f)(<type>))`](#option_type-option_type_and_thenoption-option-ftype)
      - [`Option_<type> option_<type>_or(option, option)`](#option_type-option_type_oroption-option)
      - [`Option_<type> option_<type>_or_else(option, option (*f)())`](#option_type-option_type_or_elseoption-option-f)
      - [`Option_<type> option_<type>_map(option, <type> (*f)(<type>))`](#option_type-option_type_mapoption-type-ftype)
      - [`Option_<type> option_<type>_filter(option, int (*pred)(const <type>*))`](#option_type-option_type_filteroption-int-predconst-type)
      - [`<type>* option_<type>_insert(option*, <type> value)`](#type-option_type_insertoption-type-value)
      - [`<type>* option_<type>_get_or_insert(option*, <type> value)`](#type-option_type_get_or_insertoption-type-value)
      - [`Option_<type> option_<type>_take(option*)`](#option_type-option_type_takeoption)
      - [`Option_<type> option_<type>_take_if(option*, int (*pred)(const <type>*))`](#option_type-option_type_take_ifoption-int-predconst-type)
      - [`Option_<type> option_<type>_replace(option*, <type> value)`](#option_type-option_type_replaceoption-type-value)
    - [Comparisons](#comparisons)
      - [`int option_<type>_eq_with(const option*, const option*, int (*eq)(const <type>*, const <type>*))`](#int-option_type_eq_withconst-option-const-option-int-eqconst-type-const-type)
      - [`int option_<type>_cmp_with(const option*, const option*, int (*cmp)(const <type>*, const <type>*))`](#int-option_type_cmp_withconst-option-const-option-int-cmpconst-type-const-type)
  - [Pre-defined Types](#pre-defined-types)
  - [Creating Custom Option Types](#creating-custom-option-types)
    - [Renaming Existing Types](#renaming-existing-types)
  - [Examples](#examples)
    - [Basic Usage](#basic-usage)
    - [Safe Division Function](#safe-division-function)
    - [Using Functional Operations](#using-functional-operations)
  - ["Panicking"](#panicking)
  - [License](#license)
  - [Compatibility](#compatibility)


## Overview

This library provides a type-safe way to handle optional values in C, similar to Rust's `Option<T>` or Haskell's `Maybe`. It uses C macros to generate type-specific Option implementations, avoiding the need for void pointers and providing compile-time type safety.

## Features

- **Type-safe**: Each Option type is generated specifically for a given type
- **Rust-inspired API**: Familiar methods like `unwrap`, `is_some`, `is_none`, `and_then`, `map`
- **Pre-generated common types**: Includes Option types for all basic C types

## Quick Start

```c
#include "option.h"

int main() {
    // Create Some and None values
    Option_int some_value = option_int_some(42);
    Option_int none_value = option_int_none();

    // Check if value exists
    if (option_int_is_some(&some_value)) {
        int value = option_int_unwrap(some_value);
        printf("Value: %d\n", value);
    }

    return 0;
}
```

## API Reference

### Creating Option Values

#### `Option_<type> option_<type>_some(value)`
Creates an Option containing the given value.

```c
Option_int opt = option_int_some(42);
```

#### `Option_<type> option_<type>_none()`
Creates an empty Option (None variant).

```c
Option_int opt = option_int_none();
```

### Checking Option State

#### `int option_<type>_is_some(option*)`
Returns 1 if the Option contains a `Some` value.

```c
Option_int x = option_int_some(2);
assert(option_int_is_some(&x));

x = option_int_none();
assert(!option_int_is_some(&x));
```

#### `int option_<type>_is_some_and(option*, int (*f)(<type>*))`
Returns 1 if the Option contains a `Some` value and the value inside of it matches a predicate.

```c
static inline int greater_than_one(int* x) {
    return *x > 1;
}

Option_int x = option_int_some(2);
assert(option_int_is_some_and(&x, greater_than_one));

x = option_int_some(0);
assert(!option_int_is_some_and(&x, greater_than_one));

x = option_int_none();
assert(!option_int_is_some_and(&x, greater_than_one));
```

#### `int option_<type>_is_none(option*)`
Returns 1 if the Option is a `None` value.

```c
Option_int x = option_int_some(2);
assert(!option_int_is_none(&x));

x = option_int_none();
assert(option_int_is_none(&x));
```

#### `int option_<type>_is_none_or(option*, int (*f)(<type>*))`
Returns 1 if the Option is a `None` value, or the value inside of it matches a predicate.

```c
static inline int greater_than_one(int* x) {
    return *x > 1;
}

Option_int x = option_int_some(2);
assert(option_int_is_none_or(&x, greater_than_one));

x = option_int_some(0);
assert(!option_int_is_none_or(&x, greater_than_one));

x = option_int_none();
assert(option_int_is_none_or(&x, greater_than_one));
```

### Extracting Values

#### `<type> option_<type>_unwrap(option)`
Returns the contained `Some` value.

Because this function may abort the program, its use is generally discouraged.

Instead, prefer to handle the `None` case explicitly, or call `option_<type>_unwrap_or`, `option_<type>_unwrap_or_else`, or `option_<type>_unwrap_or_zeroed`

```c
Option_str x = option_str_some("air");
char* unwrapped = option_str_unwrap(x);
assert(strcmp(unwrapped, "air") == 0);
```

#### `<type> option_<type>_unwrap_or(option, <type> fallback)`
Returns the contained `Some` value or a provided fallback.

Arguments passed to `unwrap_or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.

```c
Option_str car = option_str_some("car");
char* unwrapped = option_str_unwrap_or(car, "bike");
assert(strcmp(unwrapped, "car") == 0);

car = option_str_none();
unwrapped = option_str_unwrap_or(car, "bike");
assert(strcmp(unwrapped, "bike") == 0);
```

#### `<type> option_<type>_unwrap_or_else(option, <type> (*f)())`
Returns the contained `Some` value or computes it from a function.

```c
static inline int ten_times_two() { return 10 * 2; }

Option_int x = option_int_some(4);
assert(option_int_unwrap_or_else(x, ten_times_two) == 4);

x = option_int_none();
assert(option_int_unwrap_or_else(x, ten_times_two) == 20);
```

#### `<type> option_<type>_unwrap_or_zeroed(option)`
Returns the contained `Some` value or zero.

If `Some`, returns the contained value, otherwise if `None`, returns the value type zeroed.

```c
Option_int x = option_int_none();
Option_int y = option_int_some(12);
assert(option_int_unwrap_or_zeroed(x) == 0);
assert(option_int_unwrap_or_zeroed(y) == 12);
```

#### `<type> option_<type>_unwrap_unchecked(option)`
Returns the contained `Some` value, without checking that the value is not `None`

##### Safety
Calling this on `None` is _undefined behavior_.

```c
Option_str x = option_str_some("air");
char* unwrapped = option_str_unwrap_unchecked(x); // "air"
x = option_str_none();
unwrapped = option_str_unwrap_unchecked(x); // UNDEFINED BEHAVIOR
// assert(strcmp(unwrapped, "air") == 0); // MAYBE TRUE, MAYBE FALSE, MAY CRASH
```


#### `<type>* option_<type>_as_ptr(option*)`
Returns a pointer to the contained value, or NULL if None.

```c
Option_int x = option_int_some(2);
int* inner_x = option_int_as_ptr(&x);
*inner_x = 5;
assert(option_int_unwrap(x) == 5);

x = option_int_none();
inner_x = option_int_as_ptr(&x);
assert(inner_x == NULL);
```

### Functional Operations

#### `Option_<type> option_<type>_and(option, option)`
Returns `None` if the first option is `None`, otherwise returns the second option.

Arguments passed to `and` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `and_then`, which is lazily evaluated.

```c
Option_int x = option_int_some(2);
Option_int y = option_int_none();
Option_int x_and_y = option_int_and(x, y);
assert(option_int_is_none(&x_and_y));

Option_double a = option_double_none();
Option_double b = option_double_some(1.3);
Option_double a_and_b = option_double_and(a, b);
assert(option_double_is_none(&a_and_b));

Option_char one = option_char_some('a');
Option_char two = option_char_some('b');
Option_char one_and_two = option_char_and(one, two);
assert(option_char_unwrap(one_and_two) == 'b');
```

#### `Option_<type> option_<type>_and_then(option, option (*f)(<type>))`
Applies a function that returns an Option to the contained value (if any).

```c
static inline Option_int checked_square(int x) {
    if (x > 46340) // sqrt(INT_MAX)
    {
        return option_int_none();
    } else {
        return option_int_some(x * x);
    }
}

Option_int x = option_int_some(2);
Option_int squared = option_int_and_then(x, checked_square);
assert(option_int_unwrap(squared) == 4);

x = option_int_some(1000000);
squared = option_int_and_then(x, checked_square);
assert(option_int_is_none(&squared)); // Overflowed

x = option_int_none();
squared = option_int_and_then(x, checked_square);
assert(option_int_is_none(&squared));
```

#### `Option_<type> option_<type>_or(option, option)`
Returns the first option if it contains a value, otherwise returns the second option.

Arguments passed to `or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `or_else`, which is lazily evaluated.

```c
Option_int x = option_int_some(2);
Option_int y = option_int_none();
Option_int x_or_y = option_int_or(x, y);
assert(option_int_unwrap(x_or_y) == 2);

Option_double a = option_double_none();
Option_double b = option_double_some(1.3);
Option_double a_or_b = option_double_or(a, b);
assert(option_double_unwrap(a_or_b) == 1.3);

Option_char one = option_char_some('a');
Option_char two = option_char_some('b');
Option_char one_or_two = option_char_or(one, two);
assert(option_char_unwrap(one_or_two) == 'a');

x = option_int_none();
x_or_y = option_int_or(x, y);
assert(option_int_is_none(&x_or_y));
```

#### `Option_<type> option_<type>_or_else(option, option (*f)())`
Returns the option if it contains a value, otherwise calls `f` and returns the result.

```c
static inline Option_str nobody() { return option_str_none(); }
static inline Option_str vikings() {return option_str_some("vikings"); }

Option_str barbs = option_str_some("barbarians");
Option_str barbs_or_vikings = option_str_or_else(barbs, vikings);
assert(strcmp(option_str_unwrap(barbs_or_vikings), "barbarians") == 0);

barbs = option_str_none();
barbs_or_vikings = option_str_or_else(barbs, vikings);
assert(strcmp(option_str_unwrap(barbs_or_vikings), "vikings") == 0);

barbs = option_str_none();
barbs_or_vikings = option_str_or_else(barbs, nobody);
assert(option_str_is_none(&barbs_or_vikings));
```

#### `Option_<type> option_<type>_map(option, <type> (*f)(<type>))`
Applies a function to the contained value (if any) and wraps the result in an Option.

```c
static inline char* halve(char* str) {
    int len = strlen(str) / 2;
    return str + len;
}

Option_str maybe_some_string = option_str_some("Hello, World!");
Option_str maybe_some_other_str = option_str_map(maybe_some_string, halve);
char* unwrapped = option_str_unwrap(maybe_some_other_str);

assert(strcmp(unwrapped, " World!") == 0);
```

#### `Option_<type> option_<type>_filter(option, int (*pred)(const <type>*))`
Returns `None` if the option is `None`, otherwise calls the predicate with the wrapped value and returns:
- `Some(t)` if the predicate returns `true` (where `t` is the wrapped value)
- `None` if the predicate return `false`

```c
static inline int is_even(const int* n) {
    return *n % 2 == 0;
}

Option_int x = option_int_none();
Option_int filtered = option_int_filter(x, is_even);
assert(option_int_is_none(&filtered));

x = option_int_some(3);
filtered = option_int_filter(x, is_even);
assert(option_int_is_none(&filtered));

x = option_int_some(4);
filtered = option_int_filter(x, is_even);
assert(option_int_is_some(&filtered));
```

#### `<type>* option_<type>_insert(option*, <type> value)`
Inserts `value` into the option, then returns a reference to it.

If the option already contains a value, the old value is forgotten.

See also `option_<type>_get_or_insert`, which doesn't update the value if the option already contains `Some`.

```c
Option_int opt = option_int_none();
int* val = option_int_insert(&opt, 1);
assert(*val == 1);
assert(option_int_unwrap(opt) == 1);

val = option_int_insert(&opt, 2);
assert(*val == 2);

*val = 3;
assert(option_int_unwrap(opt) == 3);
```

#### `<type>* option_<type>_get_or_insert(option*, <type> value)`
Inserts `value` into the option if it is `None`, then returns a reference to the contained value.

See also `option_<type>_insert`, which updates the value even if the option already contains `Some`.

```c
Option_int x = option_int_none();
{
    int* y = option_int_get_or_insert(&x, 5);
    assert(*y == 5);
    *y = 7;
}
assert(option_int_unwrap(x) == 7);
```

#### `Option_<type> option_<type>_take(option*)`
Takes the value out of option, leaving a `None` in its place.

```c
Option_int x = option_int_some(2);
Option_int y = option_int_take(&x);
assert(option_int_is_none(&x));
assert(option_int_unwrap(y) == 2);

x = option_int_none();
y = option_int_take(&x);
assert(option_int_is_none(&x));
assert(option_int_is_none(&y));
```

#### `Option_<type> option_<type>_take_if(option*, int (*pred)(const <type>*))`
Takes the value out of option, but only if the predicate evaluates to `true` on a reference to the value.

In other words, replaces the option with `None` if the predicates returns `true`. This operates similar to `option_<type>_take` but conditional.

```c
static inline int is_odd(int* v) { return *v % 2 == 1; }
static inline int add_one_if_even(int* v) { 
    if (*v % 2 == 0) *v += 1;
    return false;
}

Option_int x = option_int_some(2);
Option_int prev = option_int_take_if(&x, add_one_if_even);
assert(option_int_unwrap(x) == 3);
assert(option_int_is_none(&prev));

prev = option_int_take_if(&x, is_odd);
assert(option_int_is_none(&x));
assert(option_int_unwrap(prev) == 3);
```

#### `Option_<type> option_<type>_replace(option*, <type> value)`
Replaces the actual value in the option by the value given in parameter, returning the old value if present, leaving a Some in its place without deinitializing either one.

```c
Option_int x = option_int_some(2);
Option_int old = option_int_replace(&x, 5);
assert(option_int_unwrap(x) == 5);
assert(option_int_unwrap(old) == 2);

x = option_int_none();
old = option_int_replace(&x, 3);
assert(option_int_unwrap(x) == 3);
assert(option_int_is_none(&old));
```

### Comparisons
#### `int option_<type>_eq_with(const option*, const option*, int (*eq)(const <type>*, const <type>*))`
Checks for equality using the function `eq` if both options are `Some`, which takes references of the containing types.

```c
Option_str a = option_str_some("Hello!");
Option_str b = option_str_none();
assert(!option_str_eq_with(&a, &b, str_eq_fn));

a = option_str_some("Hello!");
b = option_str_some("Hello!");
assert(option_str_eq_with(&a, &b, str_eq_fn));
```

#### `int option_<type>_cmp_with(const option*, const option*, int (*cmp)(const <type>*, const <type>*))`
Compares the two options and returns an integer indicating the result of the comparison, as follows:
- 0, if the options are equal (both are `None` or `cmp` returned 0)
- a negative value if the first option is less than the second one ((Some(...), None), or `cmp` returned a negative value)
- a positive value if the first option is greater than the second one ((None, Some(...)), or `cmp` returned a positive value)

```c
Option_int a = option_int_some(3);
Option_int b = option_int_none();
assert(option_int_cmp_with(&a, &b, int_cmp_fn) > 0);

b = option_int_some(5);
assert(option_int_cmp_with(&a, &b, int_cmp_fn) < 0);

a = option_int_none();
b = option_int_none();
assert(option_int_cmp_with(&a, &b, int_cmp_fn) == 0);
```

## Pre-defined Types

The following Option types are pre-generated and ready to use:

| C Type               | Option Type                 | Constructor                                                             |
| -------------------- | --------------------------- | ----------------------------------------------------------------------- |
| `char*`              | `Option_str`                | `option_str_some()` / `option_str_none()`                               |
| `char`               | `Option_char`               | `option_char_some()` / `option_char_none()`                             |
| `unsigned char`      | `Option_unsigned_char`      | `option_unsigned_char_some()` / `option_unsigned_char_none()`           |
| `short`              | `Option_short`              | `option_short_some()` / `option_short_none()`                           |
| `unsigned short`     | `Option_unsigned_short`     | `option_unsigned_short_some()` / `option_unsigned_short_none()`         |
| `int`                | `Option_int`                | `option_int_some()` / `option_int_none()`                               |
| `unsigned int`       | `Option_unsigned_int`       | `option_unsigned_int_some()` / `option_unsigned_int_none()`             |
| `long`               | `Option_long`               | `option_long_some()` / `option_long_none()`                             |
| `unsigned long`      | `Option_unsigned_long`      | `option_unsigned_long_some()` / `option_unsigned_long_none()`           |
| `long long`          | `Option_long_long`          | `option_long_long_some()` / `option_long_long_none()`                   |
| `unsigned long long` | `Option_unsigned_long_long` | `option_unsigned_long_long_some()` / `option_unsigned_long_long_none()` |
| `float`              | `Option_float`              | `option_float_some()` / `option_float_none()`                           |
| `double`             | `Option_double`             | `option_double_some()` / `option_double_none()`                         |
| `long double`        | `Option_long_double`        | `option_long_double_some()` / `option_long_double_none()`               |

## Creating Custom Option Types

You can generate Option types for your custom types using the `OPTION` macro:

```c
// Define a custom struct
typedef struct {
    int x, y;
} Point;

// Generate Option_Point type
// OPTION(type, name)
OPTION(Point, point);

// Usage
Point p = {10, 20};
Option_point opt_point = option_point_some(p);
```

### Renaming Existing Types

You can also rename existing types.

For example, if you think `option_unsigned_long_long_get_or_insert(&some_opt)` is too long
```c
OPTION(unsigned long long, ull); // -> Option_ull
```
will change that to `option_ull_get_or_insert(&some_opt)`.


## Examples

### Basic Usage

```c
#include "option.h"
#include <stdio.h>

int main() {
    Option_int numbers[] = {
        option_int_some(1),
        option_int_some(2),
        option_int_none(),
        option_int_some(4)
    };
    
    for (int i = 0; i < 4; i++) {
        if (option_int_is_some(&numbers[i])) {
            printf("numbers[%d] = %d\n", i, option_int_unwrap(numbers[i]));
        } else {
            printf("numbers[%d] = None\n", i);
        }
    }
    
    return 0;
}
```

### Safe Division Function

```c
Option_double safe_divide(double a, double b) {
    if (b == 0.0) {
        return option_double_none();
    }
    return option_double_some(a / b);
}

int main() {
    Option_double result = safe_divide(10.0, 2.0);
    
    if (option_double_is_some(&result)) {
        printf("Result: %.2f\n", option_double_unwrap(result));
    } else {
        printf("Division by zero!\n");
    }
    
    return 0;
}
```

### Using Functional Operations

```c
Option_int increment_if_positive(int x) {
    return x > 0 ? option_int_some(x + 1) : option_int_none();
}

int double_value(int x) {
    return x * 2;
}

int main() {
    Option_int opt = option_int_some(5);
    
    // Chain operations
    Option_int result1 = option_int_and_then(opt, increment_if_positive); // Some(6)
    Option_int result2 = option_int_map(result1, double_value);           // Some(12)
    
    printf("Final result: %d\n", option_int_unwrap(result2)); // 12
    
    return 0;
}
```

## "Panicking"

Similar to Rust, calling `unwrap()` or `expect()` will abort the program. Consider using `unwrap_or()`, `unwrap_or_else()`, `unwrap_or_zeroed()`, or "functional" alternatives instead. 

## License

[GPL v3.0](LICENSE)

## Compatibility

- **C Standard**: C99 or later (uses inline functions and designated initializers)
- **Compilers**: GCC, Clang, (MSVC untested)
- **Platforms**: Cross-platform (no platform-specific code)
