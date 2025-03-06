#ifndef   _PURRFECT_ENGINE_COMPONENT_VIEW_HPP_
#define   _PURRFECT_ENGINE_COMPONENT_VIEW_HPP_

#include "sparseSet.hpp"
#include "array.hpp"

#include <functional>
#include <algorithm>
#include <typeindex>
#include <array>
#include <assert.h>

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

      for (const Entity &entity : m_smallest->getSparse()) {
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

}

#endif // _PURRFECT_ENGINE_COMPONENT_VIEW_HPP_