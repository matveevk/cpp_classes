#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<char*>(&pod), sizeof(T));
}

void Serialize(const std::string& str, std::ostream& out) {
    size_t sz = str.size();
    out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
    out.write(str.data(), sizeof(char) * sz);
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t sz = data.size();
    out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
    for (const T& elem : data) {
        Serialize(elem, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t sz = data.size();
    out.write(reinterpret_cast<char*>(&sz), sizeof(size_t));
    for (const auto& pr : data) {
        Serialize(pr.first, out);
        Serialize(pr.second, out);
    }
}


template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(T));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
    str.resize(sz);
    for (char& ch : str) {
        in.read(&ch, sizeof(char));
    }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
    data.resize(sz);
    for (T& elem : data) {
        Deserialize(in, elem);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    data.clear();
    size_t sz;
    in.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
    for (size_t i = 0; i != sz; ++i) {
        T1 key;
        T2 val;
        Deserialize(in, key);
        Deserialize(in, val);
        data[key] = val;
    }
}
