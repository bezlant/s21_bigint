#include "s21_decimal_test.h"

START_TEST(no_exp_decimals_to_int) {
    int got = rand(), expected = rand();

    s21_decimal input = get_random_int_decimal();
    expected = reverse_bits_int(input.bits[0]);
    int code = s21_from_decimal_to_int(input, &got);
    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(got, expected);
}
END_TEST;

START_TEST(exp_decimals_to_int_hardcoded) {
    int got1 = 1, got2 = 2, got3 = 3, got4 = 4, got5 = 5, got6 = 6;

    s21_decimal a1 = {0};
    s21_decimal a2 = {0};
    s21_decimal a3 = {0};
    s21_decimal a4 = {0};
    s21_decimal a5 = {0};
    s21_decimal a6 = {0};

    a1.bits[0] = 0b11000101011011011101111111111000; /* Dec: 3312312.312 */
    set_exponent(&a1, 3);
    ck_assert_int_eq(got1, 3312312);

    a2.bits[0] = 0b00000000000000000000000000000000; /* Dec: 555 */
    a3.bits[0] = 0b00000000000000000000000000000000; /* Dec: 555 */
    a4.bits[0] = 0b00000000000000000000000000000000; /* Dec: 555 */
    a5.bits[0] = 0b00000000000000000000000000000000; /* Dec: 555 */
    a6.bits[0] = 0b00000000000000000000000000000000; /* Dec: 555 */

    
    set_exponent(&a2, 3);
    set_exponent(&a3, 3);
    set_exponent(&a4, 3);
    set_exponent(&a5, 3);
    set_exponent(&a6, 3);

    ck_assert_int_eq(got2, expected2);
    ck_assert_int_eq(got3, expected3);
    ck_assert_int_eq(got4, expected4);
    ck_assert_int_eq(got5, expected5);
    ck_assert_int_eq(got6, expected6);
}
END_TEST;

Suite *suite_s21_from_decimal_to_int(void) {
    Suite *s = suite_create("suite_s21_from_decimal_to_int");
    TCase *tc = tcase_create("s21_from_decimal_to_int_tc");

    tcase_add_loop_test(tc, no_exp_decimals_to_int, 0, 100);
    tcase_add_test(tc, exp_decimals_to_int_hardcoded);
    // tcase_add_loop_test(tc, exp_decimals_to_int, 0, 100);

    suite_add_tcase(s, tc);
    return s;
}

// * Note on the conversion from decimal type to int:*
// * If there is a fractional part in a decimal number, it should be discarded (for example, 0.9 is converted to 0)*
// 873123.1233 (e-4) => 873123.0000