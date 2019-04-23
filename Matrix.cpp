#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Matrix;

template <typename T>
class NonConstMatrixIterator {
    Matrix<T>& matrix;
    size_t pos_i, pos_j;

public:
    NonConstMatrixIterator(Matrix<T>& matrix, size_t pos_i, size_t pos_j)
            : matrix(matrix), pos_i(pos_i), pos_j(pos_j) {}

    bool operator == (const NonConstMatrixIterator& other) const {
        return pos_i == other.pos_i && pos_j == other.pos_j;
    }

    bool operator != (const NonConstMatrixIterator& other) const {
        return !(*this == other);
    }

    NonConstMatrixIterator& operator ++() {
        ++pos_j;
        if (pos_j >= matrix.size().second) {
            ++pos_i;
            pos_j = 0;
        }
        return *this;
    }

    T& operator *() {
        return matrix[pos_i][pos_j];
    }
};


template <typename T>
class ConstMatrixIterator {
    const Matrix<T>& matrix;
    size_t pos_i, pos_j;

public:
    ConstMatrixIterator(const Matrix<T>& matrix, size_t pos_i, size_t pos_j)
            : matrix(matrix), pos_i(pos_i), pos_j(pos_j) {}

    bool operator == (const ConstMatrixIterator& other) const {
        return pos_i == other.pos_i && pos_j == other.pos_j;
    }

    bool operator != (const ConstMatrixIterator& other) const {
        return !(*this == other);
    }

    ConstMatrixIterator& operator ++() {
        ++pos_j;
        if (pos_j >= matrix.size().second) {
            ++pos_i;
            pos_j = 0;
        }
        return *this;
    }

    const T& operator *() const {
        return matrix[pos_i][pos_j];
    }
};


template <typename T>
class Matrix {
    vector<vector<T>> rows;

public:
    Matrix() = default;

    Matrix(const vector<vector<T>>& rows) {
        this->rows = rows;
    }

    Matrix(size_t countRows, size_t countColumns) {
        rows = vector<vector<T>>(countRows, vector<T>(countColumns));
    }

    explicit Matrix(pair<size_t, size_t> sz) {
        rows = vector<vector<T>>(sz.first, vector<T>(sz.second));
    }

    pair<size_t, size_t> size() const {
        if (rows.empty())
            return {0, 0};
        return {rows.size(), rows[0].size()};
    }

    const vector<T>& operator[](size_t i) const {
        return rows[i];
    }

    vector<T>& operator[](size_t i) {
        return rows[i];
    }

    const vector<T>& back() const {
        return rows.back();
    }

    Matrix& operator += (const Matrix& other) {
        for (size_t i = 0; i != rows.size(); ++i) {
            for (size_t j = 0; j != rows[i].size(); ++j) {
                rows[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix operator + (const Matrix& other) const {
        Matrix result = *this;
        for (size_t i = 0; i != rows.size(); ++i) {
            for (size_t j = 0; j != rows[i].size(); ++j) {
                result[i][j] += other[i][j];
            }
        }
        return result;
    }

    template <typename Num>
    Matrix& operator *= (Num k) {
        for (size_t i = 0; i != rows.size(); ++i) {
            for (size_t j = 0; j != rows[i].size(); ++j) {
                rows[i][j] *= k;
            }
        }
        return *this;
    }

    template <typename Num>
    Matrix operator * (Num k) const {
        Matrix result = *this;
        result *= k;
        return result;
    }

    Matrix& transpose() {
        vector<vector<T>> newRows(size().second, vector<T>(size().first));
        for (size_t i = 0; i != rows.size(); ++i) {
            for (size_t j = 0; j != rows[i].size(); ++j) {
                newRows[j][i] = rows[i][j];
            }
        }
        rows = newRows;
        return *this;
    }

    Matrix transposed() const {
        vector<vector<T>> newRows(size().second, vector<T>(size().first));
        for (size_t i = 0; i != rows.size(); ++i) {
            for (size_t j = 0; j != rows[i].size(); ++j) {
                newRows[j][i] = rows[i][j];
            }
        }
        return Matrix(newRows);
    }

    Matrix& operator *= (const Matrix& other) {
        assert(size().second == other.size().first);
        vector<vector<T>> newRows(size().first, vector<T>(other.size().second));
        for (size_t i = 0; i != newRows.size(); ++i) {
            for (size_t k = 0; k != size().second; ++k) {
                for (size_t j = 0; j != newRows[i].size(); ++j) {
                    newRows[i][j] += rows[i][k] * other[k][j];
                }
            }
        }
        rows = newRows;
        return *this;
    }

    Matrix operator * (const Matrix& other) const {
        Matrix result = *this;
        result *= other;
        return result;
    }

    const ConstMatrixIterator<T> begin() const {
        return {*this, 0, 0};
    }

    NonConstMatrixIterator<T> begin() {
        return {*this, 0, 0};
    }

    const ConstMatrixIterator<T> end() const {
        if (size().first == 0 || size().second == 0)
            return begin();
        return {*this, size().first, 0};
    }

    NonConstMatrixIterator<T> end() {
        if (size().first == 0 || size().second == 0)
            return begin();
        return {*this, size().first, 0};
    }
};

template <typename T>
ostream& operator <<(ostream& out, const Matrix<T>& matrix) {
    if (matrix.size().first != 0 && matrix.size().second != 0) {
        for (size_t i = 0; i + 1 != matrix.size().first; ++i) {
            for (size_t j = 0; j + 1 != matrix.size().second; ++j)
                out << matrix[i][j] << "\t";
            out << matrix[i].back() << "\n";
        }
        for (size_t j = 0; j + 1 != matrix.size().second; ++j)
            out << matrix.back()[j] << "\t";
        out << matrix.back().back();
    } else if (matrix.size().first != 0) {
        for (size_t i = 0; i + 1 != matrix.size().first; ++i)
            out << "\n";
    }
    return out;
}
