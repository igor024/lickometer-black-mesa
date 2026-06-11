#pragma once

#include <string>

#define FILE_WRITE 0x01

class File {
public:
    explicit operator bool() const {
        return true;
    }

    template <typename T>
    void print(const T&) {}
};

class SdFat {
public:
    bool begin() {
        return true;
    }

    bool exists(const char*) {
        return true;
    }

    bool mkdir(const char*) {
        return true;
    }

    File open(const char*, int) {
        return File{};
    }
};
