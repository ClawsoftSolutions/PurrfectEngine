#ifndef   _PURRFECT_ENGINE_QUEUE_HPP_
#define   _PURRFECT_ENGINE_QUEUE_HPP_

#include "result.hpp"

namespace PurrfectEngine {

  template <typename T>
  class Queue { // Circular queue I think
  public:
    constexpr Queue()
    {}

    constexpr Queue(size_t initialCapacity)
      : m_capacity(initialCapacity)
    {
      if (!m_capacity) return;
      m_items = (T*)malloc(sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);
    }

    Queue(const Queue<T> &other)
    {
      *this = other;
    }

    Queue(Queue<T> &&other)
      : m_items(other.m_items), m_begin(other.m_begin), m_capacity(other.m_capacity), m_count(other.m_count)
    {
      other.m_items = nullptr;
      other.m_begin = 0;
      other.m_capacity = 0;
      other.m_count = 0;
    }

    Queue &operator =(const Queue<T> &other) {
      if (this == &other) return *this;
      if (m_items) free(m_items);
      m_begin = other.m_begin;
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

    ~Queue() {
      free(m_items);
    }

    void push(const T &value = T{}) {
      if (empty()) m_begin = 0;
      if (m_count >= m_capacity) reserve(m_capacity?m_capacity:16);
      const size_t &index = getIndex(m_count++);
      new (&m_items[index]) T();
      m_items[index] = value;
    }

    T pop() {
      if (empty()) throw CodeException(Code::OutOfBounds);
      --m_count;
      return m_items[m_begin++];
    }

    void clear() { m_count = 0; m_begin = 0; }

    void reserve(const size_t &count) { // If you have an idea on how to make it more tangled up and still work, make a pr or something, idk
      if (m_count+count <= m_capacity) return;
      size_t savedCapacity = m_capacity;
      m_capacity += m_count+count-m_capacity;
      T *saved = m_items;
      m_items = (T*)malloc(sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);

      const size_t &a = savedCapacity-m_begin;
      memcpy(m_items+0, saved+m_begin, sizeof(T)*a);
      memcpy(m_items+a, saved, sizeof(T)*m_begin);
      m_begin = 0;

      free(saved);
    }

    void resize(const size_t &size, const T &filler = T()) {
      m_capacity = size;
      m_items = (T*)realloc(m_items, sizeof(T) * m_capacity);
      if (!m_items) throw CodeException(Code::BuyMoreRAM);
      for (size_t i = m_count; i < m_capacity; ++i) {
        new (&m_items[getIndex(i)]) T();
        m_items[getIndex(i)] = filler; // Juz tylko filler
      }
      m_count = size;
    }

    T &operator [](const size_t &index) {
      if (index >= m_count) throw CodeException(Code::OutOfBounds);
      return m_items[getIndex(index)];
    }

    constexpr const T &operator [](const size_t &index) const {
      if (index >= m_count) throw CodeException(Code::OutOfBounds);
      return m_items[getIndex(index)];
    }

    constexpr bool empty() const { return m_count == 0; }
  private:
    constexpr size_t getIndex(size_t index) const {
      if (index < m_capacity-m_begin) return m_begin+index;
      return index-m_capacity-m_begin;
    }
  private:
    T *m_items = nullptr;
    size_t m_begin = 0;
    size_t m_capacity = 0;
    size_t m_count = 0;
  };

}

#endif // _PURRFECT_ENGINE_QUEUE_HPP_