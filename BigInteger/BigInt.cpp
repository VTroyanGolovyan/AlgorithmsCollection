#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <ctime>
class BigInteger {
public:

    BigInteger () {
        sign_ = true;
    };

    BigInteger (int b) {
        sign_ = b >= 0;
        b = b < 0 ? -b : b;
        number_.clear();
        do {
            number_.push_back(b % base_);
            b /= base_;
        } while (b != 0);
    }

    BigInteger (size_t b, bool sign) {
        sign_ = sign;
        number_.clear();
        do {
            number_.push_back(b % base_);
            b /= base_;
        } while (b != 0);
    }

    ~BigInteger () = default;

    BigInteger (const BigInteger& b) {
        base_ = b.base_;
        sign_ = b.sign_;
        number_ = b.number_;
    }

    BigInteger (const bool sign, const std::vector<size_t>& number, size_t base) {
        base_ = base;
        sign_ = sign;
        number_ =  number;
    }

    BigInteger (const std::string& s) {
        sign_ = s[0] != '-';
        size_t num_count = (s.length() - (s[0] == '-' ? 1 : 0)) / baze_pow_;
        size_t number;
        bool has = false;
        for (size_t  i = 0; i < num_count; ++i) {
            number = 0;
            size_t j = 0;
            size_t k = s.length() - (i + 1) * baze_pow_ + j;
            while (j != baze_pow_  && k < s.length()) {
                number = number * 10 + static_cast<size_t>(s[k]) - 48;
                ++j;
                k = s.length() - (i + 1) * baze_pow_ + j;
            }
            number_.push_back(number);
            has = true;
        }
        number = 0;
        for (size_t i = (s[0] == '-' ? 1 : 0); i < s.length() - num_count * baze_pow_; ++i) {
            number = number * 10 + static_cast<size_t>(s[i]) - 48;
        }
        if (number != 0 || !has) {
            number_.push_back(number);
        }
    }

    BigInteger& operator = (const BigInteger& b) {
        if (this == &b) {
            return *this;
        }
        base_ = b.base_;
        sign_ = b.sign_;
        number_ = b.number_;
        return *this;
    }

    BigInteger& operator = (BigInteger&& b) noexcept {
        if (this == &b) {
            return *this;
        }
        base_ = b.base_;
        sign_ = b.sign_;
        number_ = b.number_;
        return *this;
    }

    BigInteger& operator ++ () {
        *this = *this + BigInteger(1);
        return *this;
    }

    BigInteger operator ++ (int) {
        BigInteger tmp = *this;
        *this = *this + BigInteger(1);
        return tmp;
    }

    BigInteger operator -- (int) {
        BigInteger tmp = *this;
        *this = *this - BigInteger(1);
        return tmp;
    }

    BigInteger& operator += (const BigInteger& a) {
        *this = *this + a;
        return *this;
    }

    BigInteger& operator -= (const BigInteger& a) {
        *this = *this - a;
        return *this;
    }

    BigInteger& operator *= (const BigInteger& a) {
        *this = *this * a;
        return *this;
    }

    BigInteger& operator /= (const BigInteger& a) {
        *this = *this / a;
        return *this;
    }

    BigInteger& operator %= (const BigInteger& a) {
        *this = *this % a;
        return *this;
    }

    friend BigInteger operator + (const BigInteger& a, const BigInteger& b) {
        if (!a.sign_ && b.sign_) {
            return b - (-a);
        }
        if (a.sign_ && !b.sign_) {
            return a - (-b);
        }
        size_t tmp = 0;
        std::vector <size_t> new_int;
        size_t n = a.number_.size() > b.number_.size() ? a.number_.size() : b.number_.size();
        for (size_t i = 0; i < n; ++i) {
            if (i < a.number_.size()) {
                tmp += a.number_[i];
            }
            if (i < b.number_.size()) {
                tmp += b.number_[i];
            }
            new_int.push_back(tmp % a.base_);
            tmp -= tmp % a.base_;
            tmp = tmp / a.base_;
        }
        if (tmp != 0) {
            new_int.push_back(tmp);
        }
        return BigInteger(a.sign_, new_int, a.base_);
    }

