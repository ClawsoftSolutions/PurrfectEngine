#ifndef   _PURRFECT_ENGINE_HPP_
#define   _PURRFECT_ENGINE_HPP_

#include <assert.h>

#include <vector>
#include <queue>
#include <unordered_map>

#include <purrr/purrr.h>

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#undef min
#undef max

namespace PurrfectEngine {

  template <typename T>
  class Wrapper {
  public:
    inline Wrapper(T handle)
      : mHandle(handle)
    {}

    inline Wrapper()
    {}
  public:
    const T &get() const { return mHandle; }
    T &get() { return mHandle; }
  protected:
    T mHandle;
  };

}

#include "scene.hpp"
#include "entity.hpp"
#include "components.hpp"

#include "window.hpp"

// Renderer:

#include "renderer/bindable.hpp"           // Internal use
#include "renderer.hpp"
#include "renderer/enums.hpp"
#include "renderer/texture.hpp"
#include "renderer/buffer.hpp"
#include "renderer/pipeline.hpp"
#include "renderer/renderTarget.hpp"

#include "application.hpp"

// Events:

#include "events/event.hpp"
#include "events/eventDispatcher.hpp"
#include "events/eventQueue.hpp"

// Renderers:
// #include "renderers/renderer2D.hpp"

#ifdef    PURRFECT_ENGINE_ENTRY
#  include "entry.hpp"
#endif // PURRFECT_ENGINE_ENTRY

#endif // _PURRFECT_ENGINE_HPP_

/*
 * TODO: Create an "interface" that will hold a handle
 *       for the type it accepts in template.
 */