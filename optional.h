#pragma once

#include <new>
#include <type_traits>
#include <utility>
#include <algorithm>

template <typename T>
class Optional {
public:
    // ======================== Constructors ========================

    Optional() = default;

    Optional(const T& value) {
        new (ptr()) T(value);
        has_value_ = true;
    }

    Optional(T&& value) {
        new (ptr()) T(value);
        has_value_ = true;
    }

    // =================== Copy and Move ===========================

    Optional(const Optional& other) {
        if (other.has_value_) {
            new (ptr()) T(*other.ptr());
            has_value_ = true; 
        }
    }

    Optional(Optional&& other) {
        if (other.has_value_) {
            new (ptr()) T{std::move(*other.ptr())};
            has_value_ = true;
            other.ptr()->~T();
            other.has_value_ = false;
        }
    }

    Optional& operator=(const Optional& other) {
        if (this == &other) return *this;
        
        if (other.has_value_) {
            if (has_value_) {
                *ptr() = *other.ptr();
            } else {
                new (ptr()) T(*other.ptr());
                has_value_ = true; 
            }
            
        } else {
            reset();
        }

        return *this;
    }

    Optional& operator=(Optional&& other) {
        if (this == &other) return *this;
        
        if (other.has_value_) {
            if (has_value_) {
                *ptr() = std::move(*other.ptr());
            } else {
                new (ptr()) T{std::move(*other.ptr())};
                has_value_ = true;
            }
            other.ptr()->~T();
            other.has_value_ = false;
        } else {
            reset();
        }

        return *this;
    }

    Optional& operator=(const T& value) {
        if (has_value_) {
            *ptr() = value;
        } else {
            new (ptr()) T(value);
            has_value_ = true; 
        }
        
        return *this;
    }

    Optional& operator=(T&& value) {
        if (has_value_) {
            *ptr() = std::move(value);
        } else {
            new (ptr()) T{std::move(value)};
            has_value_ = true; 
        }
        
        return *this;
    }

    // =================== Destructor ==============================

    ~Optional() {
        reset();
    }

    // =================== Observers ===============================

    bool has_value() const {
        return has_value_;
    }

    explicit operator bool() const {
        return has_value_;
    }

    T&       value() {
        return *ptr();
    }
    
    const T& value() const {
        return *ptr();
    }

    T        value_or(const T& default_value) const {
        return has_value_ ? *ptr() : default_value;
    }

    T&       operator*() {
        return *ptr();
    }

    const T& operator*() const {
        return *ptr();
    }

    T*       operator->() {
        return ptr();
    }

    const T* operator->() const {
        return ptr();
    }

    // =================== Modifiers ===============================

    template <typename... Args>
    T& emplace(Args&&... args) {
        reset();

        new (ptr()) T(std::forward<Args>(args)...);

        has_value_ = true;
        return *ptr();
    }

    void reset() {
        if (has_value_) {
            ptr()->~T();
            has_value_ = false;
        }
    }

    void swap(Optional& other) {
        if (has_value_ && other.has_value_) {
            std::swap(*ptr(), *other.ptr());
        }
        else if (has_value_) {
            other.has_value_ = true;
            new (other.ptr()) T(std::move(*ptr()));
            ptr()->~T();
            has_value_ = false;
        }
        else if (other.has_value_) {
            has_value_ = true;
            new (ptr()) T(std::move(*(other.ptr())));
            other.ptr()->~T();
            other.has_value_ = false;
        }
    }

    // =================== Comparison ==============================

    bool operator==(const Optional& rhs) const {
        if (this->has_value_ != rhs.has_value_) return false;
        if (*ptr() != *(rhs.ptr())) return false;
        return true;
    }

    bool operator!=(const Optional& rhs) const {
        return !(*this == rhs);
    }

private:

    T *ptr() {
        return reinterpret_cast<T*>(&storage_);
    }

    const T *ptr() const {
        return reinterpret_cast<const T*>(&storage_);
    }

    alignas(T) unsigned char storage_[sizeof(T)];
    bool has_value_ = false;
};
