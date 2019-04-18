class Rational {
    int numer, denom;

    int abs(int a) {
        if (a < 0)
            return -a;
        return a;
    }

    int gcd(int a, int b) {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    void reduceFraction() {
        int div = gcd(abs(numer), abs(denom));
        numer /= div;
        denom /= div;
        if (denom < 0) {
            numer *= -1;
            denom *= -1;
        }
    }

public:
    Rational(int numer = 0, int denom = 1) {
        this->numer = numer;
        this->denom = denom;
        reduceFraction();
    }

    Rational operator + (const Rational& other) const {
        return {numer * other.denom + other.numer * denom, denom * other.denom};
    }

    Rational operator - (const Rational& other) const {
        return *this + (-other);
    }

    Rational operator * (const Rational& other) const {
        return {numer * other.numer, denom * other.denom};
    }

    Rational operator / (const Rational& other) const {
        return {numer * other.denom, denom * other.numer};
    }

    Rational operator +() const {
        return *this;
    }

    Rational operator -() const {
        return *this * -1;
    }

    Rational& operator += (const Rational& other) {
        numer = numer * other.denom + other.numer * denom;
        denom *= other.denom;
        reduceFraction();
        return *this;
    }

    Rational& operator -= (const Rational& other) {
        numer = numer * other.denom - other.numer * denom;
        denom *= other.denom;
        reduceFraction();
        return *this;
    }

    Rational& operator *= (const Rational& other) {
        numer *= other.numer;
        denom *= other.denom;
        reduceFraction();
        return *this;
    }

    Rational& operator /= (const Rational& other) {
        *this = *this / other;
        return *this;
    }

    int numerator() const {
        return numer;
    }

    int denominator() const {
        return denom;
    }

    bool operator == (const Rational& other) const {
        return numer == other.numer && denom == other.denom;
    }

    bool operator != (const Rational& other) const {
        return !(*this == other);
    }

    Rational& operator ++() {
        numer += denom;
        return *this;
    }

    Rational& operator --() {
        numer -= denom;
        return *this;
    }

    Rational operator ++(int) {
        Rational temp(*this);
        numer += denom;
        return temp;
    }

    Rational operator --(int) {
        Rational temp(*this);
        numer -= denom;
        return temp;
    }
};

Rational operator + (int num, const Rational& rational) {
    return Rational(num) + rational;
}

Rational operator - (int num, const Rational& rational) {
    return Rational(num) - rational;
}

Rational operator * (int num, const Rational& rational) {
    return Rational(num) * rational;
}

Rational operator / (int num, const Rational& rational) {
    return Rational(num) / rational;
}

Rational operator == (int num, const Rational& rational) {
    return Rational(num) == rational;
}

Rational operator != (int num, const Rational& rational) {
    return Rational(num) != rational;
}
