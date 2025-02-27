// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_SCOPED_REFPTR_H_
#define ACF_INCLUDE_INTERNAL_ACF_SCOPED_REFPTR_H_
#pragma once

#include <stddef.h>

#include <atomic>
#include <iosfwd>
#include <memory>
#include <type_traits>
#include <utility>

#if defined(USING_CHROMIUM_INCLUDES)
// When building CEF include the Chromium header directly.
#include "base/memory/scoped_refptr.h"
#else  // !USING_CHROMIUM_INCLUDES

template <class T>
class scoped_refptr;

// Takes an instance of T, which is a ref counted type, and wraps the object
// into a scoped_refptr<T>.
template <typename T>
scoped_refptr<T> WrapRefCounted(T* t) {
  return scoped_refptr<T>(t);
}

///
/// A smart pointer class for reference counted objects.  Use this class instead
/// of calling AddRef and Release manually on a reference counted object to
/// avoid common memory leaks caused by forgetting to Release an object
/// reference. Sample usage:
///
/// <pre>
///   class MyFoo : public RefCounted<MyFoo> {
///    ...
///    private:
///     friend class RefCounted<MyFoo>;  // Allow destruction by RefCounted<>.
///     ~MyFoo();                        // Destructor must be
///     private/protected.
///   };
///
///   void some_function() {
///     scoped_refptr<MyFoo> foo = MakeRefCounted<MyFoo>();
///     foo->Method(param);
///     // |foo| is released when this function returns
///   }
///
///   void some_other_function() {
///     scoped_refptr<MyFoo> foo = MakeRefCounted<MyFoo>();
///     ...
///     foo.reset();  // explicitly releases |foo|
///     ...
///     if (foo)
///       foo->Method(param);
///   }
/// </pre>
///
/// The above examples show how scoped_refptr<T> acts like a pointer to T.
/// Given two scoped_refptr<T> classes, it is also possible to exchange
/// references between the two objects, like so:
///
/// <pre>
///   {
///     scoped_refptr<MyFoo> a = MakeRefCounted<MyFoo>();
///     scoped_refptr<MyFoo> b;
///
///     b.swap(a);
///     // now, |b| references the MyFoo object, and |a| references nullptr.
///   }
/// </pre>
///
/// To make both |a| and |b| in the above example reference the same MyFoo
/// object, simply use the assignment operator:
///
/// <pre>
///   {
///     scoped_refptr<MyFoo> a = MakeRefCounted<MyFoo>();
///     scoped_refptr<MyFoo> b;
///
///     b = a;
///     // now, |a| and |b| each own a reference to the same MyFoo object.
///   }
/// </pre>
///
/// Also see Chromium's ownership and calling conventions:
/// https://chromium.googlesource.com/chromium/src/+/lkgr/styleguide/c++/c++.md#object-ownership-and-calling-conventions
/// Specifically:
///   If the function (at least sometimes) takes a ref on a refcounted object,
///   declare the param as scoped_refptr<T>. The caller can decide whether
///   it wishes to transfer ownership (by calling std::move(t) when passing t)
///   or retain its ref (by simply passing t directly). In other words, use
///   scoped_refptr like you would a std::unique_ptr except in the odd case
///   where it's required to hold on to a ref while handing one to another
///   component (if a component merely needs to use t on the stack without
///   keeping a ref: pass t as a raw T*).
///
template <class T>
class scoped_refptr {
 public:
  typedef T element_type;

  constexpr scoped_refptr() = default;

  // Allow implicit construction from nullptr.
  constexpr scoped_refptr(std::nullptr_t) {}

  // Constructs from a raw pointer. Note that this constructor allows implicit
  // conversion from T* to scoped_refptr<T> which is strongly discouraged.
  // If you are creating a new ref-counted object please use
  // base::MakeRefCounted<T>() or base::WrapRefCounted<T>(). Otherwise you
  // should move or copy construct from an existing scoped_refptr<T> to the
  // ref-counted object.
  scoped_refptr(T* p) : ptr_(p) {
    if (ptr_) {
      AddRef(ptr_);
    }
  }

  // Copy constructor. This is required in addition to the copy conversion
  // constructor below.
  scoped_refptr(const scoped_refptr& r) : scoped_refptr(r.ptr_) {}

  // Copy conversion constructor.
  template <typename U,
            typename = typename std::enable_if<
                std::is_convertible<U*, T*>::value>::type>
  scoped_refptr(const scoped_refptr<U>& r) : scoped_refptr(r.ptr_) {}

  // Move constructor. This is required in addition to the move conversion
  // constructor below.
  scoped_refptr(scoped_refptr&& r) noexcept : ptr_(r.ptr_) { r.ptr_ = nullptr; }

