# Option\<T\> for C

A lightweight, macro-generated Option\<T\> implementation for C that provides Rust-like optional value handling.

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
if (option_int_is_some(&opt)) {
    // Handle Some case
}
```

#### `int option_<type>_is_some_and(option*, int (*f)(const <type>*))`
Returns 1 if the Option contains a `Some` value and the value inside of it matches a predicate.

```c
if (option_int_is_some_and(&opt, pred)) {
    // Handle Some case if pred(&opt->payload) is true
}
```

#### `int option_<type>_is_none(option*)`
Returns 1 if the Option is a `None` value.

```c
if (option_int_is_none(&opt)) {
    // Handle None case
}
```

#### `int option_<type>_is_none_or(option*, int (*f)(const <type>*))`
Returns 1 if the Option is a `None` value, or the value inside of it matches a predicate.

```c
if (option_int_is_none_or(&opt, pred)) {
    // Handle None case or if pred(&opt->payload) is true
}
```

### Extracting Values

#### `<type> option_<type>_unwrap(option)`
Returns the contained `Some` value.

Because this function may abort the program, its use is generally discouraged.

Instead, prefer to handle the `None` case explicitly, or call `option_<type>_unwrap_or`, `option_<type>_unwrap_or_else`, or `option_<type>_unwrap_or_zeroed`

```c
int value = option_int_unwrap(opt); // Aborts if opt is None
```

#### `<type> option_<type>_unwrap_or(option, <type> fallback)`
Returns the contained `Some` value or a provided fallback.

Arguments passed to `unwrap_or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `unwrap_or_else`, which is lazily evaluated.

```c
int value = option_int_unwrap_or(opt, 2); // 2 if opt is None, else opt.payload
```

#### `<type> option_<type>_unwrap_or_else(option, <type> (*f)())`
Returns the contained `Some` value or computes it from a function.

```c
int compute() { /*...*/ }
int value = option_int_unwrap_or_else(opt, compute); // result of `compute` if opt is None, else opt.payload
```

#### `<type> option_<type>_unwrap_or_zeroed(option)`
Returns the contained `Some` value or zero.

If `Some`, returns the contained value, otherwise if `None`, returns the value type zeroed.

```c
int value = option_int_unwrap_or_zeroed(opt); // 0 if opt is None, else opt.payload
```

#### `<type>* option_<type>_as_ptr(option*)`
Returns a pointer to the contained value, or NULL if None.

```c
int* ptr = option_int_as_ptr(&opt);
if (ptr != NULL) {
    printf("Value: %d\n", *ptr);
}
```

### Functional Operations

#### `Option_<type> option_<type>_and(option, option)`
Returns `None` if the first option is `None`, otherwise returns the second option.

Arguments passed to `and` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `and_then`, which is lazily evaluated.

```c
Option_int x = option_int_some(2);
Option_int y = option_int_none();
assert(option_int_eq(option_int_and(x, y), option_int_none()));

Option_double x = option_double_none();
Option_double y = option_double_some(1.3);
assert(option_double_eq(option_double_and(x, y), option_double_none()));

Option_char x = option_char_some('a');
Option_char y = option_char_some('b');
assert(option_char_eq(option_char_and(x, y), option_char_some('b')));
```

#### `Option_<type> option_<type>_and_then(option, option (*f)(<type>))`
Applies a function that returns an Option to the contained value (if any).

```c
Option_int double_if_positive(int x) {
    return x > 0 ? option_int_some(x * 2) : option_int_none();
}

Option_int result = option_int_and_then(opt, double_if_positive);
```

#### `Option_<type> option_<type>_or(option, option)`
Returns the first option if it contains a value, otherwise returns the second option.

Arguments passed to `or` are eagerly evaluated; if you are passing the result of a function call, it is recommended to use `or_else`, which is lazily evaluated.

```c
Option_int x = option_int_some(2);
Option_int y = option_int_none();
assert(option_int_eq_with(option_int_or(x, y), option_int_some(2), int_eq_fn));

Option_double x = option_double_none();
Option_double y = option_double_some(1.3);
assert(option_double_eq_with(option_double_and(x, y), option_double_some(1.3), double_eq_fn));

Option_char x = option_char_some('a');
Option_char y = option_char_some('b');
assert(option_char_eq_with(option_char_and(x, y), option_char_some('a'), char_eq_fn));

Option_int x = option_int_none();
Option_int y = option_int_none();
assert(option_int_eq_with(option_int_or(x, y), option_int_none(), int_eq_fn));
```

#### `Option_<type> option_<type>_or_else(option, option (*f)())`
Returns the option if it contains a value, otherwise calls `f` and returns the result.

