#ifndef   _PURRFECT_ENGINE_ENTRY_HPP_
#define   _PURRFECT_ENGINE_ENTRY_HPP_

namespace PurrfectEngine {

#if   defined(PE_WINDOWS)

#elif defined(PE_LINUX)

#else
#  error "Unsupported platform!"
#endif

  int Main(int argc, char **argv) {
    Application *app = CreateApplication(argc, argv);

    if (!app->init()) {
      fprintf(stderr, "Failed to initialize application!\n");
      return 1;
    }

    if (!app->run()) {
      fprintf(stderr, "Something went wrong while running application!\n");
      return 1;
    }

    delete app;
    return 0;
  }

}

int main(int argc, char **argv) {
  return PurrfectEngine::Main(argc, argv);
}

#endif // _PURRFECT_ENGINE_ENTRY_HPP_