    friend BigInteger operator - (const BigInteger& a, const BigInteger& b) {
        if (!a.sign_ && !b.sign_)
            return  -((-a) - (-b));
        if (!a.sign_) {
            return  -(-a + b);
        }
        if (!b.sign_) {
            return  a + (-b);
        }
        bool result_sign = a >= b;
        BigInteger x =  a < b ? b : a;
        BigInteger y = a < b ? a : b;
        ssize_t tmp = 0;
        std::vector <size_t> new_int;
        for (size_t i = 0; i < y.number_.size(); ++i) {
            tmp += x.number_[i] - y.number_[i];
            if (tmp < 0) {
                new_int.push_back(y.base_ + tmp);
                tmp = -1;
            } else {
                new_int.push_back(tmp % y.base_);
                tmp /= y.base_;
            }
        }
        for (size_t i = y.number_.size(); i < x.number_.size(); ++i) {
            tmp += x.number_[i];
            if (tmp < 0) {
                new_int.push_back(x.base_ + tmp);
                tmp = -1;
            } else {
                new_int.push_back(tmp % y.base_);
                tmp /= y.base_;
            }
        }
        if (new_int.empty()) {
            return BigInteger("0");
        }
        while (new_int[new_int.size() - 1] == 0 && new_int.size() > 1) {
            new_int.resize(new_int.size() - 1);
        }
        return BigInteger(result_sign, new_int, a.base_);
    }
    /* Multiply */
    friend BigInteger operator * (const BigInteger& a, const BigInteger& b) {

        BigInteger x = a >= 0 ? a : -a;
        BigInteger y = b >= 0 ? b : -b;
        if (x == BigInteger(0) || y == BigInteger(0))
            return BigInteger("0");
        size_t n = x.number_.size() > y.number_.size() ? x.number_.size() : y.number_.size();
        if (n == 1) {
            return BigInteger(x.toInt() * y.toInt(), a.sign_ == b.sign_);
        }
        size_t n_fix = n;

        size_t shift = 0;
        size_t x_shift = 0;
        size_t y_shift = 0;

        if (x > y) {
            shift += x.number_.size() - y.number_.size();
            y_shift = x.number_.size() - y.number_.size();
        } else {
            shift += y.number_.size() - x.number_.size();
            x_shift = y.number_.size() - x.number_.size();
        }
        if ((x.number_.size() + x_shift) % 2 != 0) {
            shift += 2;
            ++x_shift;
            ++y_shift;
            ++n_fix;
        }

        y.shiftLeft(y_shift);
        x.shiftLeft(x_shift);

        BigInteger x_l = x.getBigIntegerWithNumbers(0, n_fix / 2);
        BigInteger x_r = x.getBigIntegerWithNumbers(n_fix / 2, n_fix);
        BigInteger y_l = y.getBigIntegerWithNumbers(0, n_fix / 2);
        BigInteger y_r = y.getBigIntegerWithNumbers(n_fix / 2, n_fix);
        BigInteger first_multiply = x_l * y_l;
        BigInteger second_multiply;
        second_multiply = x_r * y_r;
        BigInteger third_multiply = (x_l + x_r) * (y_l + y_r);
        BigInteger calc = third_multiply - first_multiply - second_multiply;

        first_multiply.shiftLeft(n_fix);
        calc.shiftLeft(n_fix / 2);
        BigInteger result = first_multiply  + calc + second_multiply;

        if (a.sign_ != b.sign_) {
            result = -result;
        }
        if (result != BigInteger(0))
            return result.shiftRight(shift);
        else return result;
    }
    /* / operator */
    friend BigInteger operator / (const BigInteger& a, const BigInteger& b) {
        if (b == BigInteger(0))
            return BigInteger(0);
        if (a.sign_ && !b.sign_) {
            return  -(a / (-b));
        }
        if (!a.sign_ && b.sign_) {
            return  -((-a) / b);
        }
        if (!a.sign_ && !b.sign_) {
            return (-a) / (-b);
        }
        if (b > a) {
            return BigInteger("0");
        }
        if (b < a.base_) {
            return a.shortDiv((size_t)b);
        }
        ssize_t range_helper = a.number_.size() - b.number_.size();

        BigInteger left(makeDecPow(range_helper - 1));
        BigInteger right(makeDecPow(range_helper + 1));
        BigInteger tmp = left + right;
        BigInteger tmpres;
        BigInteger difference = a - b;
        while (right - left > 1) {
            tmp = left + right;
            tmp = tmp.shortDiv(2);
            tmpres = tmp * b;
            if (tmpres > difference && tmpres <= a)
                return tmp;
            if (tmpres > a) {
                right = tmp;
            } else {
                left = tmp;
            }
        }
        if (left * b > difference && left * b <= a)
            return left;
        if (right * b > difference && right * b <= a)
            return right;
        return tmp;

    }

