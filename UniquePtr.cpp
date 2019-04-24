#include <cstddef>
#include <iostream>
#include <memory>

template <typename T, class Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> data;

public:
    UniquePtr() noexcept: data(nullptr, Deleter()) {}

    explicit UniquePtr(T * ptr) noexcept: data(std::move(ptr), Deleter()) {
        ptr = nullptr;
    }

    explicit UniquePtr(T * ptr, const Deleter& deleter) noexcept
    : data(std::move(ptr), deleter) {
        ptr = nullptr;
    }

    UniquePtr(UniquePtr&& other) noexcept
    : data(std::move(other.get()), other.get_deleter()) {
        std::get<T*>(other.data) = nullptr;
    }

    UniquePtr(const UniquePtr& other) = delete;

    UniquePtr& operator = (std::nullptr_t) noexcept {
        std::get<Deleter>(data)(std::get<T*>(data));
        std::get<T*>(data) = nullptr;
        return *this;
    }

    UniquePtr& operator = (UniquePtr&& other) noexcept {
        if (std::get<T*>(data) == other.get())
            return *this;
        std::get<Deleter>(data)(std::get<T*>(data));
        std::get<T*>(data) = std::move(other.get());
        std::get<T*>(other.data) = nullptr;
        return *this;
    }

    UniquePtr& operator = (const UniquePtr& other) = delete;

    T& operator *() const {
        return *std::get<T*>(data);
    }

    T * operator ->() const noexcept {
        return std::get<T*>(data);
    }

    T * release() noexcept {
        T * copy = std::move(std::get<T*>(data));
        std::get<T*>(data) = nullptr;
        return copy;
    }

    void reset(T * new_ptr) noexcept {
        if (std::get<T*>(data) != new_ptr) {
            std::get<Deleter>(data)(std::get<T*>(data));
            std::get<T*>(data) = new_ptr;
        }
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(std::get<T*>(data), std::get<T*>(other.data));
    }

    T * get() const noexcept {
        return std::get<T*>(data);
    }

    explicit operator bool() const noexcept {
        return std::get<T*>(data) != nullptr;
    }

    const Deleter& get_deleter() const {
        return std::get<Deleter>(data);
    }

    Deleter& get_deleter() {
        return std::get<Deleter>(data);
    }

    ~UniquePtr() {
        std::get<Deleter>(data)(std::get<T*>(data));
    }
};
