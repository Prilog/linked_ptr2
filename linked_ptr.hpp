//
// Created by Семен Трапезников on 02/03/2019.
//

#ifndef LINKED_PTR2_LINKED_PTR_HPP
#define LINKED_PTR2_LINKED_PTR_HPP

#include <cstdio>
#include <algorithm>

namespace smart_ptr {

    struct linked_ptr_cycle {
        mutable linked_ptr_cycle *next;
        mutable linked_ptr_cycle *prev;
    };

    template<class T>
    class linked_ptr : linked_ptr_cycle {
    private:
        T *value;

        template<typename U> friend
        class linked_ptr;

        template<typename U>
        void link(const linked_ptr<U> &arg) {
            this->prev = arg.prev;
            arg.prev = this;
            this->next = const_cast<linked_ptr<U> *>(&arg);
            this->prev->next = this;
        }

    public:
        linked_ptr() {
            prev = this;
            next = this;
            value = nullptr;
        }

        template<typename U>
        explicit linked_ptr(U *arg) {
            prev = this;
            next = this;
            value = static_cast<T*>(arg);
        }

        linked_ptr(const linked_ptr<T> &arg) {
            value = arg.value;
            prev = this;
            next = this;
            if (value) {
                link(arg);
            }
        }

        template<typename U>
        explicit linked_ptr(const linked_ptr<U> &arg) {
            value = const_cast<linked_ptr<U>*>(&arg)->value;
            prev = this;
            next = this;
            if (value) {
                link(arg);
            }
        }

        ~linked_ptr() {
            reset();
        }

        linked_ptr& operator=(const linked_ptr &arg) {
            linked_ptr x = arg;
            swap(x, *this);
            return *this;
        }

        template<typename U>
        linked_ptr& operator=(const linked_ptr<U> &arg) {
            linked_ptr x = arg;
            swap(x, *this);
            return *this;
        }

        template<typename U>
        linked_ptr &operator=(U *arg) {
            reset(arg);
            return *this;
        }

        void reset() {
            reset(static_cast<T *>(nullptr));
        }

        template<typename U>
        void reset(U *arg) {
            if (arg == value) {
                return;
            }
            if (unique()) {
                delete value;
            } else {
                next->prev = prev;
                prev->next = next;
                prev = this;
                next = this;
            }
            value = arg;
        }

        bool unique() const {
            return prev == static_cast<const linked_ptr_cycle *>(this);
        }

        template <typename U>
        friend void swap(linked_ptr<U>& a, linked_ptr<U>& b) noexcept;

        template<typename U>
        void swap(linked_ptr<U> &arg) {
            if (value == arg.value) {
                return;
            }
            std::swap(value, arg.value);
            if (unique()) {
                if (!arg.unique()) {
                    this->next = arg.next;
                    this->prev = arg.prev;
                    arg.next->prev = this;
                    arg.prev->next = this;
                    arg.next = &arg;
                    arg.prev = &arg;
                }
            } else {
                if (arg.unique()) {
                    arg.next = this->next;
                    arg.prev = this->prev;
                    this->next->prev = &arg;
                    this->prev->next = &arg;
                    this->next = this;
                    this->prev = this;
                } else {
                    std::swap(arg.next->prev, this->next->prev);
                    std::swap(arg.prev->next, this->prev->next);
                    std::swap(arg.prev, this->prev);
                    std::swap(arg.next, this->next);
                }
            }
        }

        T &operator*() const {
            return *value;
        }

        T *operator->() const {
            return value;
        }

        T *get() const {
            return value;
        }

        explicit operator bool() const {
            return value != nullptr;
        }

        bool operator!() {
            return (value == nullptr);
        }
    };

    template <typename U>
    void swap(linked_ptr<U>& a, linked_ptr<U>& b) noexcept {
        a.swap(b);
    }

    template<typename T, typename U>
    inline bool operator==(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() == b.get());
    }

    template<typename T, typename U>
    inline bool operator!=(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() != b.get());
    }

    template<typename T, typename U>
    inline bool operator<(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() < b.get());
    }

    template<typename T, typename U>
    inline bool operator<=(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() <= b.get());
    }

    template<typename T, typename U>
    inline bool operator>(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() > b.get());
    }

    template<typename T, typename U>
    inline bool operator>=(const linked_ptr<T> &a, const linked_ptr<U> &b) {
        return (a.get() >= b.get());
    }

    template<typename U>
    linked_ptr<U> make_linked(U *ptr) {
        return linked_ptr<U>(ptr);
    }

}

#endif //LINKED_PTR2_LINKED_PTR_HPP
