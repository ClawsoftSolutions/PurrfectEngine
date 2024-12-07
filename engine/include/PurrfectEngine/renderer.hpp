#ifndef   _PURRFECT_ENGINE_RENDERER_HPP_
#define   _PURRFECT_ENGINE_RENDERER_HPP_

namespace PurrfectEngine {

namespace renderer {

  class RenderTarget;
  class PipelineDescriptor;

}

  // TODO: Make it a wrapper, I am too lazy to do it rn :p
  class Renderer {
  public:
    inline Renderer(Window *window)
      : mWindow(window)
    {}

    bool init();
    void destroy();
    virtual bool initialize() = 0;
    virtual bool update() = 0;
    virtual bool render() = 0;
    virtual void resize() {}
    virtual void cleanup() {}

    inline Window *getWindow() const { return mWindow; }

    inline purrr_renderer_t *getRenderer() const { return mRenderer; }
  protected:
    void begin();

    template <typename... Args>
    inline void bind(renderer::Bindable<Args...> *bindable, Args... args) {
      bindable->bind(this, args...);
    }

    template <typename T, typename... Args>
    inline T bind(renderer::BindableEx<T, Args...> *bindable, Args... args) {
      return bindable->bind(this, args...);
    }

    void draw(uint32_t vertexCount);

    void end();
  protected:
    Window *mWindow = nullptr;
    renderer::RenderTarget       *mRenderTarget       = nullptr;
    renderer::PipelineDescriptor *mPipelineDescriptor = nullptr;
  private:
    purrr_renderer_t *mRenderer = nullptr;
  };

}

#endif // _PURRFECT_ENGINE_RENDERER_HPP_