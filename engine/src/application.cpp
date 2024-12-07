#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine {

  Application::Application()
  {}

  Application::~Application() {
  }

  bool Application::init() {
    return initialize();
  }

  bool Application::run() {
    while (isRunning()) {
      if (!update()) return false;
    }
    cleanup();
    return true;
  }

}