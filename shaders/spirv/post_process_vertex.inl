constexpr uint32_t const post_process_vertex_shader_module_code[] = {
#ifndef NDEBUG
#include "debug/_internal_post_process_vertex.inl"
#else
#include "release/_internal_post_process_vertex.inl"
#endif
};