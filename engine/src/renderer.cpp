#include "PurrfectEngine/PurrfectEngine.hpp"

#include <assert.h>

namespace PurrfectEngine {

  bool Renderer::init() {
    mRenderTarget = new renderer::RenderTarget(nullptr);
    mPipelineDescriptor = new renderer::PipelineDescriptor((purrr_pipeline_descriptor_t*)nullptr);

    purrr_renderer_info_t renderer_info = {
      .window = mWindow->getWindow(),
      .vsync = true,
      .swapchain_pipeline_descriptor = &mPipelineDescriptor->get(),
      .swapchain_render_target = &mRenderTarget->get(),
    };
    if (!(mRenderer = purrr_renderer_create(&renderer_info))) return false;

    return initialize();
  }

  void Renderer::destroy() {
    purrr_renderer_wait(mRenderer);
    Renderer::cleanup();
    purrr_renderer_destroy(mRenderer);
  }

  void Renderer::begin() {
    purrr_renderer_begin_frame(mRenderer);
  }

  void Renderer::draw(uint32_t vertexCount) {
    purrr_renderer_draw(mRenderer, 1, 0, vertexCount, 0);
  }

  void Renderer::end() {
    purrr_renderer_end_frame(mRenderer);
  }

}