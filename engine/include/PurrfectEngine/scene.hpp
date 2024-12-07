#ifndef   _PURRFECT_ENGINE_SCENE_HPP_
#define   _PURRFECT_ENGINE_SCENE_HPP_

namespace PurrfectEngine {

  using EntityID = entt::entity;

  class Entity;
  class Scene {
    friend class Entity;
  public:
    Scene() = default;

    Entity *newEntity();
    Entity *getEntity(EntityID id);
    void removeEntity(EntityID id);
  private:
    inline operator entt::registry&() { return mRegistry; }
  private:
    entt::registry mRegistry{};
  };

}

#endif // _PURRFECT_ENGINE_SCENE_HPP_