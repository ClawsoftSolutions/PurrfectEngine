#include "PurrfectEngine/PurrfectEngine.hpp"

// namespace PurrfectEngine::builders {

//   uint32_t purrr_format_size(purrr_format_t format) {
//     switch (format) {
//     case PURRR_FORMAT_RGBA8U:   return 4;
//     case PURRR_FORMAT_RGBA8RGB: return 4;
//     case PURRR_FORMAT_BGRA8U:   return 4;
//     case PURRR_FORMAT_BGRA8RGB: return 4;
//     case PURRR_FORMAT_RGBA16F:  return 6;
//     case PURRR_FORMAT_RG32F:    return 8;
//     case PURRR_FORMAT_RGB32F:   return 12;
//     case PURRR_FORMAT_RGBA32F:  return 16;
//     case PURRR_FORMAT_RGBA64F:  return 32;
//     default: return 0;
//     }
//   }

//   PipelineBuilder::PipelineBuilder(purrr_pipeline_descriptor_t *descriptor)
//     : mDescriptor(descriptor)
//   {}

//   PipelineBuilder &PipelineBuilder::loadShader(purrr_shader_type_t type, const char *filepath) {
//     purrr_shader_info_t shader_info = {
//       .type = type,
//       .filename = filepath,
//     };
//     mShaderInfos.push_back(shader_info);
//     return *this;
//   }

//   PipelineBuilder &PipelineBuilder::addShader(purrr_shader_type_t type, char *buffer, const size_t &size) {
//     purrr_shader_info_t shader_info = {
//       .type = type,
//       .buffer = buffer,
//       .buffer_size = size,
//     };
//     mShaderInfos.push_back(shader_info);
//     return *this;
//   }

//   PipelineBuilder &PipelineBuilder::addShader(purrr_shader_t *shader) {
//     mShaders.push_back(shader);
//     return *this;
//   }

//   PipelineBuilder &PipelineBuilder::addVertexAttribute(purrr_format_t format) {
//     uint32_t size = purrr_format_size(format);
//     mVertexInfos.push_back(purrr_vertex_info_t{
//       .format = format,
//       .size = size,
//       .offset = mVertexSize,
//     });
//     mVertexSize += size;
//     return *this;
//   }

//   PipelineBuilder &PipelineBuilder::addDescriptor(purrr_descriptor_type_t type) {
//     mDescriptorSlots.push_back(type);
//     return *this;
//   }

//   purrr_pipeline_t *PipelineBuilder::build(purrr_renderer_t *renderer, bool deleteShaders) {
//     for (purrr_shader_info_t &info : mShaderInfos) {
//       purrr_shader_t *shader = purrr_shader_create(&info, renderer);
//       if (!shader) return nullptr;
//       mShaders.push_back(shader);
//     }
//     mShaderInfos.clear();

//     purrr_pipeline_info_t info = {
//       .shaders = mShaders.data(),
//       .shader_count = static_cast<uint32_t>(mShaders.size()),

//       .mesh_info = purrr_mesh_binding_info_t{
//         .vertex_infos = mVertexInfos.data(),
//         .vertex_info_count = static_cast<uint32_t>(mVertexInfos.size()),
//       },

//       .pipeline_descriptor = mDescriptor,

//       .descriptor_slots = mDescriptorSlots.data(),
//       .descriptor_slot_count = static_cast<uint32_t>(mDescriptorSlots.size()),

//       // purrr_pipeline_push_constant_t *push_constants;
//       // uint32_t push_constant_count;
//     };

//     purrr_pipeline_t *result = purrr_pipeline_create(&info, renderer);
//     if (deleteShaders)
//       for (purrr_shader_t *shader : mShaders)
//         purrr_shader_destroy(shader);
//     return result;
//   }

// }