```c
Option_str nobody() { return option_str_none(); }
Option_str vikings() {return option_str_some("vikings"); }

assert(option_str_eq_with(option_str_or_else(option_str_some("barbarians"), vikings), str_eq_fn), option_str_some("barbarians"));
assert(option_str_eq_with(option_str_or_else(option_str_none(), vikings), str_eq_fn), option_str_some("vikings"));
assert(option_str_eq_with(option_str_or_else(option_str_none(), nobody), str_eq_fn), option_str_none());
```

#### `Option_<type> option_<type>_map(option, <type> (*f)(<type>))`
Applies a function to the contained value (if any) and wraps the result in an Option.

```c
int double_value(int x) {
    return x * 2;
}

Option_int result = option_int_map(opt, double_value);
```

#### `Option_<type> option_<type>_filter(option, int (*pred)(const <type>*))`
Returns `None` if the option is `None`, otherwise calls the predicate with the wrapped value and returns:
- `Some(t)` if the predicate returns `true` (where `t` is the wrapped value)
- `None` if the predicate return `false`

```c
int is_even(const int* n) {
    return *n % 2 == 0;
}

assert(option_int_eq_with(&option_int_filter(option_int_none(), is_even), &option_int_none(), int_eq_fn));
assert(option_int_eq_with(&option_int_filter(option_int_some(3), is_even), &option_int_none(), int_eq_fn));
assert(option_int_eq_with(&option_int_filter(option_int_some(2), is_even), &option_int_some(2), int_eq_fn));
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
assert(option_int_eq_with(&x, &option_int_none(), int_eq_fn));
assert(option_int_eq_with(&y, &option_int_some(2), int_eq_fn));
```

#### `Option_<type> option_<type>_take_if(option*, int (*pred)(const <type>*))`
Takes the value out of option, but only if the predicate evaluates to `true` on a reference to the value.

In other words, replaces the option with `None` if the predicates returns `true`. This operates similar to `option_<type>_take` but conditional.

```c
int add_one_if_even(const int* v) { 
    if (*v % 2 == 0) *v += 1;
    return false;
}
int is_odd(const int* v) { return *v % 2 == 1; }

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
prev = option_int_replace(&x, 3);
assert(option_int_unwrap(x) == 3);
assert(option_int_is_none(&prev));
```

### Comparisons
#### `int option_<type>_eq_with(const option*, const option*, int (*eq)(const <type>*, const <type>*))`
Checks for equality using the function `eq` if both options are `Some`, which takes references of the containing types.

#### `int option_<type>_cmp_with(const option*, const option*, int (*cmp)(const <type>*, const <type>*))`
Compares the two options and returns an integer indicating the result of the comparison, as follows:
- 0, if the options are equal (both are `None` or `cmp` returned 0)
- a negative value if the first option is less than the second one ((Some(...), None), or `cmp` returned a negative value)
- a positive value if the first option is greater than the second one ((None, Some(...)), or `cmp` returned a positive value)

## Pre-defined Types

The following Option types are pre-generated and ready to use:

| C Type | Option Type | Constructor | 
|--------|-------------|-------------|
| `char*` | `Option_str` | `option_str_some()` / `option_str_none()` |
| `char` | `Option_char` | `option_char_some()` / `option_char_none()` |
| `unsigned char` | `Option_unsigned_char` | `option_unsigned_char_some()` / `option_unsigned_char_none()` |
| `short` | `Option_short` | `option_short_some()` / `option_short_none()` |
| `unsigned short` | `Option_unsigned_short` | `option_unsigned_short_some()` / `option_unsigned_short_none()` |
| `int` | `Option_int` | `option_int_some()` / `option_int_none()` |
| `unsigned int` | `Option_unsigned_int` | `option_unsigned_int_some()` / `option_unsigned_int_none()` |
| `long` | `Option_long` | `option_long_some()` / `option_long_none()` |
| `unsigned long` | `Option_unsigned_long` | `option_unsigned_long_some()` / `option_unsigned_long_none()` |
| `long long` | `Option_long_long` | `option_long_long_some()` / `option_long_long_none()` |
| `unsigned long long` | `Option_unsigned_long_long` | `option_unsigned_long_long_some()` / `option_unsigned_long_long_none()` |
| `float` | `Option_float` | `option_float_some()` / `option_float_none()` |
| `double` | `Option_double` | `option_double_some()` / `option_double_none()` |
| `long double` | `Option_long_double` | `option_long_double_some()` / `option_long_double_none()` |

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

For example, if you think `Option_unsigned_long_long` is too long:
```c
OPTION(unsigned long long, ull); // -> becomes Option_ull
```


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
