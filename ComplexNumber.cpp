#include <iostream>
#include <cmath>

class Complex {
    double re, im;

public:
    Complex(double re, double im = 0): re(re), im(im) {}

    double Re() const {
        return re;
    }

    double Im() const {
        return im;
    }

    double& Re() {
        return re;
    }

    double& Im() {
        return im;
    }

    Complex operator + (const Complex& other) const {
        return {re + other.re, im + other.im};
    }

    Complex operator - (const Complex& other) const {
        return {re - other.re, im - other.im};
    }

    Complex operator * (const Complex& other) const {
        return {re * other.re - im * other.im,
                re * other.im + im * other.re};
    }

    Complex inv() const {
        double squaredAbs = re * re + im * im;
        return {re / squaredAbs, -im / squaredAbs};
    }

    Complex operator / (const Complex& other) const {
        return *this * other.inv();
    }

    Complex& operator += (const Complex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }

    Complex& operator -= (const Complex& other) {
        re -= other.re;
        im -= other.im;
        return *this;
    }

    Complex& operator *= (const Complex& other) {
        re = re * other.re - im * other.im;
        im = re * other.im + im * other.re;
        return *this;
    }

    Complex& operator /= (const Complex& other) {
        return *this *= other.inv();
    }

    Complex operator -() const {
        return {-re, -im};
    }

    Complex operator +() const {
        return {re, im};
    }

    bool operator == (const Complex& other) const {
        return re == other.re && im == other.im;
    }

    bool operator != (const Complex& other) const {
        return !(*this == other);
    }
};

Complex operator + (double num, const Complex& complex) {
    return Complex(num) + complex;
}

Complex operator - (double num, const Complex& complex) {
    return Complex(num) - complex;
}

Complex operator * (double num, const Complex& complex) {
    return Complex(num) * complex;
}

Complex operator / (double num, const Complex& complex) {
    return Complex(num) / complex;
}

double abs(const Complex& complex) {
    return sqrt(complex.Re() * complex.Re() + complex.Im() * complex.Im());
}
