#include "../option.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define RESET "\e[0m"


#define TEST(fn)\
printf("Testing `" #fn "`... ");\
if (fn) {\
    printf(GREEN "OK" RESET "\n");\
} else {\
    printf(RED "FAIL" RESET "\n");\
}

#define ASSERT(expr) if (!(expr)) return false

bool is_some() {
    Option_int x = option_int_some(2);
    ASSERT(option_int_is_some(&x));
    
    x = option_int_none();
    ASSERT(!option_int_is_some(&x));
    
    return true;
}

int greater_than_one(int* x) {
    return *x > 1;
}

bool is_some_and() {
    Option_int x = option_int_some(2);
    ASSERT(option_int_is_some_and(&x, greater_than_one));
    
    x = option_int_some(0);
    ASSERT(!option_int_is_some_and(&x, greater_than_one));
    
    x = option_int_none();
    ASSERT(!option_int_is_some_and(&x, greater_than_one));
    
    return true;
}

bool is_none() {
    Option_int x = option_int_some(2);
    ASSERT(!option_int_is_none(&x));
    
    x = option_int_none();
    ASSERT(option_int_is_none(&x));
    
    return true;
}

bool is_none_or() {
    Option_int x = option_int_some(2);
    ASSERT(option_int_is_none_or(&x, greater_than_one));
    
    x = option_int_some(0);
    ASSERT(!option_int_is_none_or(&x, greater_than_one));
    
    x = option_int_none();
    ASSERT(option_int_is_none_or(&x, greater_than_one));
    
    return true;
}

bool as_ptr() {
    Option_int x = option_int_some(2);
    int* inner_x = option_int_as_ptr(&x);
    *inner_x = 5;
    ASSERT(option_int_unwrap(x) == 5);
    
    x = option_int_none();
    inner_x = option_int_as_ptr(&x);
    ASSERT(inner_x == NULL);
    
    return true;
}

bool expect() {
    Option_str x = option_str_some("value");
    char* expected = option_str_expect(x, "fruits are healthy");
    ASSERT(strcmp(expected, "value") == 0);

    return true;
}

bool unwrap() {
    Option_str x = option_str_some("air");
    char* unwrapped = option_str_unwrap(x);
    ASSERT(strcmp(unwrapped, "air") == 0);

    return true;
}

bool unwrap_or() {
    Option_str car = option_str_some("car");
    char* unwrapped = option_str_unwrap_or(car, "bike");
    ASSERT(strcmp(unwrapped, "car") == 0);

    car = option_str_none();
    unwrapped = option_str_unwrap_or(car, "bike");
    ASSERT(strcmp(unwrapped, "bike") == 0);

    return true;
}

int ten_times_two() { return 10 * 2; }

bool unwrap_or_else() {
    Option_int x = option_int_some(4);
    ASSERT(option_int_unwrap_or_else(x, ten_times_two) == 4);

    x = option_int_none();
    ASSERT(option_int_unwrap_or_else(x, ten_times_two) == 20);

    return true;
}

bool unwrap_unchecked() {
    Option_str x = option_str_some("air");
    char* unwrapped = option_str_unwrap_unchecked(x);
    ASSERT(strcmp(unwrapped, "air") == 0);

    x = option_str_none();
    unwrapped = option_str_unwrap_unchecked(x); // UNDEFINED BEHAVIOR
    // ASSERT(strcmp(unwrapped, "air") == 0); // MAYBE TRUE, MAYBE FALSE, MAY CRASH

    return true;
}

bool unwrap_or_zeroed() {
    Option_int x = option_int_none();
    Option_int y = option_int_some(12);
    ASSERT(option_int_unwrap_or_zeroed(x) == 0);
    ASSERT(option_int_unwrap_or_zeroed(y) == 12);

    return true;
}

char* halve(char* str) {
    int len = strlen(str) / 2;
    return str + len;
}

bool map() {
    Option_str maybe_some_string = option_str_some("Hello, World!");
    Option_str maybe_some_other_str = option_str_map(maybe_some_string, halve);
    char* unwrapped = option_str_unwrap(maybe_some_other_str);

    ASSERT(strcmp(unwrapped, " World!") == 0);

    return true;
}

bool and() {
    Option_int x = option_int_some(2);
    Option_int y = option_int_none();
    Option_int x_and_y = option_int_and(x, y);
    ASSERT(option_int_is_none(&x_and_y));

    Option_double a = option_double_none();
    Option_double b = option_double_some(1.3);
    Option_double a_and_b = option_double_and(a, b);
    ASSERT(option_double_is_none(&a_and_b));

    Option_char one = option_char_some('a');
    Option_char two = option_char_some('b');
    Option_char one_and_two = option_char_and(one, two);
    ASSERT(option_char_unwrap(one_and_two) == 'b');

    return true;
}

Option_int checked_square(int x) {
    if (x > 46340) // sqrt(INT_MAX)
    {
        return option_int_none();
    } else {
        return option_int_some(x * x);
    }
}

bool and_then() {
    Option_int x = option_int_some(2);
    Option_int squared = option_int_and_then(x, checked_square);
    ASSERT(option_int_unwrap(squared) == 4);
    
    x = option_int_some(1000000);
    squared = option_int_and_then(x, checked_square);
    ASSERT(option_int_is_none(&squared)); // Overflowed

    x = option_int_none();
    squared = option_int_and_then(x, checked_square);
    ASSERT(option_int_is_none(&squared));

    return true;
}

bool or() {
    Option_int x = option_int_some(2);
    Option_int y = option_int_none();
    Option_int x_or_y = option_int_or(x, y);
    ASSERT(option_int_unwrap(x_or_y) == 2);

    Option_double a = option_double_none();
    Option_double b = option_double_some(1.3);
    Option_double a_or_b = option_double_or(a, b);
    ASSERT(option_double_unwrap(a_or_b) == 1.3);

    Option_char one = option_char_some('a');
    Option_char two = option_char_some('b');
    Option_char one_or_two = option_char_or(one, two);
    ASSERT(option_char_unwrap(one_or_two) == 'a');

    x = option_int_none();
    x_or_y = option_int_or(x, y);
    ASSERT(option_int_is_none(&x_or_y));

    return true;
}


Option_str nobody() { return option_str_none(); }
Option_str vikings() {return option_str_some("vikings"); }

int str_eq_fn(const char** a, const char** b) {
    return strcmp(*a, *b) == 0;
}

bool or_else() {
    Option_str barbs = option_str_some("barbarians");
    Option_str barbs_or_vikings = option_str_or_else(barbs, vikings);
    ASSERT(strcmp(option_str_unwrap(barbs_or_vikings), "barbarians") == 0);
    
    barbs = option_str_none();
    barbs_or_vikings = option_str_or_else(barbs, vikings);
    ASSERT(strcmp(option_str_unwrap(barbs_or_vikings), "vikings") == 0);

    barbs = option_str_none();
    barbs_or_vikings = option_str_or_else(barbs, nobody);
    ASSERT(option_str_is_none(&barbs_or_vikings));

    return true;
}

int is_even(const int* n) {
    return *n % 2 == 0;
}

bool filter() {
    Option_int x = option_int_none();
    Option_int filtered = option_int_filter(x, is_even);
    ASSERT(option_int_is_none(&filtered));

    x = option_int_some(3);
    filtered = option_int_filter(x, is_even);
    ASSERT(option_int_is_none(&filtered));

    x = option_int_some(4);
    filtered = option_int_filter(x, is_even);
    ASSERT(option_int_is_some(&filtered));

    return true;
}

bool insert() {
    Option_int opt = option_int_none();
    int* val = option_int_insert(&opt, 1);
    ASSERT(*val == 1);
    ASSERT(option_int_unwrap(opt) == 1);

    val = option_int_insert(&opt, 2);
    ASSERT(*val == 2);
    
    *val = 3;
    ASSERT(option_int_unwrap(opt) == 3);

    return true;
}

bool get_or_insert() {
    Option_int x = option_int_none();
    {
        int* y = option_int_get_or_insert(&x, 5);
        ASSERT(*y == 5);
        *y = 7;
    }
    ASSERT(option_int_unwrap(x) == 7);

    return true;
}

bool take() {
    Option_int x = option_int_some(2);
    Option_int y = option_int_take(&x);
    ASSERT(option_int_is_none(&x));
    ASSERT(option_int_unwrap(y) == 2);

    x = option_int_none();
    y = option_int_take(&x);
    ASSERT(option_int_is_none(&x));
    ASSERT(option_int_is_none(&y));

    return true;
}

int add_one_if_even(int* v) { 
    if (*v % 2 == 0) *v += 1;
    return false;
}

int is_odd(int* v) { return *v % 2 == 1; }

bool take_if() {
    Option_int x = option_int_some(2);
    Option_int prev = option_int_take_if(&x, add_one_if_even);
    ASSERT(option_int_unwrap(x) == 3);
    ASSERT(option_int_is_none(&prev));

    prev = option_int_take_if(&x, is_odd);
    ASSERT(option_int_is_none(&x));
    ASSERT(option_int_unwrap(prev) == 3);

    return true;
}

bool replace() {
    Option_int x = option_int_some(2);
    Option_int old = option_int_replace(&x, 5);
    ASSERT(option_int_unwrap(x) == 5);
    ASSERT(option_int_unwrap(old) == 2);

    x = option_int_none();
    old = option_int_replace(&x, 3);
    ASSERT(option_int_unwrap(x) == 3);
    ASSERT(option_int_is_none(&old));

    return true;
}



bool eq_with() {
    Option_str a = option_str_some("Hello!");
    Option_str b = option_str_none();
    ASSERT(!option_str_eq_with(&a, &b, str_eq_fn));

    a = option_str_some("Hello!");
    b = option_str_some("Hello!");
    ASSERT(option_str_eq_with(&a, &b, str_eq_fn));

    return true;
}

int int_cmp(const int* a, const int* b) {
    return *a - *b;
}

bool cmp_with() {
    Option_int a = option_int_some(3);
    Option_int b = option_int_none();
    ASSERT(option_int_cmp_with(&a, &b, int_cmp) > 0);

    b = option_int_some(5);
    ASSERT(option_int_cmp_with(&a, &b, int_cmp) < 0);

    a = option_int_none();
    b = option_int_none();
    ASSERT(option_int_cmp_with(&a, &b, int_cmp) == 0);

    return true;
}

int main() {
    TEST(is_some());
    TEST(is_some_and());
    TEST(is_none());
    TEST(is_none_or());

    TEST(as_ptr());

    TEST(expect());
    TEST(unwrap());
    TEST(unwrap_or());
    TEST(unwrap_or_else());
    TEST(unwrap_or_zeroed());
    TEST(unwrap_unchecked());

    TEST(map());
    TEST(and());
    TEST(and_then());
    TEST(or());
    TEST(or_else());
    TEST(filter());

    TEST(insert());
    TEST(get_or_insert());
    TEST(take());
    TEST(take_if());
    TEST(replace());

    TEST(eq_with());
}
