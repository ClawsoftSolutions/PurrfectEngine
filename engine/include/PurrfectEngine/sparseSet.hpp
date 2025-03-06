#ifndef   _PURRFECT_ENGINE_SPARSE_SET_HPP_
#define   _PURRFECT_ENGINE_SPARSE_SET_HPP_

#include "array.hpp"

namespace PurrfectEngine {

  template <typename Index = uint32_t>
  class ISparseSet {
  public:
    static constexpr Index tombstone = UINT32_MAX;
  public:
    virtual void remove(const Index &index) = 0;
    virtual void clear() = 0;
    virtual bool contains(const Index &index) const = 0;
    virtual constexpr Index size() const = 0;
    virtual constexpr const Array<Index> &getSparse() const = 0;
  };

  template <typename T, typename Index = uint32_t>
  class SparseSet : public ISparseSet<Index> {
  public:
    static constexpr Index tombstone = UINT32_MAX;
  public:
    constexpr SparseSet()
    {}

    SparseSet(const SparseSet &other)
    {
      *this = other;
    }

    SparseSet(SparseSet &&other)
      : m_sparse(other.m_sparse), m_dense(other.m_dense)
    {
      other.m_sparse.~Array();
      other.m_dense.~Array();
    }

    SparseSet &operator =(const SparseSet &other) {
      if (this == &other) return *this;

      new (&m_sparse) Array<Index>();
      new (&m_dense) Array<T>();

      m_sparse = other.m_sparse;
      m_dense = other.m_dense;

      return *this;
    }

    void insert(const Index &index, const T &value) {
      if (index >= m_sparse.size()) m_sparse.resize(index+1, tombstone);
      if (m_sparse[index] != tombstone) remove(index);
      m_sparse[index] = m_dense.size();
      m_dense.append(value);
      m_denseToSparse.append(index);
    }

    template <typename... Args>
    T &emplace(const Index &index, Args&&... args) {
      if (index >= m_sparse.size()) m_sparse.resize(index+1, tombstone);
      if (m_sparse[index] != tombstone) remove(index);
      m_sparse[index] = m_dense.size();
      m_denseToSparse.append(index);
      return m_dense.emplace(args...);
    }

    void remove(const Index &index) {
      if (index >= m_sparse.size()) return;
      Index denseIndex = m_sparse[index];
      if (denseIndex == tombstone) return;
      m_sparse[index] = tombstone;
      m_sparse[m_denseToSparse.back()] = denseIndex;

      m_dense[denseIndex] = m_dense.back();
      m_denseToSparse[denseIndex] = m_denseToSparse.back();
    }

    void clear() {
      m_sparse.clear();
      m_dense.clear();
    }

    T &get(const Index &index) {
      return (*this)[index];
    }

    T &operator [](const Index &index) {
      if (index >= m_sparse.size()) m_sparse.resize(index+1, tombstone);
      const Index &denseIndex = m_sparse[index];
      if (denseIndex == tombstone) {
        m_sparse[index] = m_dense.size();
        m_dense.append();
        return m_dense.back();
      }

      return m_dense[denseIndex];
    }

    const T &operator [](const Index &index) const {
      if (index >= m_sparse.size()) throw CodeException(Code::OutOfBounds);
      const Index &denseIndex = m_sparse[index];
      if (denseIndex == tombstone) throw CodeException(Code::OutOfBounds);
      return m_dense[denseIndex];
    }

    bool contains(const Index &index) const {
      return m_sparse.size() > index && m_sparse[index] != tombstone;
    }

    constexpr Index size() const { return m_dense.size(); }
    constexpr const Array<Index> &getSparse() const { return m_denseToSparse; }
  private:
    Array<Index> m_sparse{};
    Array<Index> m_denseToSparse{};
    Array<T> m_dense{};
  };

}

#endif // _PURRFECT_ENGINE_SPARSE_SET_HPP_