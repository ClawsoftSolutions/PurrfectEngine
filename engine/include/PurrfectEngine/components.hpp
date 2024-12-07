#ifndef   _PURRFECT_ENGINE_COMPONENTS_HPP_
#define   _PURRFECT_ENGINE_COMPONENTS_HPP_

namespace PurrfectEngine {

  struct Transform3DComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
  };

  struct Transform2DComponent {
    glm::vec2 position;
    uint32_t depth;
    float rotation;
    glm::vec2 scale;
  };

  struct CameraComponent {
    float far = 1000.0f, near = 0.1f;
    float fov = 90.0f;
  };

}

#endif // _PURRFECT_ENGINE_COMPONENTS_HPP_