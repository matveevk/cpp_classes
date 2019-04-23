#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Polynomial {
    vector<T> coef;

    void reduceZeros() {
        T zero = T(0);
        while (!coef.empty() && coef.back() == zero)
            coef.pop_back();
        coef.push_back(zero);
    }

    const T& largest() const {
        if (Degree() == -1)
            return coef.back();
        return coef[Degree()];
    }

public:
    Polynomial(const vector<T> &coef) {
        this->coef = coef;
        reduceZeros();
    }

    Polynomial(T constant = T()) {
        coef = {constant};
        reduceZeros();
    }

    template <typename Iter>
    Polynomial(Iter first, Iter last) {
        coef = vector<T>(first, last);
        reduceZeros();
    }

    typename vector<T>::const_iterator begin() const {
        return coef.begin();
    }

    typename vector<T>::const_iterator end() const {
        if (coef.empty())
            return coef.begin();
        return std::prev(coef.end());
    }

    T operator[] (size_t power) const {
        if (power < coef.size())
            return coef[power];
        return coef.back();
    }

    bool operator == (const Polynomial& other) const {
        return coef == other.coef;
    }

    bool operator != (const Polynomial& other) const {
        return !(*this == other);
    }

    int Degree() const {
        return static_cast<int>(coef.size()) - 2;
    }

    Polynomial operator +() const {
        return *this;
    }

    Polynomial operator -() const {
        return *this * T(-1);
    }

    Polynomial& operator += (const Polynomial& other) {
        coef.resize(max(coef.size(), other.coef.size()));
        for (size_t i = 0; i != other.coef.size(); ++i) {
            coef[i] += other.coef[i];
        }
        reduceZeros();
        return *this;
    }

    Polynomial& operator -= (const Polynomial& other) {
        coef.resize(max(coef.size(), other.coef.size()));
        for (size_t i = 0; i != other.coef.size(); ++i) {
            coef[i] -= other.coef[i];
        }
        reduceZeros();
        return *this;
    }

    Polynomial& operator *= (const Polynomial& other) {
        if (Degree() == -1 || other.Degree() == -1)
            return *this = Polynomial(T(0));
        auto thisSize = static_cast<size_t>(Degree() + 1);
        auto otherSize = static_cast<size_t>(other.Degree() + 1);
        vector<T> resultCoef(thisSize + otherSize);
        for (size_t i = 0; i != thisSize; ++i) {
            for (size_t j = 0; j != otherSize; ++j) {
                resultCoef[i + j] += coef[i] * other.coef[j];
            }
        }
        return *this = Polynomial(resultCoef);
    }

    Polynomial operator + (const Polynomial& other) const {
        Polynomial result(*this);
        result += other;
        return result;
    }

    Polynomial operator - (const Polynomial& other) const {
        Polynomial result(*this);
        result -= other;
        return result;
    }

    Polynomial operator * (const Polynomial& other) const {
        Polynomial result(*this);
        result *= other;
        return result;
    }

    T operator () (const T& x) const {
        T result(0), power(1);
        for (const T& c : coef) {
            result += c * power;
            power *= x;
        }
        return result;
    }

    Polynomial operator & (const Polynomial& other) const {
        Polynomial result, power{T(1)};
        for (const T& c : coef) {
            result += c * power;
            power *= other;
        }
        return result;
    }

    Polynomial operator / (const Polynomial& other) const {
        if (Degree() < other.Degree())
            return Polynomial(T(0));
        Polynomial left(*this);
        vector<T> resCoef(static_cast<size_t>(2 + Degree() - other.Degree()));
        for (int power = Degree() - other.Degree(); power >= 0; --power) {
            vector<T> termCoef(static_cast<size_t>(power + 1));
            T curCoef = left[power + other.Degree()] / other.largest();
            resCoef[power] = termCoef[power] = curCoef;
            Polynomial term(termCoef);
            left -= term * other;
        }
        return Polynomial(resCoef);
    }

    Polynomial& operator /= (const Polynomial& other) {
        return *this = *this / other;
    }

    Polynomial operator % (const Polynomial& other) const {
        return *this - (*this / other) * other;
    }

    Polynomial& operator %= (const Polynomial& other) {
        return *this = *this % other;
    }

    Polynomial operator , (const Polynomial& other) const {
        if (Degree() == -1 && other.Degree() == -1)
            return *this;
        else if (other.Degree() == -1)
            return *this / largest();
        return (other, *this % other);
    }
};

template <typename T>
Polynomial<T> operator + (const T& constant, const Polynomial<T>& polynomial) {
    return polynomial + constant;
}

template <typename T>
Polynomial<T> operator - (const T& constant, const Polynomial<T>& polynomial) {
    return polynomial - constant;
}

template <typename T>
Polynomial<T> operator * (const T& constant, const Polynomial<T>& polynomial) {
    return polynomial * constant;
}

template <typename T>
Polynomial<T> operator / (const T& constant, const Polynomial<T>& polynomial) {
    return Polynomial<T>(constant) / polynomial;
}

template <typename T>
Polynomial<T> operator % (const T& constant, const Polynomial<T>& polynomial) {
    return Polynomial<T>(constant) % polynomial;
}

template <typename T>
Polynomial<T> operator , (const T& constant, const Polynomial<T>& polynomial) {
    return (Polynomial<T>(constant), polynomial);
}

// Output format: -x^4+4*x^2+x-1
template <typename T>
ostream& operator <<(ostream& out, const Polynomial<T>& p) {
    auto one = T(1), minusOne = T(-1), zero = T(0);
    int deg = p.Degree();
    if (deg == -1) {
        out << "0";
    } else if (deg == 0) {
        out << p[0];
    } else if (deg == 1) {
        if (p[1] == minusOne)
            out << "-x";
        else if (p[1] == one)
            out << "x";
        else
            out << p[1] << "*x";
        if (p[0] > zero)
            out << "+";
        if (p[0] != zero)
            out << p[0];
    } else {
        if (p[deg] == minusOne)
            out << "-x^" << deg;
        else if (p[deg] == one)
            out << "x^" << deg;
        else
            out << p[deg] << "*x^" << deg;

        for (--deg; deg > 1; --deg) {
            if (p[deg] == one) {
                out << "+x^" << deg;
            } else if (p[deg] == minusOne) {
                out << "-x^" << deg;
            } else if (p[deg] > zero) {
                out << "+" << p[deg] << "*x^" << deg;
            } else if (p[deg] < zero) {
                out << p[deg] << "*x^" << deg;
            }
        }

        if (deg == 1) {
            if (p[1] == one) {
                out << "+x";
            } else if (p[1] == minusOne) {
                out << "-x";
            } else if (p[1] > zero) {
                out << "+" << p[1] << "*x";
            } else if (p[1] < zero) {
                out << p[1] << "*x";
            }
            --deg;
        }

        if (deg == 0) {
            if (p[0] > zero)
                out << "+";
            if (p[0] != zero)
                out << p[0];
        }
    }
    return out;
}
