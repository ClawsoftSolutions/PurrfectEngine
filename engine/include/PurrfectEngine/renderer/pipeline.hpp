#ifndef   _PURRFECT_ENGINE_RENDERER_PIPELINE_HPP_
#define   _PURRFECT_ENGINE_RENDERER_PIPELINE_HPP_

namespace PurrfectEngine {
namespace renderer {

  class PipelineDescriptor : public Wrapper<purrr_pipeline_descriptor_t*> {
    friend class Renderer;
  public:
    inline PipelineDescriptor(purrr_pipeline_descriptor_t *handle)
      : Wrapper(handle)
    {}

    PipelineDescriptor(Renderer *renderer);
    ~PipelineDescriptor();
  };

  class Shader : public Wrapper<purrr_shader_t*> {
    friend class Renderer;
  public:
    inline Shader(purrr_shader_t *handle)
      : Wrapper(handle)
    {}

    Shader(Renderer *renderer, ShaderType type, const char *filename);
    Shader(Renderer *renderer, ShaderType type, char *buffer, size_t size);
    ~Shader();
  };

  struct PipelineCreateInfo {
    PipelineDescriptor *descriptor;
    std::vector<Shader*> shaders;
    std::vector<Format> vertexAttributes;
    std::vector<DescriptorType> descriptorSlots;
  };

  class Pipeline : public Bindable<>, public Wrapper<purrr_pipeline_t*> {
    friend class Renderer;
  public:
    inline Pipeline(purrr_pipeline_t *handle)
      : Wrapper(handle)
    {}

    Pipeline(Renderer *renderer, PipelineCreateInfo createInfo);
    ~Pipeline();
  private:
    virtual void bind(Renderer *renderer) override;
  };

}
}

#endif // _PURRFECT_ENGINE_RENDERER_PIPELINE_HPP_