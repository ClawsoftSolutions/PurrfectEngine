#include "PurrfectEngine/PurrfectEngine.hpp"

namespace PurrfectEngine::renderer {

  PipelineDescriptor::PipelineDescriptor(Renderer *renderer)
  {}

  PipelineDescriptor::~PipelineDescriptor() {

  }

  Shader::Shader(Renderer *renderer, ShaderType type, const char *filename)
  {
    purrr_shader_info_t info = {
      .type = (purrr_shader_type_t)type,
      .filename = filename
    };
    assert(mHandle = purrr_shader_create(&info, renderer->getRenderer()));
  }

  Shader::Shader(Renderer *renderer, ShaderType type, char *buffer, size_t size)
  {
    purrr_shader_info_t info = {
      .type = (purrr_shader_type_t)type,
      .buffer = buffer,
      .buffer_size = size,
    };
    assert(mHandle = purrr_shader_create(&info, renderer->getRenderer()));
  }

  Shader::~Shader() {
    purrr_shader_destroy(mHandle);
  }

  Pipeline::Pipeline(Renderer *renderer, PipelineCreateInfo createInfo)
  {
    std::vector<purrr_shader_t*> shaders{};
    shaders.reserve(createInfo.shaders.size());

    std::vector<purrr_vertex_info_t> vertexInfos{};
    vertexInfos.reserve(createInfo.vertexAttributes.size());

    for (auto &shader : createInfo.shaders) {
      assert(shader);
      shaders.push_back(shader->get());
    }

    {
      uint32_t vertexSize = 0;
      for (const Format &format : createInfo.vertexAttributes) {
        uint32_t size = formatSize(format);
        vertexInfos.push_back(purrr_vertex_info_t{
          (purrr_format_t)format, size, vertexSize
        });
        vertexSize += size;
      }
    }

    purrr_pipeline_info_t info = {
      .shaders             = shaders.data(),
      .shader_count        = static_cast<uint32_t>(shaders.size()),
      .mesh_info           = purrr_mesh_binding_info_t{
        .vertex_infos      = vertexInfos.data(),
        .vertex_info_count = static_cast<uint32_t>(vertexInfos.size()),
      },
      .pipeline_descriptor = createInfo.descriptor->get(),
      .descriptor_slots = reinterpret_cast<purrr_descriptor_type_t*>(createInfo.descriptorSlots.data()),
      .descriptor_slot_count = static_cast<uint32_t>(createInfo.descriptorSlots.size()),
      // .push_constants = ,
      // .push_constant_count = ,
    };
    assert(mHandle = purrr_pipeline_create(&info, renderer->getRenderer()));
  }

  Pipeline::~Pipeline() {
    purrr_pipeline_destroy(mHandle);
  }

  void Pipeline::bind(Renderer *renderer) {
    purrr_renderer_bind_pipeline(renderer->getRenderer(), mHandle);
  }

}