    friend BigInteger operator % (const BigInteger& a, const BigInteger& b) {
        return a - b * (a / b);
    }

    static std::string makeDecPow(ssize_t pow) {
        if (pow <= 0) {
            return "0";
        }
        std::string t = "1";
        for (ssize_t i = 0; i < pow; ++i) {
            t += "000000000";
        }
        return t;
    }
    /* To string method */
    std::string toString () const {
        std::string s;
        if (!sign_ && !(number_.size() == 1 && number_[0] == 0)) {
            s += "-";
        }
        for (size_t i = 0; i < number_.size(); ++i) {
            std::string number;
            s += intToString(number_[number_.size() - i - 1], i != 0);
        }
        return s;
    }
    /* Unary + */
    BigInteger operator - () const {
        return BigInteger(!sign_, number_, base_);
    }
    /* Unary - */
    BigInteger operator + () const {
        return BigInteger(!sign_, number_, base_);
    }

    friend bool operator < (const BigInteger& a, const BigInteger& b) {
        if (a.sign_ && !b.sign_) {
            return false;
        }
        if (!a.sign_ && b.sign_) {
            return true;
        }
        if (!a.sign_ && !b.sign_) {
            return -a > -b;
        }

        if (a.number_.size() > b.number_.size()) {
            return false;
        } else if (a.number_.size() < b.number_.size()) {
            return true;
        } else {
            for (size_t i = 0; i < a.number_.size(); ++i) {
                size_t a_index = a.number_.size() - i - 1;
                size_t b_index = b.number_.size() - i - 1;
                if (a.number_[a_index] < b.number_[b.number_.size() - i - 1]) {
                    return a.sign_;
                } else if (a.number_[a_index] > b.number_[b_index]) {
                    return !a.sign_;
                }
            }
            return false;
        }
    }
    /* inequalities */
    friend bool operator > (const BigInteger& a, const BigInteger& b) {
        return b < a;
    }

    friend bool operator >= (const BigInteger& a, const BigInteger& b) {
        return !(a < b);
    }

    friend bool operator <= (const BigInteger& a, const BigInteger& b) {
        return !(a > b);
    }

    friend bool operator != (const BigInteger& a, const BigInteger& b) {
        return a < b || b < a;
    }

    friend bool operator == (const BigInteger& a, const BigInteger& b) {
        return !(a != b);
    }
    /* out operator */
    friend std::ostream& operator << (std::ostream& out, const BigInteger& a) {
        out << a.toString();
        return out;
    }

    /* in operator */
    friend std::istream& operator >> (std::istream& in, BigInteger& a) {
        std::string s;
        in >> s;
        a = BigInteger(s);
        return in;
    }

    explicit operator bool () const {
        return *this != BigInteger("0");
    }

