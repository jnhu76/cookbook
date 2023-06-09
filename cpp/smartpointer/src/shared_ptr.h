#pragma once

#include <algorithm>
#include <iostream>

// error mechanism
#include <cassert>
#include <new>
#define SHARED_ASSERT(x) assert(x)


class shared_ptr_count {
public:
    shared_ptr_count(): pn(NULL) {}

    shared_ptr_count(const shared_ptr_count& count): pn(count.pn) {}

    void swap(shared_ptr_count& lhs) noexcept {
        std::swap(pn, lhs.pn);
    }

    // getter of the underlying reference counter.
    long use_count(void) const {
        long count = 0;
        if (pn != nullptr) {
            count = *pn;
        }
        return count;
    }

    // acquire/share the ownership of the pointer, initializing the reference counter.
    template<typename T>
    void acquire(T* p) {
        if (p != nullptr) {
            if (pn == nullptr) {
                try {
                    pn = new long(1);  // may throw std::bad_alloc.
                }
                catch (std::bad_alloc&) {
                    delete p;
                    throw;  // rethow the std::bad_alloc.
                }
            } else {
                (*pn)++;
            }
        }
    }

    // release the ownership of the px pointer, destroying the object when appropriate.
    template<typename T>
    void release(T* p) noexcept {
        if (pn != nullptr) {
            --(*pn);
            if (*pn == 0) {
                delete p;
                delete pn;
            }
            pn = nullptr;
        }
    }

public:
    long* pn;
};

/*
* minimal implementation of smart pointer
*/
template<typename T>
class shared_ptr {
public:
    typedef T element_type;

    shared_ptr(): px(nullptr), pn() {}

    explicit shared_ptr(T* p): pn() {
        acquire(p);
    } 

    template<typename U>
    shared_ptr(const shared_ptr<U>& ptr, T *p): pn(ptr.pn) {
        acquire(p);
    }

    //  Constructor to share ownership. Warning : to be used for pointer_cast only.
    template<typename U>
    shared_ptr(const shared_ptr<U>& ptr): pn(ptr.pn) {
        SHARED_ASSERT((nullptr == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(static_cast<typename shared_ptr<T>::element_type*>(ptr.px));
    }

    // Copy constructor (used by the copy-and-swap idiom)
    shared_ptr(const shared_ptr& ptr) noexcept: pn(ptr.pn) {
        SHARED_ASSERT((nullptr == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(ptr.px);
    }

    shared_ptr& operator=(shared_ptr ptr) noexcept {
        swap(ptr);
        return *this;
    }

    ~shared_ptr() noexcept {
        release();
    }

    // this reset releases its ownership
    void reset() noexcept {
        release();
    }

    // this reset release its ownership and re-acquire another one
    void reset(T* p) noexcept {
        SHARED_ASSERT((nullptr == p) || (px != p));
        release();
        acquire(p);
    }

    // swap method for the copy-and-swap idiom
    void swap(shared_ptr& lhs) {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    // reference counter operations.
    operator bool() const noexcept {
        return (0 < pn.use_count());
    }

    bool unique(void) const noexcept {
        return (1 == pn.use_count());
    }

    long use_count() const noexcept {
        return pn.use_count();
    }

    // underlying pointer operations.
    T& operator*() const noexcept {
        SHARED_ASSERT(nullptr != px);
        return *px;
    }

    T* operator->() const noexcept {
        SHARED_ASSERT(nullptr != px);
        return px;
    }

    T* get() const noexcept {
        return px;
    }

private:
    void acquire(T* p) {
        pn.acquire(p);
        px = p;
    }

    void release() noexcept {
        pn.release(px);
        px = nullptr;
    }

private:
    template<typename U>
    friend class shared_ptr;

private:
    T* px;
    shared_ptr_count pn;
};

// comparasion operators.
template<typename T, typename U>
bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() == r.get());
}
template<typename T, typename U>
bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() != r.get());
}
template<typename T, typename U>
bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() <= r.get());
}
template<typename T, typename U>
bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() >= r.get());
}
template<typename T, typename U>
bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() < r.get());
}
template<typename T, typename U>
bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) noexcept {
    return (l.get() > r.get());
}

// static_case of shared_ptr
template<typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) noexcept {
    return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of shared_ptr
template<typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) noexcept {
    T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
    if (p != nullptr) {
        return shared_ptr<T>(ptr, p);
    } else {
        return shared_ptr<T>();
    }
}