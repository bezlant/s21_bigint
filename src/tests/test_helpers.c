#include "s21_decimal_test.h"

int reverse_bits_int(int in) {
    int res = 0;
    int i = 0, j = 15;

    while (i < 16) {
        SET_BIT(res, IS_SET(in, j), i);
        i++;
        j--;
    }

    return res;
}

unsigned int reverse_bits_u32(unsigned int n) {
    unsigned int res = 0;
    for (int i = 0; i < 32; i++) {
        res = res << 1;
        res = res | (n & 1);

        n = n >> 1;
    }

    return res;
}

void print_bits(s21_decimal d) {
    for (int i = 0; i < 3; i++) {
        int x = 3;
        for (int j = 0; j < 32; j++) {
            if (IS_SET(d.bits[i], j)) printf("%s", HRED);
            printf("%u", IS_SET(d.bits[i], j));
            if (j == x && !(i == 2 && j == 31)) {
                x += 4;
                printf(" ");
            }
            printf("%s", RESET);
        }
    }
    printf("\n");
}

s21_decimal get_random_int_decimal(void) {
    s21_decimal res;

    set_exponent(&res, 0);

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 31; i++) {
            SET_BIT(res.bits[j], (rand() % 2), i);
        }
    }

    return res;
}

void print_bits_r(s21_decimal d) {
    for (int i = 2; i >= 0; --i) {
        int x = 28;
        for (int j = 31; j >= 0; --j) {
            if (IS_SET(d.bits[i], j)) printf("%s", HRED);
            printf("%u", IS_SET(d.bits[i], j));
            if (j == x && !(i == 0 && j == 0)) {
                x -= 4;
                printf("");
            }
            printf("%s", RESET);
        }
    }
    printf("\n");
}

void print_bits_r_set(s21_decimal d, int set_n) {
    int x = 28;
    for (int j = 15; j >= 0; --j) {
        if (IS_SET(d.bits[set_n], j)) printf("%s", HRED);
        printf("%u", IS_SET(d.bits[set_n], j));
        if (j == x && !(set_n == 0 && j == 0)) {
            x -= 4;
            printf("");
        }
        printf("%s", RESET);
    }

    printf("\n");
}

void print_bits_set(s21_decimal d, int set_n) {
    int x = 28;
    for (int j = 0; j < 16; ++j) {
        if (IS_SET(d.bits[set_n], j)) printf("%s", HRED);
        printf("%u", IS_SET(d.bits[set_n], j));
        if (j == x && !(set_n == 0 && j == 0)) {
            x -= 4;
            printf("");
        }
        printf("%s", RESET);
    }

    printf("\n");
}