  // Move conversion constructor.
  template <typename U,
            typename = typename std::enable_if<
                std::is_convertible<U*, T*>::value>::type>
  scoped_refptr(scoped_refptr<U>&& r) noexcept : ptr_(r.ptr_) {
    r.ptr_ = nullptr;
  }

  ~scoped_refptr() {
    if (ptr_) {
      Release(ptr_);
    }
  }

  T* get() const { return ptr_; }

  T& operator*() const { return *ptr_; }

  T* operator->() const { return ptr_; }

  scoped_refptr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  scoped_refptr& operator=(T* p) { return *this = scoped_refptr(p); }

  // Unified assignment operator.
  scoped_refptr& operator=(scoped_refptr r) noexcept {
    swap(r);
    return *this;
  }

  // Sets managed object to null and releases reference to the previous managed
  // object, if it existed.
  void reset() { scoped_refptr().swap(*this); }

  // Returns the owned pointer (if any), releasing ownership to the caller. The
  // caller is responsible for managing the lifetime of the reference.
  T* release();

  void swap(scoped_refptr& r) noexcept { std::swap(ptr_, r.ptr_); }

  explicit operator bool() const { return ptr_ != nullptr; }

  template <typename U>
  bool operator==(const scoped_refptr<U>& rhs) const {
    return ptr_ == rhs.get();
  }

  template <typename U>
  bool operator!=(const scoped_refptr<U>& rhs) const {
    return !operator==(rhs);
  }

  template <typename U>
  bool operator<(const scoped_refptr<U>& rhs) const {
    return ptr_ < rhs.get();
  }

 protected:
  T* ptr_ = nullptr;

 private:
  // Friend required for move constructors that set r.ptr_ to null.
  template <typename U>
  friend class scoped_refptr;

  // Non-inline helpers to allow:
  //     class Opaque;
  //     extern template class scoped_refptr<Opaque>;
  // Otherwise the compiler will complain that Opaque is an incomplete type.
  static void AddRef(T* ptr);
  static void Release(T* ptr);
};

template <typename T>
T* scoped_refptr<T>::release() {
  T* ptr = ptr_;
  ptr_ = nullptr;
  return ptr;
}

// static
template <typename T>
void scoped_refptr<T>::AddRef(T* ptr) {
  ptr->AddRef();
}

// static
template <typename T>
void scoped_refptr<T>::Release(T* ptr) {
  ptr->Release();
}

template <typename T, typename U>
bool operator==(const scoped_refptr<T>& lhs, const U* rhs) {
  return lhs.get() == rhs;
}

template <typename T, typename U>
bool operator==(const T* lhs, const scoped_refptr<U>& rhs) {
  return lhs == rhs.get();
}

template <typename T>
bool operator==(const scoped_refptr<T>& lhs, std::nullptr_t null) {
  return !static_cast<bool>(lhs);
}

template <typename T>
bool operator==(std::nullptr_t null, const scoped_refptr<T>& rhs) {
  return !static_cast<bool>(rhs);
}

template <typename T, typename U>
bool operator!=(const scoped_refptr<T>& lhs, const U* rhs) {
  return !operator==(lhs, rhs);
}

template <typename T, typename U>
bool operator!=(const T* lhs, const scoped_refptr<U>& rhs) {
  return !operator==(lhs, rhs);
}

template <typename T>
bool operator!=(const scoped_refptr<T>& lhs, std::nullptr_t null) {
  return !operator==(lhs, null);
}

template <typename T>
bool operator!=(std::nullptr_t null, const scoped_refptr<T>& rhs) {
  return !operator==(null, rhs);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const scoped_refptr<T>& p) {
  return out << p.get();
}

template <typename T>
void swap(scoped_refptr<T>& lhs, scoped_refptr<T>& rhs) noexcept {
  lhs.swap(rhs);
}

#endif  // !USING_CHROMIUM_INCLUDES

///
/// All ref-counted framework classes must extend this class.
///
class AcfBaseRefCounted {
 public:
  ///
  /// Called to increment the reference count for the object. Should be called
  /// for every new copy of a pointer to a given object.
  ///
  virtual void AddRef() const = 0;

  ///
  /// Called to decrement the reference count for the object. Returns true if
  /// the reference count is 0, in which case the object should self-delete.
  ///
  virtual bool Release() const = 0;

  ///
  /// Returns true if the reference count is 1.
  ///
  virtual bool HasOneRef() const = 0;

  ///
  /// Returns true if the reference count is at least 1.
  ///
  virtual bool HasAtLeastOneRef() const = 0;

 protected:
  virtual ~AcfBaseRefCounted() {}
};

///
/// All scoped framework classes must extend this class.
///
class AcfBaseScoped {
 public:
  virtual ~AcfBaseScoped() {}
};