    explicit operator double () const {
        ssize_t num = 0;
        for (size_t i = 0; i < number_.size(); ++i) {
            num *= base_;
            num += number_[number_.size() - 1 - i];
        }
        return num;
    }
    explicit operator size_t () const {
        size_t num = 0;
        for (size_t i = 0; i < number_.size(); ++i) {
            num *= base_;
            num += number_[number_.size() - 1 - i];
        }
        return num;
    }
private:
    BigInteger getBigIntegerWithNumbers (size_t l, size_t r) const {
        std::vector<size_t> new_nums;
        for (size_t i = l; i < r && i < number_.size(); ++i) {
            new_nums.push_back(number_[number_.size() - 1 - i]);
        }
        for (size_t i = 0; i < new_nums.size() / 2; ++i) {
            size_t tmp = new_nums[i];
            new_nums[i] = new_nums[new_nums.size() - 1 - i];
            new_nums[new_nums.size() - 1 - i] = tmp;
        }
        while (new_nums.size() > 1 && new_nums[new_nums.size() - 1] == 0) {
            new_nums.pop_back();
        }
        if (!new_nums.empty()) {
            return BigInteger(true, new_nums, base_);
        } else {
            return BigInteger("0");
        }

    }

    std::string intToString (size_t a, bool f=true) const {
        std::string result;
        size_t divide = base_;
        for (size_t i = 0; i < baze_pow_; ++i) {
            char num = static_cast<char>(a % divide / (divide / 10) + 48);
            if (num != '0' || f) {
                result += num;
                f = true;
            }
            divide /= 10;
        }
        return result.empty() ? "0" : result;
    }

    /* b < base */
    BigInteger shortDiv (size_t b) const {
        size_t carry = 0;
        std::vector<size_t> res(number_.size(), 0);
        for (size_t i = 0; i < number_.size(); ++i) {
            size_t index = number_.size() - 1 - i;
            size_t current = number_[index] + carry * base_;
            res[index] = current / b;
            carry = current % b;
        }
        while (res.size() > 1 && res[res.size() - 1] == 0) {
            res.pop_back();
        }
        return BigInteger(true, res, base_);
    }

    void shiftLeft (size_t n) {
        if (n == 0 || *this == BigInteger("0"))
            return;
        std::vector<size_t> tmp = number_;
        for (size_t i = 0; i < n; ++i) {
            number_.push_back(0);
        }
        for (size_t i = 0; i < number_.size() - n; ++i) {
            number_[number_.size() - 1 - i] = number_[number_.size() - n - 1 - i];
        }
        for (size_t i = 0; i < n; ++i) {
            number_[i] = 0;
        }
    }

    BigInteger shiftRight (size_t n) {
        if (n == 0) {
            return *this;
        }
        for (size_t i = 0; i < number_.size() - n; ++i) {
            number_[i] = number_[i + n];
        }
        number_.resize(number_.size() - n);
        return *this;
    }

    size_t toInt () const {
        ssize_t res = 0;
        for (size_t i = 0; i < number_.size(); ++i) {
            res = res * base_ + number_[number_.size() - 1 - i];
        }
        if (!sign_)
            return -res;
        return res;
    }
    size_t base_ = 1000000000;
    size_t baze_pow_ = 9;
    bool sign_;
    std::vector <size_t> number_;
};

class Rational {
public:

    Rational (int b) {
        numerator_ = BigInteger(b);
        denominator_ = BigInteger(1);
    }

    Rational () {
        numerator_ = BigInteger(1);
        denominator_ = BigInteger(1);
    }

    Rational (const BigInteger &b) {
        numerator_ = b;
        denominator_ = BigInteger(1);
    }

    Rational (const BigInteger &a, const BigInteger &b) {
        if (a > 0 && b < 0)
            numerator_ = -a;
        else if (a < 0 && b < 0) {
            numerator_ = -a;
        } else {
            numerator_ = a;
        }
        denominator_ = b >= 0 ? b : -b;
        reduce();
    }

    Rational (const Rational& b) {
        numerator_ = b.numerator_;
        denominator_ = b.denominator_;
    }

    std::string toString() {
        if (denominator_ == 1) {
            return numerator_.toString();
        } else {
            return numerator_.toString() + "/" + denominator_.toString();
        }
    }

    std::string asDecimal(size_t precision = 0) {

        BigInteger tmp = numerator_ / denominator_;
        BigInteger new_numerator = numerator_ % denominator_;
        std::string result = tmp.toString();
        if (precision > 0) {
            result += ".";
            for (size_t i = 0; i < precision; ++i) {
                new_numerator *= 10;
                tmp = new_numerator / denominator_;
                if (tmp < 0) {
                    tmp = -tmp;
                }
                new_numerator %= denominator_;
                if (i != precision - 1)
                  result += tmp.toString();
            }
            new_numerator *= 10;
            BigInteger next_tmp = new_numerator / denominator_;
            if (next_tmp >= 5 || next_tmp <= -5) {
                ++tmp;
            }
            result += tmp.toString();
        }
        if (numerator_ < 0 && result[0] != '-')
            return '-' + result;
        return result;
    }

