#pragma once

#include <cstddef>
#include <memory>
#include <utility>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    // ======================== Constructors ========================

    UniquePtr() = default;

    explicit UniquePtr(T* ptr): ptr_(ptr) {}

    UniquePtr(T* ptr, const Deleter& deleter): ptr_(ptr), deleter_(deleter) {}

    UniquePtr(T* ptr, Deleter&& deleter): ptr_(ptr), deleter_(deleter) {}

    // =================== No copy ==================================

    UniquePtr(const UniquePtr&)            = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // =================== Move ====================================

    UniquePtr(UniquePtr&& other) {
        ptr_ = other.ptr_;
        deleter_ = other.deleter_;
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (ptr_ != nullptr) {
            deleter_(ptr_);
        }
        ptr_ = other.ptr_;
        deleter_ = other.deleter_;
        other.ptr_ = nullptr;
        return *this;
    }

    // =================== Destructor ==============================

    ~UniquePtr() {
        if (ptr_ != nullptr) {
            deleter_(ptr_);
        }
    }

    // =================== Observers ===============================

    T*       get() {
        return ptr_;
    }

    const T* get() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T&       operator*() {
        return *ptr_;
    }

    const T& operator*() const {
        return *ptr_;
    }

    T*       operator->() {
        return ptr_;
    }

    const T* operator->() const {
        return ptr_;
    }

    Deleter&       get_deleter() {
        return deleter_;    
    }
    
    const Deleter& get_deleter() const {
        return deleter_;
    }

    // =================== Modifiers ===============================

    T*   release() {
        T* copy_ptr = ptr_;
        ptr_ = nullptr;
        return copy_ptr;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != nullptr) {
            deleter_(ptr_);
        }
        ptr_ = ptr;
    }

    void swap(UniquePtr& other) {
        std::swap(ptr_, other.ptr_);
    }

private:
    T*      ptr_ = nullptr;
    [[no_unique_address]] Deleter deleter_;
};

// =====================================================================
//  Partial specialization for arrays: UniquePtr<T[]>
//  Uses operator[] instead of operator*/operator->.
//  Default deleter is std::default_delete<T[]> (calls delete[]).
// =====================================================================

template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    // ======================== Constructors ========================

    UniquePtr() = default;

    explicit UniquePtr(T* ptr): ptr_(ptr) {}

    UniquePtr(T* ptr, const Deleter& deleter): ptr_(ptr), deleter_(deleter) {}

    UniquePtr(T* ptr, Deleter&& deleter): ptr_(ptr), deleter_(deleter) {}

    // =================== No copy ==================================

    UniquePtr(const UniquePtr&)            = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // =================== Move ====================================

    UniquePtr(UniquePtr&& other) {
        ptr_ = other.ptr_;
        deleter_ = other.deleter_;
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (ptr_ != nullptr) {
            deleter_(ptr_);
        }
        ptr_ = other.ptr_;
        deleter_ = other.deleter_;
        other.ptr_ = nullptr;
        return *this;
    }

    // =================== Destructor ==============================

    ~UniquePtr() {
        deleter_(ptr_);
    }

    // =================== Observers ===============================

    T*       get() {
        return ptr_;
    }

    const T* get() const {
        return ptr_; 
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T&       operator[](size_t index) {
        return ptr_[index];
    }

    const T& operator[](size_t index) const {
        return ptr_[index];
    }

    Deleter&       get_deleter() {
        return deleter_;    
    }

    const Deleter& get_deleter() const {
        return deleter_;    
    }

    // =================== Modifiers ===============================

    T*   release() {
        T* copy_ptr = ptr_;
        ptr_ = nullptr;
        return copy_ptr;
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != nullptr) {
            deleter_(ptr_);
        }
        ptr_ = ptr;
    }

    void swap(UniquePtr& other) {
        std::swap(ptr_, other.ptr_);
    }

private:
    T*      ptr_ = nullptr;
    [[no_unique_address]] Deleter deleter_;
};

// =================== Free function ===============================

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    T* obj = new T(std::forward<Args>(args)...);
    return UniquePtr<T>(obj);
}

