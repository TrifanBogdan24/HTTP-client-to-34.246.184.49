#ifndef BUFFER_H
#define BUFFER_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

class Buffer {
 public:
    Buffer();   // constructor
    ~Buffer();  // destructor

    void add(const char* data, size_t data_size);
    int find(const char* str, size_t str_size) const;
    int find_insensitive(const char* str, size_t str_size) const;

    const char* get_data() const;
    size_t get_size() const;

 private:
    char* data;
    size_t size;
    size_t capacity;

    void ensure_capacity(size_t new_capacity);
};

Buffer::Buffer() : data(nullptr), size(0), capacity(0) {}

Buffer::~Buffer() {
    free(data);
}

void Buffer::add(const char* data, size_t data_size) {
    ensure_capacity(size + data_size);
    std::memcpy(this->data + size, data, data_size);
    size += data_size;
}

int Buffer::find(const char* str, size_t str_size) const {
    for (size_t i = 0; i <= size - str_size; ++i) {
        if (std::memcmp(data + i, str, str_size) == 0) {
            return i;
        }
    }
    return -1;
}

int Buffer::find_insensitive(const char* str, size_t str_size) const {
    auto tolower = [](unsigned char c) { return std::tolower(c); };
    for (size_t i = 0; i <= size - str_size; ++i) {
        if (std::equal(str, str + str_size, data + i, [tolower](char a, char b) {
            return tolower(a) == tolower(b);
        })) {
            return i;
        }
    }
    return -1;
}

const char* Buffer::get_data() const {
    return data;
}

size_t Buffer::get_size() const {
    return size;
}

void Buffer::ensure_capacity(size_t new_capacity) {
    if (new_capacity > capacity) {
        capacity = std::max(new_capacity, capacity * 2);
        data = static_cast<char*>(std::realloc(data, capacity));
        if (!data) {
            throw std::bad_alloc();
        }
    }
}

#endif // BUFFER_H
