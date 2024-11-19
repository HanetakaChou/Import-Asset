#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif
namespace kaitai
{

    /**
     * Common vector types shared by multiple formats
     */

    class vector_types_t : public kaitai::kstruct
    {

    public:
        class vec4_t;
        class color4_t;
        class color3_t;
        class vec2_t;
        class vec3_t;

        vector_types_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, vector_types_t *p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~vector_types_t();

        class vec4_t : public kaitai::kstruct
        {

        public:
            vec4_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, vector_types_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~vec4_t();

        private:
            float m_x;
            float m_y;
            float m_z;
            float m_w;
            vector_types_t *m__root;
            kaitai::kstruct *m__parent;

        public:
            float x() const { return m_x; }
            float y() const { return m_y; }
            float z() const { return m_z; }
            float w() const { return m_w; }
            vector_types_t *_root() const { return m__root; }
            kaitai::kstruct *_parent() const { return m__parent; }
        };

        class color4_t : public kaitai::kstruct
        {

        public:
            color4_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, vector_types_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~color4_t();

        private:
            float m_r;
            float m_g;
            float m_b;
            float m_a;
            vector_types_t *m__root;
            kaitai::kstruct *m__parent;

        public:
            float r() const { return m_r; }
            float g() const { return m_g; }
            float b() const { return m_b; }
            float a() const { return m_a; }
            vector_types_t *_root() const { return m__root; }
            kaitai::kstruct *_parent() const { return m__parent; }
        };

        class color3_t : public kaitai::kstruct
        {

        public:
            color3_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, vector_types_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~color3_t();

        private:
            float m_r;
            float m_g;
            float m_b;
            vector_types_t *m__root;
            kaitai::kstruct *m__parent;

        public:
            float r() const { return m_r; }
            float g() const { return m_g; }
            float b() const { return m_b; }
            vector_types_t *_root() const { return m__root; }
            kaitai::kstruct *_parent() const { return m__parent; }
        };

        class vec2_t : public kaitai::kstruct
        {

        public:
            vec2_t(kaitai::kstream *p__io, mmd_pmx_t::vertex_t *p__parent = nullptr, vector_types_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~vec2_t();

        private:
            float m_x;
            float m_y;
            vector_types_t *m__root;
            mmd_pmx_t::vertex_t *m__parent;

        public:
            float x() const { return m_x; }
            float y() const { return m_y; }
            vector_types_t *_root() const { return m__root; }
            mmd_pmx_t::vertex_t *_parent() const { return m__parent; }
        };

        class vec3_t : public kaitai::kstruct
        {

        public:
            vec3_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, vector_types_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~vec3_t();

        private:
            float m_x;
            float m_y;
            float m_z;
            vector_types_t *m__root;
            kaitai::kstruct *m__parent;

        public:
            float x() const { return m_x; }
            float y() const { return m_y; }
            float z() const { return m_z; }
            vector_types_t *_root() const { return m__root; }
            kaitai::kstruct *_parent() const { return m__parent; }
        };

    private:
        vector_types_t *m__root;
        kaitai::kstruct *m__parent;

    public:
        vector_types_t *_root() const { return m__root; }
        kaitai::kstruct *_parent() const { return m__parent; }
    };
}
