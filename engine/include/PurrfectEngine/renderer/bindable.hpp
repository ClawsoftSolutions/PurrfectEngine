#ifndef   _PURRFECT_ENGINE_RENDERER_BINDABLE_HPP_
#define   _PURRFECT_ENGINE_RENDERER_BINDABLE_HPP_

namespace PurrfectEngine {

  class Renderer;

namespace renderer {

  template <typename... Args>
  class Bindable {
    friend class ::PurrfectEngine::Renderer;
  private:
    virtual void bind(Renderer *renderer, Args... args) = 0;
  };

  template <typename T, typename... Args>
  class BindableEx {
    friend class ::PurrfectEngine::Renderer;
  private:
    virtual T bind(Renderer *renderer, Args... args) = 0;
  };

}
}

#endif // _PURRFECT_ENGINE_RENDERER_BINDABLE_HPP_