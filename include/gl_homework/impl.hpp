// Copyright 2024 Chengfu Zou
#ifndef GL_HOMEWORK_IMPL_HPP_
#define GL_HOMEWORK_IMPL_HPP_

#include <cstddef>
#include <utility>

namespace gl_hwk {
// 为Pimpl模式实现RAII
template <typename T>
class UniqueImpl {
 public:
  UniqueImpl(std::nullptr_t dummy = nullptr) { m_p_ = nullptr; }
  UniqueImpl(T* p) { m_p_ = p; }

  ~UniqueImpl() {
    if (m_p_) delete m_p_;
  }

  auto operator->() -> T* { return m_p_; }

  auto operator*() -> T& { return *m_p_; }

  auto operator=(UniqueImpl&& other) -> UniqueImpl& {
    if (this != &other) {
      if (m_p_) delete m_p_;
      m_p_ = other.m_p_;
      other.m_p_ = nullptr;
    }
    return *this;
  }

  auto get() -> T* { return m_p_; }

 private:
  T* m_p_;
};

template <typename T, typename... Args>
auto make_unique_impl(Args&&... args) -> UniqueImpl<T> {
  return UniqueImpl<T>(new T(std::forward<Args>(args)...));
}

}  // namespace gl_hwk
#endif