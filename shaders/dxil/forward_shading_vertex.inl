#define BYTE uint8_t
#ifndef NDEBUG
#include "debug/_internal_forward_shading_vertex.inl"
#else
#include "release/_internal_forward_shading_vertex.inl"
#endif
#undef BYTE