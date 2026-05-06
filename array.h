#pragma once

#include <cstddef>

template <typename T>
class Array {
public:
    // ======================== Constructors ========================

    Array() {}       

    explicit Array(size_t size): data_(new T[size]), size_(size) {} 

    Array(size_t size, const T& value): data_(new T[size]), size_(size) {
        std::fill(data_, data_ + size_, value);
    }                              

    // =================== Copy and Move ===========================

    Array(const Array& other): data_(new T[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Array(Array&& other): data_(other.data_), size_(other.size_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    Array& operator=(const Array& other) {
        if (this == &other) return *this;

        Array temp(other);
        swap(temp);
        return *this;
    }

    Array& operator=(Array&& other) {
        if (this == &other) return *this;

        delete[] this->data_;

        this->size_ = other.size_;
        this->data_ = other.data_;

        other.size_ = 0;
        other.data_ = nullptr;

        return *this;
    }

    // =================== Destructor ==============================

    ~Array() {
        delete[] data_;
    }

    // =================== Element access ==========================

    T&       operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T&       front() {
        return data_[0];
    }

    const T& front() const {
        return data_[0];
    }

    T&       back() {
        return data_[size_ - 1];
    }

    const T& back() const {
        return data_[size_ - 1];
    }

    T*       data() {
        return data_;
    }
    
    const T* data() const {
        return data_;
    }

    // =================== Capacity ================================

    size_t size() const  {
        return size_;
    }

    bool   empty() const {
        return size_ == 0;
    }

    // =================== Operations ==============================

    void fill(const T& value) {
        std::fill(data_, data_ + size_, value);
    }

    void swap(Array& other) {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    // =================== Comparison ==============================

    bool operator==(const Array& rhs) const {
        if (this->size_ != rhs.size_) return false;

        for (size_t i = 0; i < this->size_; i++) {
            if ((*this)[i] != rhs[i]) return false;
        }

        return true;
    }

    bool operator!=(const Array& rhs) const {
        return !(*this == rhs);
    }

private:
    T*     data_ = nullptr;
    size_t size_ = 0;
};
