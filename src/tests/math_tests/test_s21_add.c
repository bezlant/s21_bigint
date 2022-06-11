#include "../s21_decimal_test.h"

START_TEST(gcc_128_bits) {
    long long long_a = get_random_ll() * rand();
    long long long_b = get_random_ll() * rand();

    __int128_t a = long_a;
    __int128_t b = long_b;
    __int128_t sum = a + b;
    s21_decimal res128 = bigint_to_decimal(sum);

    s21_decimal dec_a = ll_to_decimal(long_a);
    s21_decimal dec_b = ll_to_decimal(long_b);
    s21_decimal dec_sum = {0};

    s21_add(dec_a, dec_b, &dec_sum);

    ck_assert_int_eq(s21_is_equal(res128, dec_sum), TRUE);
}
END_TEST

START_TEST(random_decimal_exp) {
    long long long_a = get_random_short();
    long long long_b = get_random_short();

    if (rand() % 2) {
        long_a *= -1;
    }

    if (rand() % 2) {
        long_b *= -1;
    }

    long long expected = long_a + long_b;

    s21_decimal a = ll_to_decimal(long_a);
    s21_decimal b = ll_to_decimal(long_b);

    s21_decimal got = {0};
    s21_decimal short_dec = {.bits[0] = expected};

    s21_decimal dec_sum = {0};

    s21_add(a, b, &dec_sum);

#ifdef DEBUG
    print_bits_r(dec_sum);
    print_bits_r(short_dec);
    printf("DEC: %d \t SHORT: %d \n", dec_sum.bits[0], short_dec.bits[0]);
    printf("DEC: %d \t SHORT: %d \n", dec_sum.bits[1], short_dec.bits[1]);
    printf("DEC: %d \t SHORT: %d \n", dec_sum.bits[2], short_dec.bits[2]);
    printf("DEC: %d \t SHORT: %d \n", dec_sum.bits[3], short_dec.bits[3]);
#endif

    ck_assert_int_eq(abs(dec_sum.bits[0]), abs(short_dec.bits[0]));
}
END_TEST

Suite *suite_s21_add(void) {
    Suite *s = suite_create(PRETTY_PRINT("s21_add"));
    TCase *tc = tcase_create("s21_add_tc");

    tcase_add_loop_test(tc, random_decimal_exp, 0, 5);
    tcase_add_loop_test(tc, gcc_128_bits, 0, 10000);
    tcase_add_loop_test(tc, random_decimal_exp, 0, 10000);

    suite_add_tcase(s, tc);
    return s;
}
