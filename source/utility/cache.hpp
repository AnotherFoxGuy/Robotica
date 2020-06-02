#pragma once

#include <functional>
#include <utility>


namespace robotica {
    template <typename T, typename U> class cache {
    public:
        cache(T&& value, U&& updater, bool valid = false) :
            value(std::forward<T>(value)),
            valid(valid),
            updater(std::forward<U>(updater))
        {}


        // When constructing from another cached value, copy / move its updater, since we don't already have one.
        cache(const cache&) = default;
        cache(cache&&) = default;

        // We want cache<T> to behave like T, so T1 = T2 shouldn't copy / move the updater.
        // Note that we don't touch valid, since assigning to this object doesn't neccesarily mean we should cancel the update.
        T& operator=(const cache& other) {
            value = *other;  // Will make sure other is up-to-date.
            return **this;
        }

        T& operator=(cache&& other) {
            value = std::move(*other);  // Will make sure other is up-to-date.
            return **this;
        }


        void set_valid(bool valid) const {
            this->valid = valid;
        }


        // Note that we don't touch valid, since assigning to this object doesn't neccesarily mean we should cancel the update.
        T& operator=(const T& v) { value = v; return value; }
        T& operator=(T&& v) { value = std::forward<T>(v); return value; }

        operator       T& (void)       { update(); return value; }
        operator const T& (void) const { update(); return value; }

        T*       operator->(void)       { update(); return &value; }
        const T* operator->(void) const { update(); return &value; }
    private:
        mutable T value;
        mutable bool valid;
        U updater;


        void update(void) const {
            if (!valid) {
                // Update method may either take no arguments, or take the previous state as an argument.
                if constexpr (std::is_invocable_v<U>) value = std::invoke(updater);
                else value = std::invoke(updater, std::move(value));

                valid = true;
            }
        }
    };
}