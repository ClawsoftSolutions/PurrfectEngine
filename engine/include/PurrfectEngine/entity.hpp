#ifndef   _PURRFECT_ENGINE_ENTITY_HPP_
#define   _PURRFECT_ENGINE_ENTITY_HPP_

namespace PurrfectEngine {

  class Entity {
    friend class Scene;
  private:
    Entity(Scene *scene, EntityID id);
  public:
    template <typename T, typename... Args>
    T &addComponent(Args... args);

    template <typename T>
    T &getComponent();
    
    template <typename T>
    void removeComponent();

    inline operator EntityID() const { return mHandle; }
  private:
    Scene *mScene;
    entt::entity mHandle;
  };

}

#endif // _PURRFECT_ENGINE_ENTITY_HPP_