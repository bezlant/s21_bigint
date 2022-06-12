#include "../s21_decimal.h"
#include "../tests/s21_decimal_test.h"

static s21_decimal s21_integer_div(s21_decimal a, s21_decimal b, s21_decimal *result, int *code);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (eq_zero(value_2)) return S21_NAN;

    int code = ARITHMETIC_OK;

    /* TODO: add mod checking to determine which version of division to launch (INT) vs (FLOAT) */

    handle_exponent_div(value_1, value_2, result, &code);

    if (get_sign(value_1) != get_sign(value_2)) set_sign_neg(result);

    return code;
}

void handle_exponent_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code) {
    int exp_v1 = get_exponent(value_1);
    int exp_v2 = get_exponent(value_2);

    set_exponent(&value_1, 0);
    set_exponent(&value_2, 0);

    int res_exp = exp_v1 - exp_v2;

    /* Edge case. Division by 1 */
    if (s21_is_equal(value_2, get_power_of_ten(0))) {
        *result = value_1;
        *code = ARITHMETIC_OK;
    } else {
        /* We must set 1 bit in result, because we will move this bit in div to the left */
        init_zero(result);
        result->bits[0] = 1;

        *result = s21_integer_div(value_1, value_2, result, code);

        while (res_exp < 0) {
            *result = binary_multiplication(*result, get_power_of_ten(1), code);
            res_exp++;
        }

        set_exponent(result, res_exp);
    }
}

static s21_decimal s21_integer_div(s21_decimal a, s21_decimal b, s21_decimal *result, int *code) {
    s21_decimal divcopy = b;
    s21_decimal temp = {0};
    s21_decimal one = {0};
    one.bits[0] = 1;

    if (s21_is_equal(a, b)) {
        return one;
    } else if (s21_is_less(a, b)) {
        return temp;
    }

    /* Our goal is to align b & a, so we are shifting b to the left */

    /**
     * a: 0101010101010101
     * b: 0000000000000101 <---
     *
     * As a result we will get something like this:
     *
     * a: 0101010101010101
     * b: 1010000000000000 <---
     *
     * We got too far by one bit. Thus, we need to shift (b) to the right once.
     */

    while (s21_is_less_or_equal(b, a)) {
        shiftl(&b);
        shiftl(result);
    }

    /* Shifting (b) once */
    if (s21_is_less(a, b)) {
        shiftr(&b);
        shiftr(result);
    }

    /**
     *  a: 0101010101010101
     *  b: 0101000000000000
     *
     *  An actial division is done via substraction.
     *
     * @arg (temp) stores new value of (a) after substraction. It will later
     * be passed to the recursive call of sivision.
     */

    *code = s21_sub(a, b, &temp);

    /* DIRTY HACK (!) Sometimes for unknown reasons sub incorrectly sets 95th bit. */
    /* But, generally, values are correct.  */
    set_bit_0(&temp, 95);

    /**
     * @arg (divcopy) is nesessary to divide by non-modified version of divisor,
     * because our original divisor was modified by shifting to the left.
     */

    one = s21_integer_div(temp, divcopy, &one, code);

    /* @arg (result) accumulates result of division */
    *code = s21_add(*result, one, result);

    return *result;
}
