#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    if (!get_exponent(src) && dst) {
        *dst = 0;
        for (int i = 0, j = sizeof(int) * 4 - 1; i < sizeof(int) * 4; i++, j--) {
            if (IS_SET(src.bits[0], j)) {
                SET_BIT(*dst, 1, i);
            }
        }
    } else {
        // divive number by 10 exponent times. Then launch
        // s21_from_decimal_to_int(src, dst);
    }

    return ARITHMETIC_OK;
}