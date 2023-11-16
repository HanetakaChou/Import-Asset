#define BYTE uint8_t
#ifndef NDEBUG
#include "debug/_internal_post_process_fragment.inl"
#else
#include "release/_internal_post_process_fragment.inl"
#endif
#undef BYTE