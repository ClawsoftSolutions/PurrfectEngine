#define PURRFECT_ENGINE_ENTRY
#include <PurrfectEngine/PurrfectEngine.hpp>
#include <iostream>

using namespace PurrfectEngine;

class SandboxRenderer : public Renderer {
public:
  SandboxRenderer(Window* window)
    : Renderer(window)
  {}

  bool initialize() {
    {
      std::vector<renderer::Shader*> shaders{};
      shaders.reserve(2);
      shaders.push_back(new renderer::Shader(this, renderer::ShaderType::Vertex, "./vertex.spv"));
      shaders.push_back(new renderer::Shader(this, renderer::ShaderType::Fragment, "./fragment.spv"));

      renderer::PipelineCreateInfo createInfo{
        .descriptor = mPipelineDescriptor,
        .shaders = shaders,
        .vertexAttributes = { renderer::Format::RG32F, renderer::Format::RG32F },
        .descriptorSlots = {},
      };

      mPipeline = new renderer::Pipeline(this, createInfo);

      for (auto& shader : shaders) delete shader;
    }

    return true;
  }

  bool update() {
    return true;
  }

  bool render() {
    begin();

    {
      bind_render_target(this, mRenderTarget);
      bind(mPipeline);

      draw(3);
    }

    end();

    return true;
  }

  void resize() {}

private:
  renderer::Pipeline* mPipeline = nullptr;
};

class SandboxApp : public Application {
public:
  SandboxApp()
    : Application(), mWindow(new Window("PurrfectEngine Sandbox", 1920, 1080)), mRenderer(new SandboxRenderer(mWindow))
  {}

  bool initialize() {
    mRenderer->init();

    mDispatcher.subscribe<KeyPressEvent>(1, [](const KeyPressEvent& event) {
      printf("Key pressed: %d\n", event.getKeyCode());
    });

    mDispatcher.subscribe<Event>(1, [](const Event& event) {
      printf("%s fired!\n", event.getName());
    });

    mWindow->subscribe(&mEventQueue);

    return true;
  }

  bool update() {
    if (!mRenderer->update()) return false;
    if (!mRenderer->render()) return false;
    purrr_poll_events();

    mEventQueue.process(mDispatcher);

    return true;
  }

  void cleanup() {
    mRenderer->destroy();
    delete mRenderer;
    delete mWindow;
  }

  bool isRunning() const { return !mWindow->shouldClose(); }

private:
  Window* mWindow = nullptr;
  SandboxRenderer* mRenderer = nullptr;

  EventQueue mEventQueue{};
  EventDispatcher mDispatcher{};
};

namespace PurrfectEngine {
  Application* CreateApplication(int argc, char** argv) {
    return new SandboxApp();
  }
}
