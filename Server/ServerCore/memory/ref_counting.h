#pragma once

class RefCountable {
public:
  RefCountable() : _ref_count(1) {}
  virtual ~RefCountable() = default;

  int32_t GetRefCount() const {
    return _ref_count;
  }

  int32_t AddRefCount() {
    return ++_ref_count;
  }

  int32_t ReleaseRef() {
    const int32_t ref_count = --_ref_count;

    if (ref_count == 0) {
      delete this;
    }

    return ref_count;
  }

protected:
  std::atomic<int32_t> _ref_count;
};

template <typename T>
class SharedPtr {
public:
  SharedPtr() {}

  SharedPtr(T* ptr) {
    Set(ptr);
  }

  SharedPtr(const SharedPtr& rhs) {
    Set(rhs._ptr);
  }

  SharedPtr(SharedPtr&& rhs) noexcept {
    _ptr = rhs._ptr;
    rhs._ptr = nullptr;
  }

  template <typename U>
  SharedPtr(const SharedPtr<U>& rhs) {
    Set(static_cast<T*>(rhs._ptr));
  }

  ~SharedPtr() {
    Release();
  }

public:
  SharedPtr& operator=(const SharedPtr& rhs) {
    if (_ptr != rhs._ptr) {
      Release();
      Set(rhs._ptr);
    }

    return *this;
  }

  SharedPtr& operator=(SharedPtr&& rhs) noexcept {
    Release();
    _ptr = rhs._ptr;
    rhs._ptr = nullptr;
    return *this;
  }

  bool operator==(const SharedPtr& rhs) const {
    return _ptr == rhs._ptr;
  }

  bool operator==(T* rhs) const {
    return _ptr == rhs._ptr;
  }

  bool operator!=(const SharedPtr& rhs) const {
    return _ptr != rhs._ptr;
  }

  bool operator!=(T* rhs) const {
    return _ptr != rhs._ptr;
  }

  bool operator<(const SharedPtr& rhs) const {
    return _ptr < rhs._ptr;
  }

  T* operator*() {
    return _ptr;
  }

  const T* operator*() const {
    return _ptr;
  }

  operator T*() const {
    return _ptr;
  }

  T* operator->() {
    return _ptr;
  }

  const T* operator->() const {
    return _ptr;
  }


  bool IsNull() {
    return _ptr == nullptr;
  }

private:
  inline void Set(T* ptr) {
    _ptr = ptr;

    if (ptr) {
      ptr->AddRef();
    }
  }

  inline void Release() {
    if (_ptr != nullptr) {
      _ptr->ReleaseRef();
      _ptr = nullptr;
    }
  }

private:
  T* _ptr = nullptr;
};
