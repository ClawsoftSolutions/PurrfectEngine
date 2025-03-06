#ifndef   _PURRFECT_ENGINE_ECS_HPP_
#define   _PURRFECT_ENGINE_ECS_HPP_

#include "componentView.hpp"
#include "sparseSet.hpp"
#include "queue.hpp"

#include <iostream>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <typeindex>

namespace PurrfectEngine {

  using Entity = uint32_t;
  class ECS {
    using ComponentMask = uint64_t;
    static constexpr size_t maxComponentCount = 64;
  public:
    Entity create();
    void removeEntity(const Entity &entity);
  public:
    template <typename T>
    void registerComponent() {
      if (m_componentBitPositions.find(typeid(T)) != m_componentBitPositions.end()) return;
      if (m_componentBitPositions.size() >= maxComponentCount)
        throw CodeException(Code::OutOfBounds);

      m_componentBitPositions[typeid(T)] = m_componentPools.size();
      m_componentPools.append(dynamic_cast<ISparseSet<>*>(new SparseSet<T>()));
    }

    template <typename T>
    void insert(const Entity &entity, const T &component = T()) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);

      m_entityMasks[entity] |= 1 << getComponentPos<T>();
      SparseSet<T> &set = getComponentSet<T>();
      set[entity] = component;
    }

    template <typename T, typename... Args>
    T &emplace(const Entity &entity, Args &&...args) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);

      m_entityMasks[entity] |= 1 << getComponentPos<T>();
      SparseSet<T> &set = getComponentSet<T>();
      return set.emplace(entity, args...);
    }

    template <typename T>
    constexpr T &get(const Entity &entity) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);
      if (!(m_entityMasks[entity] & 1 << getComponentPos<T>())) throw CodeException(Code::OutOfBounds);

      SparseSet<T> &set = getComponentSet<T>();
      return set[entity];
    }

    template <typename T>
    constexpr const T &get(const Entity &entity) const {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);
      if (!(m_entityMasks[entity] & 1 << getComponentPos<T>())) throw CodeException(Code::OutOfBounds);

      const SparseSet<T> &set = getComponentSet<T>();
      return set[entity];
    }

    template <typename T>
    void remove(const Entity &entity) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);

      ComponentMask &mask = m_entityMasks[entity];
      ComponentMask componentMask = 1<<getComponentPos<T>();
      if (!(mask & componentMask)) return;

      mask &= ~componentMask;
      SparseSet<T> &set = getComponentSet<T>();
      set.remove(entity);
    }
  public: // Has
    template <typename ...Components>
    constexpr bool hasAll(const Entity &entity) const {
      ComponentMask componentsMask = ((1<<getComponentPos<Components>()) | ...);
      return (m_entityMasks[entity] & componentsMask) == componentsMask;
    }

    template <typename ...Components>
    constexpr bool hasAny(const Entity &entity) const {
      ComponentMask componentsMask = ((1<<getComponentPos<Components>()) | ...);
      return (m_entityMasks[entity] & componentsMask);
    }
  public:
    template <typename ...Get, typename ...Exclude>
    ComponentView<ComponentViewGet<Get...>, ComponentViewExclude<Exclude...>> view(ComponentViewExclude<Exclude...> = ComponentViewExclude{}) const {
      return { { getComponentPool<Get>()... }, { getComponentPool<Exclude>()... } };
    }
  private:
    template <typename T>
    size_t getComponentPos() const {
      auto it = m_componentBitPositions.find(typeid(T));
      if (it == m_componentBitPositions.end()) throw CodeException(Code::OutOfBounds);
      return it->second;
    }

    template <typename T>
    SparseSet<T> &getComponentSet() {
      size_t i = getComponentPos<T>();
      ISparseSet<> *vset = m_componentPools[i];
      SparseSet<T> *set = reinterpret_cast<SparseSet<T>*>(vset);
      return *set;
    }

    template <typename T>
    ISparseSet<> *getComponentPool() const {
      return m_componentPools[getComponentPos<T>()];
    }
  private:
    Queue<Entity> m_deadEntities{}; // TODO: Done
    SparseSet<ComponentMask, Entity> m_entityMasks{};
    Array<ISparseSet<>*> m_componentPools = Array<ISparseSet<>*>(8);
    Entity m_entityCounter = 0;
    std::unordered_map<std::type_index, size_t> m_componentBitPositions{}; // I am too lazy to implement a hash map myself (for now)
  };

}

#endif // _PURRFECT_ENGINE_ECS_HPP_