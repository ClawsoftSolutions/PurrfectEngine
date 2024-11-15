#include "PurrfectEngine/PurrfectEngine.hpp"

#include <stdio.h>

namespace PurrfectEngine {

  purrAppRendererExt::purrAppRendererExt(purrRenderer *renderer, purrWindowInitInfo windowInfo):
    mRenderer(renderer), mWindowInfo(windowInfo)
  {}

  purrAppRendererExt::~purrAppRendererExt() {
    delete mRenderer;
    delete mWindow;
  }

  bool purrAppRendererExt::initialize() {
    mWindow = new purrWindow();
    if (!mWindow->initialize(mWindowInfo)) return false;
    input::SetWindow(mWindow);

    mRenderer = new purrRenderer3D();
    return mRenderer->initialize(mWindow);
  }

  bool purrAppRendererExt::preUpdate() {
    glfwPollEvents();
    input::update();
    return !mWindow->shouldClose();
  }

  bool purrAppRendererExt::update() {
    return mRenderer->render();
  }

  void purrAppRendererExt::cleanup() {
    mRenderer->waitIdle();
    mRenderer->cleanup();
  }

  purrAppAudioExt::purrAppAudioExt()
  {}

  purrAppAudioExt::~purrAppAudioExt() {

  }

  bool purrAppAudioExt::initialize() {
    return purrAudioEngine::create() && purrAudioEngine::getInstance()->initialize();
  }

  bool purrAppAudioExt::preUpdate() {
    return true;
  }

  bool purrAppAudioExt::update() {
    if (!purrAudioEngine::getInstance()) return false;
    purrAudioEngine::getInstance()->update();
    return true;
  }

  void purrAppAudioExt::cleanup() {
    delete purrAudioEngine::getInstance();
  }

  purrApp::purrApp(purrAppCreateInfo createInfo, std::vector<purrAppExt*> extensions):
    purrExtendable<purrAppExt>(extensions), mCreateInfo(createInfo)
  {
    sInstance = this;
  }

  purrApp::~purrApp() {

  }

  bool purrApp::init() {
    return extsInitialize() && initialize();
  }

  void purrApp::run() {
    float lastTime = 0;
    while (mRunning) {
      float time = (float)glfwGetTime();
      float deltaTime = time - lastTime;
      lastTime = time;

      if (!extsPreUpdate() || !update(deltaTime) || !extsUpdate()) break;
    }

    extsCleanup();
    cleanup();
    for (purrAppExt *ext: getExtensions()) delete ext;
  }

}