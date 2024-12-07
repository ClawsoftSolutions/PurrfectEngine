#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  Entity *Scene::newEntity() {
    EntityID id = mRegistry.create();
    return new Entity(this, id);
  }

  Entity *Scene::getEntity(EntityID id) {
    return new Entity(this, id);
  }

  void Scene::removeEntity(EntityID id) {
    mRegistry.destroy(id);
  }

}