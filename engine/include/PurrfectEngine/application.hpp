#ifndef   _PURRFECT_ENGINE_APPLICATION_HPP_
#define   _PURRFECT_ENGINE_APPLICATION_HPP_

namespace PurrfectEngine {

  class Application {
  public:
    Application();
    ~Application();

    virtual bool initialize() = 0;
    virtual bool update() = 0;
    virtual void cleanup() = 0;

    bool init();
    bool run();

    virtual bool isRunning() const = 0;
  protected:
  };

  Application *CreateApplication(int argc, char **argv);

}

#endif // _PURRFECT_ENGINE_APPLICATION_HPP_