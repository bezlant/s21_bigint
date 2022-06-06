#include "../s21_decimal_test.h"

int get_rand(int min, int max) { return (rand() % (max - min + 1)) + min; }

int get_random_pair(s21_decimal *in, mpz_t *in_mpz_copy, int size) {
    /* NOTE: Negatives disabled for now */

    if (rand() % 2)
        set_sign_neg(in);
    else
        set_sign_pos(in);

    char mpz_bin_str[96] = {0};
    for (int j = 0, m = 0; j < size; j++)
        for (int i = 0; i < 32; i++, m++) {
            if (rand() % 2) {
                ADD_BIT(in->bits[j], i);
                mpz_bin_str[m] = '1';
            } else {
                ZERO_BIT(in->bits[j], i);
                mpz_bin_str[m] = '0';
            }
        }

    for (int j = 0; j < 3; j++)
        in->bits[j] = reverse_bits(in->bits[j]);

    /* TODO: Changes needed */
    int exp = get_rand(0, 28);
    set_exponent(in, exp);

    bool res = mpz_set_str(*in_mpz_copy, mpz_bin_str, 2);

/* #define DEBUG */
#if defined(DEBUG)
    printf("%s", HRED);
    printf("EXPONENT WAS: %d\n", exp);
    printf("LINE: %d IN %s\n", __LINE__, __FILE__);
    mpz_out_str(stdout, 10, *in_mpz_copy);
    printf("\n");
#endif

    apply_exponent_to_mpz(in_mpz_copy, exp);

#if defined(DEBUG)
    printf("LINE: %d IN %s\n", __LINE__, __FILE__);
    mpz_out_str(stdout, 10, *in_mpz_copy);
    printf("\n");
    printf("%s", RESET);
#endif

    if (get_sign(*in))
        mpz_neg(*in_mpz_copy, *in_mpz_copy);

    return res;
}

/**
 * @brief Returns random decimal that is in range of [-MAX_UINT32; +MAX_UINT32]
 *
 *
 * @return s21_decimal
 */

s21_decimal get_random_int_decimal(void) {
    s21_decimal res = {0};

    if (rand() % 2)
        set_sign_neg(&res);
    else
        set_sign_pos(&res);

    for (int j = 0; j < 1; j++) {
        for (int i = 31; i > 0; i--) {
            unsigned int x = rand() % 2;
            SET_BIT(res.bits[0], x, i);
        }
    }

    return res;
}

/**
 * @brief Returns random signed decimal of specified size with exponent in range
 * [min; max].
 *
 * @param size - Number of bits that will be set in the integer
 * @param min_exp - min possible exponent
 * @param max_exp - max possible exponent
 *
 * @return s21_decimal
 */

s21_decimal get_random_decimal_size(int size, int min_exp, int max_exp) {
    s21_decimal res = {0};

    int n_words = size / 32;

    if (rand() % 2)
        set_sign_neg(&res);
    else
        set_sign_pos(&res);

    set_exponent(&res, get_rand(min_exp, max_exp));

    for (int j = 0, m = 0; j < n_words && m <= size; j++) {
        for (int i = 0; i < 32 && m <= size; i++) {
            unsigned int x = rand() % 2;
            SET_BIT(res.bits[0], x, i);
            m++;
        }
    }

    return res;
}