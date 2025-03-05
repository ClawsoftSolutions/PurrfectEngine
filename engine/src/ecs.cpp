#include "PurrfectEngine/ecs.hpp"

namespace PurrfectEngine {

  Entity ECS::create() {
    Entity entity = m_entityMasks.tombstone;
    if (m_deadEntities.size()) {
      entity = m_deadEntities.back();
      m_deadEntities.fastRemove(m_deadEntities.size()-1);
    } else entity = m_entityCounter++;

    m_entityMasks.emplace(entity, 0);

    return entity;
  }

  void ECS::removeEntity(const Entity &entity) {
    if (entity >= m_entityCounter) throw CodeException(Code::OutOfBounds);
    if (m_deadEntities.has(entity)) throw CodeException(Code::DeadEntity);

    m_deadEntities.append(entity);

    const ComponentMask &mask = m_entityMasks[entity];
    for (size_t i = 0; i < m_componentPools.size(); ++i)
      if (mask & (1 << i))
        m_componentPools[i]->remove(entity);
    m_entityMasks.remove(entity);
  }

}