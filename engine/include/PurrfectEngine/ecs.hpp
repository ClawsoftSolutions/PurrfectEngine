#ifndef   _PURRFECT_ENGINE_ECS_HPP_
#define   _PURRFECT_ENGINE_ECS_HPP_

#include "sparseSet.hpp"

#include <iostream>
#include <functional>
#include <unordered_map>
#include <typeindex>

namespace PurrfectEngine {

  using Entity = uint32_t;

  template <typename ...Components>
  class ComponentView {
    friend class ECS;
  public:
    void forEach(std::function<void(Entity, Components &...)> func) {
      eachImpl(func);
    }

    void forEach(std::function<void(Components &...)> func) {
      eachImpl(func);
    }
  private:
    ComponentView(std::array<ISparseSet<>*, sizeof...(Components)> pools)
      : m_viewPools(pools)
    {
      assert(sizeof...(Components) == m_viewPools.size() && "Size mismatch");

      auto smallestPool = std::min_element(m_viewPools.begin(), m_viewPools.end(), [](ISparseSet<> *a, ISparseSet<> *b){
        return a->size() < b->size();
      });

      assert(smallestPool != m_viewPools.end() && "Invalid pools");

      m_smallest = *smallestPool;
    }
  private:
    template <size_t Index>
    auto getPool() {
      return static_cast<SparseSet<std::tuple_element_t<Index, ComponentTypes>>*>(m_viewPools[Index]);
    }

    template <size_t ...Indices>
    auto makeComponentTuple(Entity entity, std::index_sequence<Indices...>) {
      return std::make_tuple((std::ref(getPool<Indices>()->get(entity)))...);
    }

    bool isComplete(const Entity &entity) {
      return std::all_of(m_viewPools.begin(), m_viewPools.end(), [entity](ISparseSet<> *set){
        return set->contains(entity);
      });
    }

    template <typename Func>
    void eachImpl(Func func) {
      constexpr auto inds = std::make_index_sequence<sizeof...(Components)>{};

      const Array<Entity> &sparse = m_smallest->getSparse();
      for (size_t i = 0; i < sparse.size(); ++i) {
        const Entity &entity = sparse[i];
        if (entity == m_smallest->tombstone || !isComplete(entity)) continue;

        if constexpr (std::is_invocable_v<Func, Entity, Components &...>) {
          std::apply(func, std::tuple_cat(std::make_tuple(entity), makeComponentTuple(entity, inds)));
        } else if constexpr (std::is_invocable_v<Func, Components &...>) {
          std::apply(func, makeComponentTuple(entity, inds));
        } else assert(0 && "Bad lambda");
      }
    }
  private:
    using ComponentTypes = std::tuple<Components...>;

    std::array<ISparseSet<>*, sizeof...(Components)> m_viewPools;
    ISparseSet<> *m_smallest = nullptr;
  };

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
      if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);

      m_entityMasks[entity] |= 1 << getComponentPos<T>();
      SparseSet<T> &set = getComponentSet<T>();
      set[entity] = component;
    }

    template <typename T, typename... Args>
    T &emplace(const Entity &entity, Args &&...args) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);

      m_entityMasks[entity] |= 1 << getComponentPos<T>();
      SparseSet<T> &set = getComponentSet<T>();
      return set.emplace(entity, args...);
    }

    template <typename T>
    constexpr T &get(const Entity &entity) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);
      if (!(m_entityMasks[entity] & 1 << getComponentPos<T>())) throw CodeException(Code::OutOfBounds);

      SparseSet<T> &set = getComponentSet<T>();
      return set[entity];
    }

    template <typename T>
    constexpr const T &get(const Entity &entity) const {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);
      if (!(m_entityMasks[entity] & 1 << getComponentPos<T>())) throw CodeException(Code::OutOfBounds);

      const SparseSet<T> &set = getComponentSet<T>();
      return set[entity];
    }

    template <typename T>
    void remove(const Entity &entity) {
      if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
      if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);

      ComponentMask &mask = m_entityMasks[entity];
      ComponentMask componentMask = 1<<getComponentPos<T>();
      if (!(mask & componentMask)) return;

      mask &= ~componentMask;
      SparseSet<T> &set = getComponentSet<T>();
      set.remove(entity);
    }
  public:
    template <typename ...Components>
    ComponentView<Components...> view() {
      return { { getComponentPool<Components>()... } };
    }
  private:
    template <typename T>
    size_t getComponentPos() {
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
    ISparseSet<> *getComponentPool() {
      return m_componentPools[getComponentPos<T>()];
    }
  private:
    Array<Entity> m_deadEntities{}; // TODO: Change to Queue
    SparseSet<ComponentMask, Entity> m_entityMasks{};
    Array<ISparseSet<>*> m_componentPools = Array<ISparseSet<>*>(8);
    Entity m_entityCounter = 0;
    std::unordered_map<std::type_index, size_t> m_componentBitPositions{}; // I am too lazy to implement a hash map myself (for now)
  };

}

#endif // _PURRFECT_ENGINE_ECS_HPP_