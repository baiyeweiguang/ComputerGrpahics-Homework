// Copyright 2024 Chengfu Zou
#ifndef GL_HOMEWORK_IMPL_HPP_
#define GL_HOMEWORK_IMPL_HPP_

#include <cstddef>
#include <utility>

namespace gl_hwk {
// 为Pimpl模式实现RAII
/**
 * @brief 类unique_ptr
 * @tparam T 类型
 */
template <typename T>
class unique_impl {
 public:
  unique_impl(std::nullptr_t dummy = nullptr) { m_p_ = nullptr; }
  unique_impl(T* p) { m_p_ = p; }

  ~unique_impl() {
    if (m_p_) delete m_p_;
  }

  auto operator->() -> T* { return m_p_; }

  auto operator*() -> T& { return *m_p_; }

  auto operator=(unique_impl&& other) -> unique_impl& {
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

/**
 * @brief 创建unique_impl对象
 * @tparam T 类型
 * @tparam Args 参数包
 * @param args 参数包
 * @return unique_impl对象
 */
template <typename T, typename... Args>
auto make_unique_impl(Args&&... args) -> unique_impl<T> {
  return unique_impl<T>(new T(std::forward<Args>(args)...));
}

}  // namespace gl_hwk
#endif