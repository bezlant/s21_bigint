#include "../s21_decimal.h"

/**
 * @brief Round down the given number
 * Simply truncated the number (will discard zeroes)
 * (exp = 7) 51351.5315153 becomes 51351
 * Then multiply by the pow(10, exponent)
 * (exp = 7) 51351 becomes 51351.0000000
 *
 * For negatives we add one (
 * i.e -1321.14121 becomes -1322.00000
 * )
 *
 * @param value given value
 * @param result truncated value
 * @return returns an error code
 */

int s21_floor(s21_decimal value, s21_decimal *result) {
    memset(result, 0, sizeof(*result));
    int exponent = get_exponent(value);
    int sign = get_sign(value);
    set_sign_pos(&value);

    s21_decimal truncated = {0};
    s21_truncate(value, &truncated);

    /* Round down for negative numbers -12.321351 becomes -13.00000 */
    if (sign == NEG && exponent) {
        s21_decimal tmp = {0};
        /* i.e -420.00 - 1 = -421.00*/
        if (s21_add(truncated, get_power_of_ten(0), &tmp))
            return CONVERTATION_ERROR;
        truncated = tmp;
    }

    int tmp_exp = exponent;
    if (!tmp_exp) {
        *result = value;
    } else {
        while (tmp_exp--) {
            if (s21_mul(truncated, get_power_of_ten(1), result))
                return CONVERTATION_ERROR;
            truncated = *result;
        }
    }

    set_exponent(result, exponent);
    set_sign(result, sign);

    return CONVERTATION_OK;
}
