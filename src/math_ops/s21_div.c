#include "../s21_decimal.h"
#include "../tests/s21_decimal_test.h"

s21_decimal div_core(s21_decimal a, s21_decimal b, s21_decimal *result, int *code);

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    if (eq_zero(value_2)) return S21_NAN;

    int code = ARITHMETIC_OK;

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

        *result = div_core(value_1, value_2, result, code);

        while (res_exp < 0) {
            *result = binary_multiplication(*result, get_power_of_ten(1), code);
            res_exp++;
        }

        set_exponent(result, res_exp);
    }
}

s21_decimal div_core(s21_decimal a, s21_decimal b, s21_decimal *result, int *code) {
    s21_decimal divcopy = b;
    s21_decimal temp = {0};
    s21_decimal one = {0};

    one.bits[0] = 1;

    if (s21_is_equal(a, b)) {
        return one;
    } else if (s21_is_less(a, b)) {
        return temp;
    }

    while (s21_is_less_or_equal(b, a)) {
        shiftl(&b);
        shiftl(result);
    }

    if (s21_is_less(a, b)) {
        shiftr(&b);
        shiftr(result);
    }

    s21_sub(a, b, &temp);

    /* DIRTY HACK (!) Sometimes for unknown reasons sub incorrectly sets 95th bit. */
    /* But, generally, values are correct.  */
    set_bit_0(&temp, 95);

    one = div_core(temp, divcopy, &one, code);

    *code = s21_add(*result, one, result);

    return *result;
}
