#include "../s21_decimal_test.h"

s21_decimal convert_gmp_to_decimal(mpz_t input) {
    s21_decimal res = {0};

    /* https://gmplib.org/manual/Integer-Import-and-Export */

// #define mpz_export __gmpz_export
//     __GMP_DECLSPEC void *mpz_export(void *, size_t *, int, size_t, int, size_t, mpz_srcptr);

    mpz_export(res.bits, 0, -1, sizeof(res.bits[0]), 1, 0, input);

    // Macro: int mpz_sgn (const mpz_t op)
    // Return +1  if op > 0, 0 if op = 0, and -1 if op < 0.

    if (mpz_sgn(input) == -1) {
        set_sign_neg(&res);
    }

    printf("#2 (gmp to dec): \n");
    print_mpz_decimal(input);

    return res;
}

void convert_decimal_to_gmp(mpz_t *gmp, s21_decimal *dec) {
    // changed to -1 because we work in litte endian & reversed bites
    mpz_import(*gmp, 3, 0, sizeof(dec->bits[0]), 0, 0, dec->bits);

    printf("#3 (D to GMP (noexp)): \n");
    print_mpz_decimal(*gmp);
    apply_exponent_to_mpz(gmp, get_exponent(*dec));
    printf("#4 (D to GMP (exp)): \n");
    print_mpz_decimal(*gmp);

    if (get_sign(*dec))
        mpz_neg(*gmp, *gmp);
}

/**
 * @brief [TEMPORARY HACK (!)] Applies exponent to decimal by converting it
 * to GMP, dividing GMP and converting back to decimal.
 *
 * @param dec pointer to decimal that needs to be normalized
 */

void tmp_normalize_exponent(s21_decimal *dec) {
    mpz_t tmp;
    mpz_init(tmp);
    mpz_set_ui(tmp, 0);

    convert_decimal_to_gmp(&tmp, dec);
    init_zero(dec);
    *dec = convert_gmp_to_decimal(tmp);

    mpz_clear(tmp);
}

void apply_exponent_to_mpz(mpz_t *src, int exp) {
    mpz_t mpz_ten_const;
    mpz_init(mpz_ten_const);
    mpz_set_ui(mpz_ten_const, 10);

    while (exp) {
        mpz_fdiv_q(*src, *src, mpz_ten_const);
        exp--;
    }

    mpz_clear(mpz_ten_const);
}