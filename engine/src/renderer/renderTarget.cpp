#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine::renderer {

  RenderTarget::RenderTarget(Renderer *renderer, RenderTargetCreateInfo createInfo)
  {
    purrr_render_target_info_t info = {
      .pipeline_descriptor = createInfo.pipelineDescriptor->get(),
      .width               = createInfo.width, 
      .height              = createInfo.height,
    };
    assert(mHandle = purrr_render_target_create(&info, renderer->getRenderer()));
  }

  RenderTarget::~RenderTarget() {
    purrr_render_target_destroy(mHandle);
  }

  RenderTargetScope RenderTarget::bind(Renderer *renderer) {
    RenderTargetScope scope = { renderer };
    purrr_renderer_begin_render_target(renderer->getRenderer(), mHandle);
    return scope;
  }

}