#include <stdio.h>

#include "../s21_decimal.h"
#include "../tests/s21_decimal_test.h"

// static void s21_sum_handle_signs(int s1, int s2, s21_decimal value_1, s21_decimal value_2,
//                                  s21_decimal *result, int *code);

// int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//     memset(result, 0, sizeof(*result));
//     int code = ARITHMETIC_OK;

//     s21_normalize_decimal_pair(&value_1, &value_2, &code);

//     int s1 = get_sign(value_1), s2 = get_sign(value_2);

//     s21_sum_handle_signs(s1, s2, value_1, value_2, result, &code);

//     // if (code && get_exponent(value_1) && get_exponent(value_2)) {
//     //     set_sign_pos(&value_1);
//     //     set_sign_pos(&value_2);

//     //     printf("E1: %d E2: %d\n", get_exponent(value_1), get_exponent(value_2));
//     //     print_python(value_1);
//     //     print_python(value_2);

//     //     s21_bank_rounding(&value_1, 1);
//     //     s21_bank_rounding(&value_2, 1);

//     //     printf("E1: %d E2: %d\n", get_exponent(value_1), get_exponent(value_2));
//     //     print_python(value_1);
//     //     print_python(value_2);

//     //     set_sign(&value_1, s1);
//     //     set_sign(&value_2, s2);

//     //     // code = s21_add(value_1, value_2, result);
//     //     // s21_sum_handle_signs(s1, s2, value_1, value_2, result, &code);
//     // }

//     set_exponent(result, get_exponent(value_1));

//     return s21_check_infinity(code, get_sign(*result));
// }

int s21_check_infinity(int code, int sign) {
    int res = ARITHMETIC_OK;

    if (code == S21_INFINITY) {
        if (sign == POS)
            res = S21_INFINITY;
        else
            res = S21_NEGATIVE_INFINITY;
    }

    return res;
}

// static void s21_sum_handle_signs(int s1, int s2, s21_decimal value_1, s21_decimal value_2,
//                                  s21_decimal *result, int *code) {
//     if (s1 == POS && s2 == POS) {
//         *result = binary_addition(value_1, value_2, code);
//     } else if (s1 == POS && s2 == NEG) {
//         set_sign_pos(&value_2);
//         if (s21_is_greater(value_1, value_2)) {
//             *result = binary_subtraction(value_1, value_2, code);
//             set_sign_neg(result);
//         } else {
//             *result = binary_subtraction(value_2, value_1, code);
//         }
//     } else if (s1 == NEG && s2 == POS) {
//         set_sign_pos(&value_1);
//         if (s21_is_greater(value_2, value_1)) {
//             *result = binary_subtraction(value_2, value_1, code);
//             set_sign_neg(result);
//         } else {
//             *result = binary_subtraction(value_1, value_2, code);
//         }
//     } else if (s1 == NEG && s2 == NEG) {
//         set_sign_pos(&value_1);
//         set_sign_pos(&value_2);
//         *result = binary_addition(value_1, value_2, code);
//         set_sign_neg(result);
//     }
// }

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    memset(result, 0, sizeof(*result));
    int code = ARITHMETIC_OK;
    int s1 = get_sign(value_1);
    int s2 = get_sign(value_2);
    set_sign_pos(&value_1);
    set_sign_pos(&value_2);
    if (s1 == POS && s2 == POS) {
        handle_exponent(value_1, value_2, result, &code);
    } else if (s1 == POS && s2 == NEG) {
        if (s21_is_greater(value_1, value_2)) {
            s21_sub(value_1, value_2, result);
        } else {
            s21_sub(value_2, value_1, result);
            set_sign_neg(result);
        }
    } else if (s1 == NEG && s2 == POS) {
        if (s21_is_greater(value_1, value_2)) {
            s21_sub(value_2, value_1, result);
        } else {
            s21_sub(value_1, value_2, result);
        }
    } else if (s1 == NEG && s2 == NEG) {
        handle_exponent(value_1, value_2, result, &code);
        set_sign_neg(result);
    }
    return code;
}

void handle_exponent(s21_decimal value_1, s21_decimal value_2, s21_decimal *result, int *code) {
    int e1 = get_exponent(value_1);
    int e2 = get_exponent(value_2);
    set_exponent(&value_1, 0);
    set_exponent(&value_2, 0);

    s21_decimal big = e1 > e2 ? value_2 : value_1;
    int e_big = min(e1, e2);
    s21_decimal small = e1 > e2 ? value_1 : value_2;
    int e_small = max(e1, e2);

    int of_check = 0;
    while (e_big < e_small) {
        s21_decimal big_overflow = big;
        binary_multiplication(big_overflow, get_power_of_ten(1), &of_check);
        if (of_check)
            break;
        e_big++;
    }
    while (e_big < e_small) {
        // [n][0-4] => n
        // [n][5] => (if (n % 2) == 1) n += 1;
        // [n][6-9] => n += 1

        // 123 / 1 % 10 == 3
        // 123 / 10 % 100 == 2

        s21_decimal ten = get_power_of_ten(1);
        s21_decimal last = {0};
        s21_decimal before_last = {0};
        s21_int_mod(small, ten, &last);
        s21_int_div(small, ten, &before_last);
        s21_int_mod(before_last, ten, &before_last);

        int ilast, ibefore_last;
        s21_from_decimal_to_int(last, &ilast);
        s21_from_decimal_to_int(before_last, &ibefore_last);

        int never_error = 0;
        s21_int_mod(small, ten, &small);
        if (ilast < 5) {
        } else if (ilast == 5) {
            if (ibefore_last % 2 == 1) {
                small = binary_addition(small, get_power_of_ten(0), &never_error);
            }
        } else if (ilast > 5) {
            small = binary_addition(small, get_power_of_ten(0), &never_error);
        }
        e_small--;
    }
    *result = binary_addition(big, small, code);
    set_exponent(result, e_big);
}