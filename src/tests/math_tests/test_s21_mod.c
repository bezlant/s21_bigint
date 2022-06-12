#include "../s21_decimal_test.h"

START_TEST(gcc_128_bits) {
    long long long_a = get_random_ll() * rand();
    long long long_b = get_random_ll() * rand();
    // printf("long_a  =%lld\n", long_a);
    // printf("long_b  =%lld\n", long_b);
    __int128_t a = long_a;
    __int128_t b = long_b;
    __int128_t mod = a % b;
    s21_decimal res128 = bigint_to_decimal(mod);

    s21_decimal dec_a = ll_to_decimal(long_a);

    // print_bits_r(dec_a);
    s21_decimal dec_b = ll_to_decimal(long_b);
    s21_decimal dec_div = {0};

    // print_bits_r(dec_b);

    int code = s21_mod(dec_a, dec_b, &dec_div);

    // printf("dec_div=");
    // print_bits_r(dec_div);
    // printf("res128_=");
    // print_bits_r(res128);

    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(s21_is_equal(res128, dec_div), TRUE);
}

START_TEST(mod_by_one) {
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_decimal res = {0};

    a.bits[0] = get_rand(0, INT_MAX);
    a.bits[1] = get_rand(0, INT_MAX);
    a.bits[2] = get_rand(0, INT_MAX);

    /* This is same as bits[0] = 1 */
    set_bit_1(&b, 0);

    int code = s21_mod(a, b, &res);

    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(res.bits[0], 0);
    ck_assert_int_eq(res.bits[1], 0);
    ck_assert_int_eq(res.bits[2], 0);
}
END_TEST

START_TEST(even_or_odd_mod) {
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_decimal res = {0};

    a.bits[0] = get_rand(0, INT_MAX);
    b.bits[0] = a.bits[0] % 2;

    int code = s21_mod(a, b, &res);

    ck_assert_int_eq(code, ARITHMETIC_OK);

    if (a.bits[0] % 2 == 0) {
        ck_assert_int_eq(res.bits[0], 0);
    } else {
        ck_assert_int_eq(res.bits[0], 1);
    }
}
END_TEST

START_TEST(mod_by_rand_int) {
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_decimal res = {0};

    /* Loop ensures we do not get 0 as a result of integer mod */
    while (a.bits[0] < b.bits[0] || b.bits[0] == 0) {
        a.bits[0] = get_rand(0, INT_MAX);
        b.bits[0] = get_rand(0, INT_MAX);
    }

    int code = s21_mod(a, b, &res);
    int64_t expected = a.bits[0] % b.bits[0];

    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(res.bits[0], expected);
}
END_TEST

START_TEST(mod_by_zero) {
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_decimal res = {0};

    set_exponent(&a, get_rand(0, 27));
    set_exponent(&b, get_rand(0, 27));
    set_random_sign(&a);
    set_random_sign(&b);

    int code = s21_mod(a, b, &res);
    ck_assert_int_eq(code, S21_NAN);
}
END_TEST

START_TEST(mod_b_greater_than_a) {
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_decimal res = {0};

    /* Loop ensures we do not get 0 as a result of integer mod */
    while (a.bits[0] >= b.bits[0] || b.bits[0] == 0) {
        a.bits[0] = get_rand(0, INT_MAX);
        b.bits[0] = get_rand(0, INT_MAX);
    }

    int code = s21_mod(a, b, &res);
    /* Expected to return unmodified argument */
    int64_t expected = a.bits[0];

    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(res.bits[0], expected);
}
END_TEST

START_TEST(equal_decimals) {
    s21_decimal a = get_random_decimal(3, get_rand(0, 28));
    set_random_sign(&a);
    s21_decimal b = a;
    s21_decimal res = {0};

    int code = s21_mod(a, b, &res);

    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(res.bits[0], 0);
}
END_TEST

Suite *suite_s21_mod(void) {
    Suite *s = suite_create(PRETTY_PRINT("s21_mod"));
    TCase *tc = tcase_create("s21_mod_tc");

    tcase_add_loop_test(tc, mod_by_one, 0, 10);
    tcase_add_loop_test(tc, mod_by_zero, 0, 10);
    tcase_add_loop_test(tc, equal_decimals, 0, 10);
    tcase_add_loop_test(tc, mod_b_greater_than_a, 0, 10);

    tcase_add_loop_test(tc, even_or_odd_mod, 0, 10);
    // tcase_add_loop_test(tc, gcc_128_bits, 0, 10);
    // tcase_add_loop_test(tc, mod_by_rand_int, 0, 10);

    suite_add_tcase(s, tc);
    return s;
}
