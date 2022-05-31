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

START_TEST(exp_decimals_to_int_hardcoded1) {
    int got1 = 1;

    s21_decimal a1 = {0};

    /* All bit values are reversed (!) */
    /* https://onlinestringtools.com/split-string */
    a1.bits[0] = 0b00011111111110111011011010100011; /* Dec: 3312312.312 */
    set_exponent(&a1, 3);

    int code = s21_from_decimal_to_int(a1, &got1);
    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(got1, 3312312);
}
END_TEST;

START_TEST(exp_decimals_to_int_hardcoded2) {
    int got2 = 2;
    s21_decimal a2 = {0};

    a2.bits[0] = 0b11001100011100011100011100011010; /* Dec: 8.8888888888888888888888883 */
    a2.bits[1] = 0b00101111011101110001111001001010;
    a2.bits[2] = 0b00101111011000011001001000000000;
    set_exponent(&a2, 25);

    int code = s21_from_decimal_to_int(a2, &got2);
    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(got2, 8);
}
END_TEST;

START_TEST(exp_decimals_to_int_hardcoded3) {
    int got3 = 3;
    s21_decimal a3 = {0};

    a3.bits[0] = 0b11001101110011100000000000000000; /* Dec: 123123123.123123 */
    a3.bits[1] = 0b00010000001101011010111111111011; /* Dec: 555 */
    set_exponent(&a3, 6);
    int code = s21_from_decimal_to_int(a3, &got3);
    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(got3, 123123123);
}
END_TEST;

START_TEST(exp_decimals_to_int_gmp) {
    int got = 1;
    s21_decimal in = {0};

    char npz_bin_str[1024] = {'\0'};

    for (int j = 0, m = 0; j < 4; j++) {
        for (int i = 0; i < 32; i++) {
            int bit = (rand() % 2);

            SET_BIT(in.bits[j], bit, i);

            if (bit) {
                npz_bin_str[m] = '1';
            } else {
                npz_bin_str[m] = '0';
            }
            m++;
        }
    }

    for (int j = 0; j < 3; j++) {
        in.bits[j] = reverse_bits_u32(in.bits[j]);
    }

    printf("%s \n\n\n", npz_bin_str);

    int exp = get_rand(0, 25);
    set_exponent(&in, exp);

    mpz_t n;

    mpz_init(n);
    char str[] = "10101010";
    mpz_set_ui(n, 0); /* UI - Unsigned int, SI - signed int */
    int flag = mpz_set_str(n, npz_bin_str, 2);
    printf("FLAG FLAG FLAG %d \n\n\n", flag);

    /* https://gmplib.org/manual/Integer-Division */

    mpz_t z;
    mpz_init(z);
    mpz_set_ui(z, 0);

    mpz_t desyat;
    mpz_init(desyat);
    mpz_set_ui(desyat, 10);

    printf("\n");
    printf("\n");
    printf("======= N =============\n");
    mpz_out_str(stdout, 10, n);
    printf("\n======= TEN =============\n");
    mpz_out_str(stdout, 10, desyat);
    printf("\n========= Z ==========\n");
    mpz_out_str(stdout, 10, z);
    printf("\n=====================\n");
    printf("\n");
    printf("\n");

    mpz_fdiv_q(z, n, desyat);

    // void mpz_fdiv_q(mpz_t q, const mpz_t n, const mpz_t d)

    printf("\n");
    printf("\n");
    printf("======= N =============\n");
    mpz_out_str(stdout, 10, n);
    printf("\n======= TEN =============\n");
    mpz_out_str(stdout, 10, desyat);
    printf("\n========= Z ==========\n");
    mpz_out_str(stdout, 10, z);
    printf("\n=====================\n");
    printf("\n");
    printf("\n");

    int code = s21_from_decimal_to_int(in, &got);
    ck_assert_int_eq(code, ARITHMETIC_OK);
    ck_assert_int_eq(got, n);
}
END_TEST;

Suite *suite_s21_from_decimal_to_int(void) {
    Suite *s = suite_create("suite_s21_from_decimal_to_int");
    TCase *tc = tcase_create("s21_from_decimal_to_int_tc");

    tcase_add_loop_test(tc, no_exp_decimals_to_int, 0, 100);
    tcase_add_test(tc, exp_decimals_to_int_hardcoded1);
    tcase_add_test(tc, exp_decimals_to_int_hardcoded2);
    tcase_add_test(tc, exp_decimals_to_int_hardcoded3);
    tcase_add_loop_test(tc, exp_decimals_to_int_gmp, 0, 5);

    suite_add_tcase(s, tc);
    return s;
}
