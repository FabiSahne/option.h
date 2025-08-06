# Option\<T\> for C

A lightweight, macro-generated Option\<T\> implementation for C that provides Rust-like optional value handling.

## Overview

This library provides a type-safe way to handle optional values in C, similar to Rust's `Option<T>` or Haskell's `Maybe`. It uses C macros to generate type-specific Option implementations, avoiding the need for void pointers and providing compile-time type safety.

## Features

- **Type-safe**: Each Option type is generated specifically for a given type
- **Zero-cost abstractions**: All functions are inlined static functions
- **Rust-inspired API**: Familiar methods like `unwrap`, `is_some`, `is_none`, `and_then`, `map`
- **Pre-generated common types**: Includes Option types for all basic C types
- **Memory efficient**: Uses tagged unions to minimize memory overhead

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

#### `option_<type>_some(value)`
Creates an Option containing the given value.

```c
Option_int opt = option_int_some(42);
```

#### `option_<type>_none()`
Creates an empty Option (None variant).

```c
Option_int opt = option_int_none();
```

### Checking Option State

#### `option_<type>_is_some(option*)`
Returns 1 if the Option contains a value, 0 otherwise.

```c
if (option_int_is_some(&opt)) {
    // Handle Some case
}
```

#### `option_<type>_is_none(option*)`
Returns 1 if the Option is empty, 0 otherwise.

```c
if (option_int_is_none(&opt)) {
    // Handle None case
}
```

### Extracting Values

#### `option_<type>_unwrap(option)`
Extracts the value from the Option. **Panics if called on None**.

```c
int value = option_int_unwrap(opt); // Aborts if opt is None
```

#### `option_<type>_as_ptr(option*)`
Returns a pointer to the contained value, or NULL if None.

```c
int* ptr = option_int_as_ptr(&opt);
if (ptr != NULL) {
    printf("Value: %d\n", *ptr);
}
```

### Functional Operations

#### `option_<type>_and_then(option, function)`
Applies a function that returns an Option to the contained value (if any).

```c
Option_int double_if_positive(int x) {
    return x > 0 ? option_int_some(x * 2) : option_int_none();
}

Option_int result = option_int_and_then(opt, double_if_positive);
```

#### `option_<type>_map(option, function)`
Applies a function to the contained value (if any) and wraps the result in an Option.

```c
int double_value(int x) {
    return x * 2;
}

Option_int result = option_int_map(opt, double_value);
```

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
OPTION(Point, Point);

// Usage
Point p = {10, 20};
Option_Point opt_point = option_Point_some(p);
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

## Memory Layout

The Option type uses a tagged union for efficient memory usage:

```c
typedef struct {
    Option_Tag tag;    // 4 bytes (typically)
    union {
        type some;     // Size of the contained type
    } payload;
} Option_<type>;
```

Total size = `sizeof(Option_Tag) + sizeof(type)` + potential padding.

## Error Handling

- `unwrap()` will print an error message to stderr and call `abort()` if called on a None value
- Use `is_some()` or `as_ptr()` for safe value checking
- Consider using `and_then()` and `map()` for functional error handling

## License

[GPL v3.0](LICENSE)

## Compatibility

- **C Standard**: C99 or later (uses inline functions and designated initializers)
- **Compilers**: GCC, Clang, MSVC
- **Platforms**: Cross-platform (no platform-specific code)