    Rational operator - () const {
        return -1 * *this;
    }

    Rational operator + () const {
        return *this;
    }

    Rational& operator += (const Rational& a) {
        *this = *this + a;
        return *this;
    }

    Rational& operator -= (const Rational& a) {
        *this = *this - a;
        return *this;
    }

    Rational& operator *= (const Rational& a) {
        *this = *this * a;
        return *this;
    }

    Rational& operator /= (const Rational& a) {
        *this = *this / a;
        return *this;
    }

    friend Rational operator + (const Rational& a, const Rational&b) {
        return Rational(a.numerator_ * b.denominator_ + b.numerator_ * a.denominator_, a.denominator_*b.denominator_);
    }

    friend Rational operator - (const Rational& a, const Rational&b) {
        return Rational(a.numerator_ * b.denominator_ - b.numerator_ * a.denominator_, a.denominator_*b.denominator_);
    }

    friend Rational operator * (const Rational& a, const Rational&b) {
        return Rational(a.numerator_ * b.numerator_ , a.denominator_*b.denominator_);
    }

    friend Rational operator / (const Rational& a, const Rational&b) {
        return Rational(a.numerator_ * b.denominator_, a.denominator_*b.numerator_);
    }

    friend bool operator < (const Rational& a, const Rational& b) {
        Rational tmp = a / b;
        return tmp.numerator_ < tmp.denominator_;
    }

    friend bool operator > (const Rational& a, const Rational& b) {
        return b < a;
    }

    friend bool operator >= (const Rational& a, const Rational& b) {
        return !(a < b);
    }

    friend bool operator <= (const Rational& a, const Rational& b) {
        return !(a > b);
    }

    friend bool operator == (const Rational& a, const Rational& b) {
        return !(a < b) && !(a > b);
    }

    friend bool operator != (const Rational& a, const Rational& b) {
        return !(a == b);
    }

    explicit operator double () {
        return  ((double)numerator_ ) / ((double)denominator_);
    }
private:
    void reduce () {
        BigInteger x = numerator_ >= 0 ? numerator_ : -numerator_;
        BigInteger y = denominator_ >= 0 ? denominator_ : - denominator_;
        while (x != 0 && y != 0) {
            if (x > y) {
                x = x % y;
            } else {
                y = y % x;
            }
        }
        numerator_ /= x + y;
        denominator_ /= x + y;
    }
    BigInteger numerator_;
    BigInteger denominator_;
};

int main() {

    BigInteger a("9000");
    BigInteger b("5000");
    BigInteger c("4");
    BigInteger d("-5");
    BigInteger e("7");
    BigInteger f("505");
    BigInteger p("-25000");

    assert(a + b == BigInteger("14000"));
    assert(a + c == BigInteger("9004"));
    assert(a + d == BigInteger("8995"));
    assert(a + d == BigInteger("8995"));

    assert(b - a == BigInteger("-4000"));
    assert(a - c == BigInteger("8996"));
    assert(a - d == BigInteger("9005"));
    assert(d - e == BigInteger("-12"));
    assert(c - e == BigInteger("-3"));

    assert(-a == BigInteger("-9000"));
    assert(-d == BigInteger("5"));

    assert(!(c > e));
    assert(b > c);

    assert(c * e == BigInteger("28"));assert(b * d == BigInteger("-25000"));
    assert(b * e == BigInteger("35000"));
    assert(c * e == BigInteger("28"));
    assert(f * d == BigInteger("-2525"));

    BigInteger x;
    BigInteger y;
    Rational s;
    Rational t = -402365939;
    ((s = 1000000007) *= 1000000009) *= 2147483647;
    std::cout << (t/s).asDecimal(25);
    return 0;
}
