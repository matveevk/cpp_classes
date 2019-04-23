#include <iostream>
#include <map>
#include <vector>

using namespace std;

template <typename T>
class Polynomial {
    map<int, T> coef;

    void reduceZeros() {
        T zero = T();
        for (auto it = coef.begin(); it != coef.end(); ) {
            if (it->second == zero) {
                it = coef.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    Polynomial(const vector<T> &coef) {
        T zero(0);
        for (size_t power = 0; power != coef.size(); ++power) {
            if (coef[power] != zero) {
                this->coef[power] = coef[power];
            }
        }
    }

    Polynomial(T constant = T()) {
        if (constant != T(0)) {
            coef[0] = constant;
        }
    }

    template <typename Iter>
    Polynomial(Iter first, Iter last) {
        T zero(0);
        for (int dist = 0; first != last; ++first, ++dist) {
            if (*first != zero) {
                coef[dist] = *first;
            }
        }
    }

    typename map<int, T>::const_iterator begin() const {
        return coef.begin();
    }

    typename map<int, T>::const_iterator end() const {
        return coef.end();
    }

    typename map<int, T>::const_reverse_iterator rbegin() const {
        return coef.rbegin();
    }

    typename map<int, T>::const_reverse_iterator rend() const {
        return coef.rend();
    }

    T operator[] (int power) const {
        if (coef.find(power) != coef.end())
            return coef.at(power);
        return T(0);
    }

    bool operator == (const Polynomial& other) const {
        return coef == other.coef;
    }

    bool operator != (const Polynomial& other) const {
        return !(*this == other);
    }

    int Degree() const {
        if (coef.empty())
            return -1;
        return coef.rbegin()->first;
    }

    Polynomial operator +() const {
        return *this;
    }

    Polynomial operator -() const {
        return *this * T(-1);
    }

    Polynomial& operator += (const Polynomial& other) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            coef[it->first] += it->second;
        }
        reduceZeros();
        return *this;
    }

    Polynomial& operator -= (const Polynomial& other) {
        return *this += -other;
    }

    Polynomial& operator *= (const Polynomial& other) {
        Polynomial result;
        for (auto it = begin(); it != end(); ++it) {
            for (auto it2 = other.begin(); it2 != other.end(); ++it2) {
                result.coef[it->first + it2->first] += it->second * it2->second;
            }
        }
        result.reduceZeros();
        return *this = result;
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
        T result(0), term(1);
        int power = 0;
        for (auto it = coef.begin(); it != coef.end(); ++it) {
            while (power < it->first) {
                term *= x;
                ++power;
            }
            result += it->second * term;
        }
        return result;
    }

    Polynomial operator & (const Polynomial& other) const {
        Polynomial result, term{T(1)};
        int power = 0;
        for (auto it = coef.begin(); it != coef.end(); ++it) {
            while (power < it->first) {
                term *= other;
                ++power;
            }
            result += it->second * term;
        }
        return result;
    }

    Polynomial operator / (const Polynomial& other) const {
        if (Degree() < other.Degree())
            return Polynomial(T(0));
        Polynomial left(*this), result;
        for (int power = Degree() - other.Degree(); power >= 0; --power) {
            if (left.coef.find(power + other.Degree()) != left.coef.end()) {
                T curCoef = left[power + other.Degree()] / other[other.Degree()];
                Polynomial term;
                term.coef[power] = result.coef[power] = curCoef;
                left -= term * other;
                left.reduceZeros();
            }
        }
        return result;
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
            return *this / coef.at(Degree());
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
        cout << zero;
    } else {
        int power = p.rbegin()->first;
        T coef = p.rbegin()->second;
        if (coef == one || coef == minusOne) {
            if (coef == minusOne) {
                out << "-";
            }
            if (power > 1) {
                out << "x^" << power;
            } else if (power == 1) {
                out << "x";
            } else {
                out << one;
            }
        } else {
            out << coef;
            if (power > 1) {
                out << "*x^" << power;
            } else if (power == 1) {
                out << "*x";
            }
        }

        for (auto it = next(p.rbegin()); it != p.rend(); ++it) {
            power = it->first;
            coef = it->second;
            if (coef == one || coef == minusOne) {
                if (coef == one) {
                    out << "+";
                } else if (coef == minusOne) {
                    out << "-";
                }
                if (power > 1) {
                    out << "x^" << power;
                } else if (power == 1) {
                    out << "x";
                } else {
                    out << one;
                }
            } else {
                if (coef > zero)
                    out << "+";
                out << coef;
                if (power > 1) {
                    out << "*x^" << power;
                } else if (power == 1) {
                    out << "*x";
                }
            }
        }
    }
    return out;
}
