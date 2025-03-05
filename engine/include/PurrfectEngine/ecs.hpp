#ifndef   _PURRFECT_ENGINE_ECS_HPP_
#define   _PURRFECT_ENGINE_ECS_HPP_

#include "sparseSet.hpp"
#include "queue.hpp"

#include <iostream>
#include <functional>
#include <unordered_map>
#include <typeindex>

namespace PurrfectEngine {

  using Entity = uint32_t;

  template<typename... Type>
  struct typeList { // "borrowed" from entt
    using type = typeList;
    static constexpr size_t size = sizeof...(Type);
  };

  template <typename ...Components>
  struct ComponentViewExclude : typeList<Components...> { // "borrowed" the idea from entt
    ComponentViewExclude() = default;
  };

  template <typename ...Components>
  inline constexpr ComponentViewExclude<Components...> exclude{};

  template <typename ...Components>
  struct ComponentViewGet : typeList<Components...> {
    ComponentViewGet() = default;
  };

  template <typename ...Components>
  inline constexpr ComponentViewGet<Components...> get{};

  template <typename, typename>
  class ComponentView;

  template <typename ...Get, typename ...Exclude>
  class ComponentView<ComponentViewGet<Get...>, ComponentViewExclude<Exclude...>> {
    friend class ECS;
  public:
    void forEach(std::function<void(Entity, Get &...)> func) {
      eachImpl(func);
    }

    void forEach(std::function<void(Get &...)> func) {
      eachImpl(func);
    }
  private:
    ComponentView(std::array<ISparseSet<>*, sizeof...(Get)> pools, std::array<ISparseSet<>*, sizeof...(Exclude)> excludePools)
      : m_viewPools(pools), m_excludePools(excludePools)
    {
      assert(sizeof...(Get) == m_viewPools.size() && "Size mismatch");
      assert(sizeof...(Exclude) == m_excludePools.size() && "Size mismatch");

      auto smallestPool = std::min_element(m_viewPools.begin(), m_viewPools.end(), [](ISparseSet<> *a, ISparseSet<> *b){
        return a->size() < b->size();
      });

      assert(smallestPool != m_viewPools.end() && "Invalid pools");

      m_smallest = *smallestPool;
    }
  private:
    template <size_t Index>
    auto getPool() {
      return static_cast<SparseSet<std::tuple_element_t<Index, GetTypes>>*>(m_viewPools[Index]);
    }

    template <size_t ...Indices>
    auto makeComponentTuple(Entity entity, std::index_sequence<Indices...>) {
      return std::make_tuple((std::ref(getPool<Indices>()->get(entity)))...);
    }

    bool isComplete(const Entity &entity) {
      return std::all_of(m_viewPools.begin(), m_viewPools.end(), [entity](ISparseSet<> *set){
        return set->contains(entity);
      }) && std::all_of(m_excludePools.begin(), m_excludePools.end(), [entity](ISparseSet<> *set){
        return !set->contains(entity);
      });
    }

    template <typename Func>
    void eachImpl(Func func) {
      constexpr auto inds = std::make_index_sequence<sizeof...(Get)>{};

      const Array<Entity> &sparse = m_smallest->getSparse();
      for (size_t i = 0; i < sparse.size(); ++i) {
        const Entity &entity = sparse[i];
        if (entity == m_smallest->tombstone || !isComplete(entity)) continue;

        if constexpr (std::is_invocable_v<Func, Entity, Get &...>) {
          std::apply(func, std::tuple_cat(std::make_tuple(entity), makeComponentTuple(entity, inds)));
        } else if constexpr (std::is_invocable_v<Func, Get &...>) {
          std::apply(func, makeComponentTuple(entity, inds));
        } else assert(0 && "Bad lambda");
      }
    }
  private:
    using GetTypes = std::tuple<Get...>;
    using ExcludeTypes = std::tuple<Exclude...>;

    std::array<ISparseSet<>*, sizeof...(Get)> m_viewPools;
    std::array<ISparseSet<>*, sizeof...(Exclude)> m_excludePools;
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