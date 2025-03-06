#include "PurrfectEngine/ecs.hpp"

#include <iostream>

namespace PurrfectEngine {

  Entity ECS::create() {
    Entity entity = m_entityMasks.tombstone;
    if (m_deadEntities.empty()) entity = m_entityCounter++;
    else entity = m_deadEntities.pop();

    m_entityMasks.emplace(entity, 0);

    return entity;
  }

  void ECS::removeEntity(const Entity &entity) {
    if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
    if (!m_entityMasks.contains(entity)) throw CodeException(Code::DeadEntity);

    m_deadEntities.push(entity);

    const ComponentMask &mask = m_entityMasks[entity];
    for (size_t i = 0; i < m_componentPools.size(); ++i)
      if (mask & (1 << i))
        m_componentPools[i]->remove(entity);
    m_entityMasks.remove(entity);
  }

  void ECS::clear() {
    for (ISparseSet<> *&set : m_componentPools)
      set->clear();
    m_entityMasks.clear();
    m_deadEntities.clear();
    m_entityCounter = 0;
  }

}