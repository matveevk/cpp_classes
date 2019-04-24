#include <iostream>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }
    Optional(T && elem) {
        new (data) T(std::move(elem));
        defined = true;
    }
    Optional(const Optional& other) {
        if (other.has_value())
            new (data) T(other.value());
        defined = other.has_value();
    }

    Optional& operator=(const Optional& other) {
        if (other.has_value()) {
            if (has_value())
                value() = other.value();
            else
                new (data) T(other.value());
        } else if (has_value()) {
            reinterpret_cast<T*>(data)->~T();
        }
        defined = other.has_value();
        return *this;
    }
    Optional& operator=(const T& elem) {
        if (has_value())
            value() = elem;
        else
            new (data) T(elem);
        defined = true;
        return *this;
    }
    Optional& operator=(T&& elem) {
        if (has_value())
            value() = std::move(elem);
        else
            new (data) T(std::move(elem));
        defined = true;
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }
    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (!has_value())
            throw BadOptionalAccess();
        return **this;
    }
    const T& value() const {
        if (!has_value())
            throw BadOptionalAccess();
        return **this;
    }

    void reset() {
        if (has_value())
            reinterpret_cast<T*>(data)->~T();
        defined = false;
    }

    ~Optional() {
        if (has_value())
            reinterpret_cast<T*>(data)->~T();
    }
};
