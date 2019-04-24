#include <iostream>

template <typename T>
struct Node {
    T data;
    Node * prev = nullptr;
    Node * next = nullptr;

    Node() = default;
    Node(const T& data): data(data) {}
    Node(T && data): data(std::move(data)) {}
    Node(const T& data, Node * prev, Node * next): data(data), prev(prev), next(next) {}
    Node(T && data, Node * prev, Node * next): data(std::move(data)), prev(prev), next(next) {}
};

template <typename T>
class ListIterator {
    Node<T> * node;
    Node<T> * list_back;

public:
    ListIterator(Node<T> * node, Node<T> * list_back) {
        this->node = node;
        this->list_back = list_back;
    }
    ListIterator operator ++() {
        node = node->next;
        return *this;
    }
    ListIterator operator --() {
        if (node == nullptr)
            node = list_back;
        else
            node = node->prev;
        return *this;
    }
    const T& operator*() const {
        return node->data;
    }
    bool operator == (const ListIterator& other) const {
        return node == other.node && list_back == other.list_back;
    }
    bool operator != (const ListIterator& other) const {
        return !(*this == other);
    }
};

template <typename T>
class List {
    Node<T> * front = nullptr;
    Node<T> * back = nullptr;
    size_t sz = 0;

public:
    List() = default;

    List(const List& other) {
        sz = 0;
        if (other.size()) {
            auto curr = other.front;
            for (size_t i = 0; i != other.size(); ++i) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
    }

    List& operator=(const List& other) {
        this->~List();
        sz = 0;
        if (other.size()) {
            auto curr = other.front;
            for (size_t i = 0; i != other.size(); ++i) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    void push_front(const T& elem) {
        if (front == nullptr) {
            front = back = new Node<T>(elem);
        } else {
            front = new Node<T>(elem, nullptr, front);
            front->next->prev = front;
        }
        ++sz;
    }

    void push_front(T&& elem) {
        if (front == nullptr) {
            front = back = new Node<T>(elem);
        } else {
            front = new Node<T>(elem, nullptr, front);
            front->next->prev = front;
        }
        ++sz;
    }

    void push_back(const T& elem) {
        if (front == nullptr) {
            push_front(elem);
        } else {
            back = new Node<T>(elem, back, nullptr);
            back->prev->next = back;
            ++sz;
        }
    }

    void push_back(T&& elem) {
        if (front == nullptr) {
            push_front(elem);
        } else {
            back = new Node<T>(elem, back, nullptr);
            back->prev->next = back;
            ++sz;
        }
    }

    void pop_front() {
        if (front != nullptr && front == back) {
            delete front;
            front = back = nullptr;
            sz = 0;
        } else if (front != nullptr) {
            front = front->next;
            delete front->prev;
            front->prev = nullptr;
            --sz;
        }
    }

    void pop_back() {
        if (back != nullptr && front == back) {
            delete back;
            front = back = nullptr;
            sz = 0;
        } else if (back != nullptr) {
            back = back->prev;
            delete back->next;
            back->next = nullptr;
            --sz;
        }
    }

    const size_t size() const {
        return sz;
    }

    ListIterator<T> begin() {
        return {front, back};
    }

    ListIterator<T> end() {
        return {nullptr, back};
    }

    ~List() {
        while (front != back) {
            front = front->next;
            delete front->prev;
        }
        delete back;
    }
};
