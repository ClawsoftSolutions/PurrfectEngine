#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  Entity::Entity(Scene *scene, EntityID id)
    : mScene(scene), mHandle(id)
  {}

  template <typename T, typename... Args>
  T &Entity::addComponent(Args... args) {
    return ((entt::registry)(*mScene)).emplace_or_replace<T>(mHandle, args...);
  }

  template <typename T>
  T &Entity::getComponent() {
    return ((entt::registry)(*mScene)).get<T>(mHandle);
  }
  
  template <typename T>
  void Entity::removeComponent() {
    ((entt::registry)(*mScene)).remove<T>(mHandle);
  }

}