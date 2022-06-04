#include "../s21_decimal_test.h"

s21_decimal convert_gmp_to_decimal(mpz_t input) {
    s21_decimal res = {0};
    /* https://gmplib.org/manual/Integer-Import-and-Export */
    mpz_export(res.bits, 0, 1, sizeof(res.bits[0]), -1, 0, input);
    return res;
}

void convert_decimal_to_gmp(mpz_t *gmp, s21_decimal *dec) {
    /**
     * Import:
     *      into GMP
     *      3 words
     *      little -> big bit
     *      sizeof word is sizeof unsigned int
     *      little -> big inside of word
     *      (???)
     *      from dec.bits
     */


    /* Initialize z and a */

    mpz_import(*gmp, 3, 1, sizeof(dec->bits[0]), 0, 0, dec->bits);
    
    apply_exponent_to_mpz(gmp, get_exponent(*dec));

    if (get_sign(*dec))
        mpz_neg(*gmp, *gmp);
}