class AcfAtomicRefCount {
 public:
  constexpr AcfAtomicRefCount() : ref_count_(0) {}
  explicit constexpr AcfAtomicRefCount(int initial_value)
      : ref_count_(initial_value) {}

  ///
  /// Increment a reference count.
  /// Returns the previous value of the count.
  ///
  int Increment() { return Increment(1); }

  ///
  /// Increment a reference count by "increment", which must exceed 0.
  /// Returns the previous value of the count.
  ///
  int Increment(int increment) {
    return ref_count_.fetch_add(increment, std::memory_order_relaxed);
  }

  ///
  /// Decrement a reference count, and return whether the result is non-zero.
  /// Insert barriers to ensure that state written before the reference count
  /// became zero will be visible to a thread that has just made the count zero.
  ///
  bool Decrement() {
    // TODO(jbroman): Technically this doesn't need to be an acquire operation
    // unless the result is 1 (i.e., the ref count did indeed reach zero).
    // However, there are toolchain issues that make that not work as well at
    // present (notably TSAN doesn't like it).
    return ref_count_.fetch_sub(1, std::memory_order_acq_rel) != 1;
  }

  ///
  /// Return whether the reference count is one.  If the reference count is used
  /// in the conventional way, a refrerence count of 1 implies that the current
  /// thread owns the reference and no other thread shares it.  This call
  /// performs the test for a reference count of one, and performs the memory
  /// barrier needed for the owning thread to act on the object, knowing that it
  /// has exclusive access to the object.
  ///
  bool IsOne() const { return ref_count_.load(std::memory_order_acquire) == 1; }

  ///
  /// Return whether the reference count is zero.  With conventional object
  /// referencing counting, the object will be destroyed, so the reference count
  /// should never be zero.  Hence this is generally used for a debug check.
  ///
  bool IsZero() const {
    return ref_count_.load(std::memory_order_acquire) == 0;
  }

  ///
  /// Returns the current reference count (with no barriers). This is subtle,
  /// and should be used only for debugging.
  ///
  int SubtleRefCountForDebug() const {
    return ref_count_.load(std::memory_order_relaxed);
  }

 private:
  std::atomic_int ref_count_;
};

///
/// Class that implements atomic reference counting.
///
class AcfRefCount {
 public:
  AcfRefCount() = default;

  AcfRefCount(const AcfRefCount&) = delete;
  AcfRefCount& operator=(const AcfRefCount&) = delete;

  ///
  /// Increment the reference count.
  ///
  void AddRef() const { ref_count_.Increment(); }

  ///
  /// Decrement the reference count. Returns true if the reference count is 0.
  ///
  bool Release() const { return !ref_count_.Decrement(); }

  ///
  /// Returns true if the reference count is 1.
  ///
  bool HasOneRef() const { return ref_count_.IsOne(); }

  ///
  /// Returns true if the reference count is at least 1.
  ///
  bool HasAtLeastOneRef() const { return !ref_count_.IsZero(); }

 private:
  mutable AcfAtomicRefCount ref_count_{0};
};

///
/// Macro that provides a reference counting implementation for classes
/// extending CefBase.
///
#define IMPLEMENT_REFCOUNTING(ClassName)          \
 public:                                          \
  void AddRef() const override {                  \
    ref_count_.AddRef();                          \
  }                                               \
  bool Release() const override {                 \
    if (ref_count_.Release()) {                   \
      delete static_cast<const ClassName*>(this); \
      return true;                                \
    }                                             \
    return false;                                 \
  }                                               \
  bool HasOneRef() const override {               \
    return ref_count_.HasOneRef();                \
  }                                               \
  bool HasAtLeastOneRef() const override {        \
    return ref_count_.HasAtLeastOneRef();         \
  }                                               \
                                                  \
 private:                                         \
  AcfRefCount ref_count_

///
/// Define refcount implement
///
template <class T>
using AcfRefPtr = scoped_refptr<T>;

///
/// A CefOwnPtr<T> is like a T*, except that the destructor of CefOwnPtr<T>
/// automatically deletes the pointer it holds (if any). That is, CefOwnPtr<T>
/// owns the T object that it points to. Like a T*, a CefOwnPtr<T> may hold
/// either NULL or a pointer to a T object. Also like T*, CefOwnPtr<T> is
/// thread-compatible, and once you dereference it, you get the thread safety
/// guarantees of T.
///
template <class T, class D = std::default_delete<T>>
using AcfOwnPtr = std::unique_ptr<T, D>;

///
/// A CefRawPtr<T> is the same as T*
///
template <class T>
using AcfRawPtr = T*;

#endif  //! ACF_INTERNAL_ACF_SCOPED_REFPTR_H_
