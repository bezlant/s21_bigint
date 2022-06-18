#include "../s21_decimal.h"
#include "../tests/s21_decimal_test.h"
#include <stdio.h>

static void handle_exponent_sub(s21_decimal value_1, s21_decimal value_2,
                                s21_decimal *result, int *code);

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int code = ARITHMETIC_OK;

    int s1 = get_sign(value_1), s2 = get_sign(value_2);

    if (s1 == POS && s2 == POS) {
        if (s21_is_greater(value_1, value_2)) {
            handle_exponent_sub(value_1, value_2, result, &code);
        } else {
            handle_exponent_sub(value_2, value_1, result, &code);
            set_sign_neg(result);
        }
    } else if (s1 == POS && s2 == NEG) {
        set_sign_pos(&value_2);
        code = s21_add(value_1, value_2, result);
    } else if (s1 == NEG && s2 == POS) {
        set_sign_pos(&value_1);
        code = s21_add(value_1, value_2, result);
        set_sign_neg(result);
    } else if (s1 == NEG && s2 == NEG) {
        set_sign_pos(&value_2);
        code = s21_add(value_1, value_2, result);
    }

    code = s21_check_infinity(code, get_sign(*result));

    if (code && get_exponent(value_1) && get_exponent(value_2)) {


        s21_bank_rounding(&value_1, 1);
        s21_bank_rounding(&value_2, 1);

        print_python(value_1);
        print_python(value_2);

        *result = binary_subtraction(value_1, value_2, &code);

        set_exponent(result, get_exponent(value_1));
        print_python(*result);
    }

    return code;
}

static void handle_exponent_sub(s21_decimal value_1, s21_decimal value_2,
                                s21_decimal *result, int *code) {
    s21_normalize_decimal_pair(&value_1, &value_2, code);
    set_exponent(result, get_exponent(value_1));
    *result = binary_subtraction(value_1, value_2, code);
}
