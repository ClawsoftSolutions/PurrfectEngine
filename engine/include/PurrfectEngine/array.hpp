#ifndef   _PURRFECT_ENGINE_ARRAY_HPP_
#define   _PURRFECT_ENGINE_ARRAY_HPP_

#include "core.hpp"
#include "result.hpp"

#include <ostream>

namespace PurrfectEngine {

  template <typename T>
  class Array {
  public:
    constexpr Array()
    {}

    constexpr Array(size_t initialCapacity)
      : m_capacity(initialCapacity)
    {
      if (!m_capacity) return;
      m_items = (T*)malloc(sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);
    }

    // constexpr Array(size_t count, const T &filler = T{}) // Juz tylko filler
    //   : m_capacity(count), m_count(count)
    // {
    //   if (!m_capacity) return;
    //   m_items = (T*)malloc(sizeof(T) * m_capacity);
    //   if (!m_items) throw CodeException(Code::BuyMoreRAM);
    //   for (size_t i = 0; i < count; ++i) {
    //     new (&m_items[i]) T();
    //     m_items[i] = filler;
    //   }
    // }

    Array(const Array<T> &other)
    {
      *this = other;
    }

    constexpr Array(Array<T> &&other)
      : m_items(other.m_items), m_capacity(other.m_capacity), m_count(other.m_count)
    {
      other.m_items = nullptr;
      other.m_capacity = 0;
      other.m_count = 0;
    }

    Array<T> &operator =(const Array<T> &other) {
      if (this == &other) return *this;
      if (m_items) free(m_items);
      m_count = other.m_count;
      m_capacity = other.m_capacity;

      if (!m_capacity) return *this;
      m_items = (T*)malloc(sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);

      for (size_t i = 0; i < m_count; ++i) {
        new (&m_items[i]) T();
        m_items[i] = other.m_items[i];
      }

      return *this;
    }

    void append(const T &value = T{}) {
      if (m_count >= m_capacity) reserve(m_capacity?m_capacity:16); // Double the capacity
      new (&m_items[m_count]) T();
      m_items[m_count++] = value;
    }

    template <typename... Args>
    T &emplace(Args &&...args) {
      if (m_count >= m_capacity) reserve(m_capacity?m_capacity:16); // Double the capacity
      new (&m_items[m_count]) T(args...);
      return m_items[m_count++];
    }

    void clear() { m_count = 0; }

    void remove(size_t index) {
      if (index > --m_count) throw CodeException(Code::OutOfBounds);
      while (index <= m_count) m_items[index] = m_items[++index];
    }

    void remove(size_t begin, size_t count) {
      if (!count) return;
      if (begin >= m_count) throw CodeException(Code::OutOfBounds);
      m_count -= count;
      for (size_t i = 0; i < count; ++i)
        m_items[begin] = m_items[count+(begin++)];
    }

    // Perfect if you don't care about the order (not recommended by dictators)
    void fastRemove(const size_t &index) {
      if (index >= --m_count) throw CodeException(Code::OutOfBounds);
      m_items[index] = m_items[m_count];
    }

    void reserve(const size_t &count) {
      if (!count) return;
      m_capacity += count;
      m_items = (T*)realloc(m_items, sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);
    }

    void resize(const size_t &size, const T &filler = T()) {
      m_capacity = size;
      m_items = (T*)realloc(m_items, sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);
      for (size_t i = m_count; i < m_capacity; ++i) {
        new (&m_items[i]) T();
        m_items[i] = filler; // Padding probably would fill better but I couldn't pass on the polish song reference
      }
      m_count = size;
    }

    T &operator [](const size_t &index) {
      if (index >= m_count) throw CodeException(Code::OutOfBounds);
      return m_items[index];
    }

    constexpr const T &operator [](const size_t &index) const {
      if (index >= m_count) throw CodeException(Code::OutOfBounds);
      return m_items[index];
    }

    constexpr bool has(const T &value) const {
      for (size_t i = 0; i < m_count; ++i)
        if (m_items[i] == value) return true;
      return false;
    }

    constexpr T &front() const {
      if (!m_count) throw CodeException(Code::OutOfBounds);
      return m_items[0];
    }

    constexpr T &back() const {
      if (!m_count) throw CodeException(Code::OutOfBounds);
      return m_items[m_count-1];
    }

    constexpr size_t size() const { return m_count; }
    constexpr size_t capacity() const { return m_capacity; }
  private:
    T *m_items = nullptr;
    size_t m_capacity = 0;
    size_t m_count = 0;
  };

}

#endif // _PURRFECT_ENGINE_ARRAY_HPP_