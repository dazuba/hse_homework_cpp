#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <ostream>
#include <iostream>


class BigNum {
public:
    int precision = 300;    
    int sign = 1;
    std::vector<int> num{0};
    int exp = 0;

    void norm() {
        reverse(num.begin(), num.end());
        while (exp + 1 > num.size()) {
            num.push_back(0);
        }
        while (exp + 1 < num.size() && num.back() == 0) {
            num.pop_back();
        }
        reverse(num.begin(), num.end());

        while (exp >= 0 && num.back() == 0) {
            num.pop_back();
            exp--;
        }
        while (exp < 0) {
            num.push_back(0);
            exp++;
        }

        if (num.size() == 1 && num[0] == 0) {
            sign = 1;
        }
    }

    BigNum() {}

    BigNum(const std::string& val) {
        sign = 1;
        num = std::vector<int>();
        exp = 0;
        for (int i = 0; i < val.size(); ++i) {
            if (val[i] == '-') {
                sign = -1;
            } else if (val[i] == '.' || val[i] == ',') {
                exp = val.size() - i - 1;
            } else {
                num.push_back(val[i] - '0');
            }
        }
        norm();
    }

    std::string ToString() const {
        std::string res;
        if (sign == -1) 
            res.push_back('-');
        for (int i = 0; i < num.size(); ++i) {
            if (i + exp == num.size()) {
                res.push_back('.');
            }
            res.push_back(num[i] + '0');
        }
        return res;
    }

    BigNum(const BigNum& val) : BigNum(val.ToString()) {}

    friend std::ostream& operator<<(std::ostream& s, const BigNum& val) {
        s << val.ToString();
        return s;
    }

    BigNum& operator=(const BigNum& other) {
        sign = other.sign;
        num = std::vector<int>{other.num};
        exp = other.exp;
        this->norm();
        return *this;
    }

    bool operator>(const BigNum& other) const {
        if (sign == 1 && other.sign == -1) 
            return true;
        if (sign == -1 && other.sign == 1) 
            return false;
        if (num.size() - exp > other.num.size() - other.exp)
            return true;
        if (num.size() - exp < other.num.size() - other.exp)
            return false;
        for (int i = 0; i < std::min(num.size(), other.num.size()); ++i) {
            if (num[i] > other.num[i]) 
                return true;
            if (num[i] < other.num[i])
                return false;
        }
        return num.size() > other.num.size();
    }

    bool operator<(const BigNum& other) const {
        if (other > *this) 
            return true;
        return false;
    }

    bool operator==(const BigNum& other) const {
        return !(other > *this || other < *this);
    }

    bool operator!=(const BigNum& other) const {
        return !(*this == other);
    }

    BigNum operator-() const {
        BigNum res(*this);
        res.sign *= -1;
        res.norm();
        return res;
    }

    BigNum& operator-=(const BigNum& other);

    BigNum& operator+=(const BigNum& other);

    BigNum operator+(const BigNum& other) const {
        BigNum res = *this;
        res += other;
        res.norm();
        return res;
    }

    BigNum operator-(const BigNum& other) const {
        BigNum res = *this;
        res -= other;
        res.norm();
        return res;
    }

    BigNum operator*(int digit) const {
        BigNum res = *this;
        int over = 0;
        for (int i = 0; i < res.num.size(); i++) {
            int over1 = (res.num[num.size() - i - 1] * digit + over) / 10;
            res.num[num.size() - i - 1] = (res.num[res.num.size() - i - 1] * digit + over) % 10;
            over = over1;
        }
        if (over) {
            res.num.insert(res.num.begin(), over);
        }
        res.norm();
        return res;
    }

    BigNum operator*(const BigNum& other) const {
        BigNum res("0");
        if (sign == 1 && other.sign == -1) {
            res = -(*this * -other);
            res.norm();
            return res;
        }
        if (sign == -1 && other.sign == 1) {
            res = -(-(*this) * other);
            res.norm();
            return res;
        }
        if (sign == -1 && other.sign == -1) {
            res = -(*this) * -other;
            res.norm();
            return res;
        }
        BigNum this_copy = *this;
        BigNum other_copy = other;
        if (exp != 0 || other.exp != 0) {
            this_copy.exp = 0;
            other_copy.exp = 0;
            this_copy.norm();
            other_copy.norm();
            res = this_copy * other_copy;
            res.exp = exp + other.exp;
            res.norm();
            return res;
        }

        for (int i = 0; i < num.size(); i++) {
            res = res * 10;
            res += other * num[i];
        }
        res.norm();
        return res;
    }

    BigNum& operator*=(const BigNum& other) {
        *this = *this * other;
        this->norm();
        return *this;
    }

    BigNum operator/(const BigNum& other) const {
        BigNum res("0");
        res.exp--;
        res.sign = sign * other.sign;
        auto this_copy = *this;
        auto other_copy = other;
        this_copy.sign = 1;
        other_copy.sign = 1;

        int mx_exp = std::max(this_copy.sign, other_copy.sign);
        this_copy.exp -= mx_exp;
        other_copy.exp -= mx_exp;
        this_copy.norm();
        other_copy.norm();

        while (other_copy.num.size() < this_copy.num.size()) {
            other_copy.num.push_back(0);
            res.exp--;
        }
        for (int p = 0; p < precision && this_copy != BigNum("0"); p++) {
            for (int i = 9; i >= 0; i--) {
                auto cur = other_copy * i;
                if (!(this_copy < cur)) {
                    this_copy -= cur;
                    res.num.push_back(i);
                    res.exp++;
                    other_copy.exp++;
                    other_copy.norm();
                    break;
                }
            }
        }

        res.norm();
        return res;
    }

    BigNum& operator/=(const BigNum& other) {
        *this = *this / other;
        this->norm();
        return *this;
    }

    static BigNum PI() {

        BigNum res("0");
        BigNum x("1");
        BigNum k("0");
        for (int i = 0; i < 100; i++) {
            BigNum d1 = BigNum("8") * k + BigNum("1");
            BigNum d2 = BigNum("8") * k + BigNum("4");
            BigNum d3 = BigNum("8") * k + BigNum("5");
            BigNum d4 = BigNum("8") * k + BigNum("6");
            res += x * (BigNum("4") / d1 - BigNum("2") / d2 - BigNum("1") / d3 - BigNum("1") / d4);

            x /= BigNum("16");
            k += BigNum("1");
        }
        return res;
    }
};