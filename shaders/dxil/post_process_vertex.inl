#define BYTE uint8_t
#ifndef NDEBUG
#include "debug/_internal_post_process_vertex.inl"
#else
#include "release/_internal_post_process_vertex.inl"
#endif
#undef BYTE