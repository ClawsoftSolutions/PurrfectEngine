#ifndef   _PURRFECT_ENGINE_RENDERER_RENDER_TARGET_HPP_
#define   _PURRFECT_ENGINE_RENDERER_RENDER_TARGET_HPP_

namespace PurrfectEngine {
namespace renderer {

  struct RenderTargetCreateInfo {
    PipelineDescriptor *pipelineDescriptor;
    uint32_t width, height;
  };

  struct RenderTargetScope {
    Renderer *renderer;

    inline ~RenderTargetScope() {
      purrr_renderer_end_render_target(renderer->getRenderer());
    }
  };

  class RenderTarget : public BindableEx<RenderTargetScope>, public Wrapper<purrr_render_target_t*> {
    friend class Renderer;
  public:
    inline RenderTarget(purrr_render_target_t *handle)
      : Wrapper(handle)
    {}

    RenderTarget(Renderer *renderer, RenderTargetCreateInfo createInfo);
    ~RenderTarget();
  public:
    Texture *getTexture(uint32_t index);
  private:
    virtual RenderTargetScope bind(Renderer *renderer) override;
  };

  #define bind_render_target(renderer, renderTarget) auto renderTargetScope = (renderer)->bind((renderTarget));

}
}

#endif // _PURRFECT_ENGINE_RENDERER_RENDER_TARGET_HPP_