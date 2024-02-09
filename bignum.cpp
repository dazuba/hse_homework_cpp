#include "bignum.h"

BigNum& BigNum::operator+=(const BigNum& other) {
    BigNum other_copy = other;
    if (sign == 1 && other_copy.sign == -1) {
        other_copy.sign = 1;
        *this -= other_copy;
        return *this;
    }
    if (sign == -1 && other_copy.sign == 1) {
        sign = 1;
        *this -= other_copy;
        sign *= -1;
        return *this;
    }
    if (sign == -1 && other_copy.sign == -1) {
        sign = 1;
        other_copy.sign = 1;
        *this += other_copy;
        sign = -1;
        return *this;
    }

    if (other_copy > *this) {
        BigNum x = other_copy;
        x += *this;
        *this = x;
        return *this;
    }
    while (exp < other_copy.exp) {
        num.push_back(0);
        exp++;
    }
    while (exp > other_copy.exp) {
        other_copy.num.push_back(0);
        other_copy.exp++;
    }
    int over = 0;
    for (int i = 0; i < num.size(); i++) {
        int over1 = 0;
        if (i < other_copy.num.size()) {
            over1 = (num[num.size() - i - 1] + other_copy.num[other_copy.num.size() - i - 1] + over) / 10;
            num[num.size() - i - 1] = (num[num.size() - i - 1] + other_copy.num[other_copy.num.size() - i - 1] + over) % 10;
        } else {
            over1 = (num[num.size() - i - 1] + over) / 10;
            num[num.size() - i - 1] = (num[num.size() - i - 1] + over) % 10;
        }
        over = over1;
    }
    if (over) {
        num.insert(num.begin(), over);
    }
    this->norm();
    return *this;
}

BigNum& BigNum::operator-=(const BigNum& other) {
    BigNum other_copy = other;
    if (sign == 1 && other_copy.sign == -1) {
        other_copy.sign = 1;
        *this += other_copy;
        return *this;
    }
    if (sign == -1 && other_copy.sign == 1) {
        sign = 1;
        *this += other_copy;
        sign *= -1;
        return *this;
    }
    if (sign == -1 && other_copy.sign == -1) {
        sign = 1;
        other_copy.sign = 1;
        *this -= other_copy;
        sign *= -1;
        return *this;
    }

    if (other_copy > *this) {
        BigNum x = other_copy;
        x -= *this;
        *this = -x;
        return *this;
    }
    while (exp < other_copy.exp) {
        num.push_back(0);
        exp++;
    }
    while (exp > other_copy.exp) {
        other_copy.num.push_back(0);
        other_copy.exp++;
    }
    int over = 0;
    for (int i = 0; i < num.size(); i++) {
        int over1 = 0;
        if (i < other_copy.num.size()) {
            if (num[num.size() - i - 1] - other_copy.num[other_copy.num.size() - i - 1] - over < 0)
                over1 = 1;
            num[num.size() - i - 1] = over1 * 10 + num[num.size() - i - 1] - other_copy.num[other_copy.num.size() - i - 1] - over;
        } else {
            if (num[num.size() - i - 1] - over < 0)
                over1 = 1;
            num[num.size() - i - 1] = over1 * 10 + num[num.size() - i - 1] - over;
        }
        over = over1;
    }
    this->norm();
    return *this;
}
