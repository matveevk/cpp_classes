#include <memory>

template <typename T>
class Vector {
    T * array = nullptr;
    size_t sz = 0;
    size_t cp = 0;

    void realloc(size_t newCap) {
        T * newArray = reinterpret_cast<T *>(new char[newCap * sizeof(T)]);
        try {
            std::uninitialized_copy_n(array, sz, newArray);
        } catch (...) {
            delete [] reinterpret_cast<char *>(newArray);
            throw;
        }

        this->~Vector();
        array = newArray;
        cp = newCap;
    }

public:
    Vector(size_t newSize = 0) {
        try {
            array = reinterpret_cast<T *>(new char[newSize * sizeof(T)]);
            if (newSize)
                std::uninitialized_fill_n(array, newSize, T());
        } catch (...) {
            delete [] reinterpret_cast<char *>(array);
            throw;
        }
        cp = sz = newSize;
    }
    Vector(size_t newSize, const T& elem) {
        try {
            array = reinterpret_cast<T *>(new char[newSize * sizeof(T)]);
            std::uninitialized_fill_n(array, newSize, elem);
        } catch (...) {
            delete [] reinterpret_cast<char *>(array);
            throw;
        }
        cp = sz = newSize;
    }
    Vector(const Vector& other) {
        try {
            array = reinterpret_cast<T *>(new char[other.size() * sizeof(T)]);
            std::uninitialized_copy_n(other.array, other.size(), array);
        } catch (...) {
            delete [] reinterpret_cast<char *>(array);
            throw;
        }
        cp = sz = other.size();
    }
    Vector&operator=(const Vector& other) {
        T * newArray = reinterpret_cast<T *>(new char[other.size() * sizeof(T)]);
        try {
            std::uninitialized_copy_n(other.array, other.size(), newArray);
        } catch (...) {
            delete [] reinterpret_cast<char *>(newArray);
            throw;
        }
        this->~Vector();
        array = newArray;
        cp = sz = other.size();
        return *this;
    }

    void push_back(const T& elem) {
        if (size() == 0) {
            realloc(1);
        } else if (size() == capacity()) {
            realloc(capacity() * 2);
        }
        new (array + sz) T(elem);
        ++sz;
    }

    void push_back(T&& elem) {
        if (size() == 0) {
            realloc(1);
        } else if (size() == capacity()) {
            realloc(capacity() * 2);
        }
        new (array + sz) T(std::move(elem));
        ++sz;
    }

    void pop_back() {
        array[sz - 1].~T();
        --sz;
    }

    void reserve(size_t newCap) {
        if (newCap <= capacity())
            return;
        realloc(newCap);
    }

    void resize(size_t newSize, const T& elem) {
        if (newSize < size()) {
            for (size_t i = newSize; i != size(); ++i)
                array[i].~T();
            sz = newSize;
        } else if (capacity() < newSize) {
            T * newArray = reinterpret_cast<T *>(new char[newSize * sizeof(T)]);
            try {
                std::uninitialized_copy_n(array, sz, newArray);
                std::uninitialized_fill_n(newArray + sz, newSize - sz, elem);
            } catch (...) {
                delete [] reinterpret_cast<char *>(newArray);
                throw;
            }
            this->~Vector();
            array = newArray;
            sz = cp = newSize;
        } else {
            std::uninitialized_fill_n(array + sz, newSize - sz, elem);
            sz = newSize;
        }
    }
    void resize(size_t newSize) {
        if (newSize < size()) {
            for (size_t i = newSize; i != size(); ++i)
                array[i].~T();
            sz = newSize;
        } else if (capacity() < newSize) {
            T * newArray = reinterpret_cast<T *>(new char[newSize * sizeof(T)]);
            try {
                std::uninitialized_copy_n(array, sz, newArray);
                std::uninitialized_default_construct_n(newArray + sz, newSize - sz);
            } catch (...) {
                delete [] reinterpret_cast<char *>(newArray);
                throw;
            }
            this->~Vector();
            array = newArray;
            sz = cp = newSize;
        } else {
            std::uninitialized_default_construct_n(array + sz, newSize - sz);
            sz = newSize;
        }
    }

    size_t size() const {
        return sz;
    }
    size_t capacity() const {
        return cp;
    }

    const T& operator[](size_t i) const {
        return array[i];
    }
    T& operator[](size_t i) {
        return array[i];
    }

    void clear() {
        for (size_t i = 0; i != sz; ++i)
            array[i].~T();
        sz = 0;
    }

    void swap(Vector& other) {
        std::swap(array, other.array);
        std::swap(sz, other.sz);
        std::swap(cp, other.cp);
    }

    const T * begin() const {
        return array;
    }
    T * begin() {
        return array;
    }
    const T * end() const {
        return array + sz;
    }
    T * end() {
        return array + sz;
    }

    ~Vector() {
        for (size_t i = 0; i != sz; ++i)
            array[i].~T();
        delete [] reinterpret_cast<char *>(array);
    }
};
