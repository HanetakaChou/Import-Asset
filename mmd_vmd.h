#pragma once

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <memory>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif
namespace kaitai
{

    /**
     * VMD is the format used by MikuMikuDance (MMD) for storing animation data.
     * A VMD file can contain data for character motion and cameras, but VMD
     * files tend to contain either one or the other, not both.
     */

    class mmd_vmd_t : public kaitai::kstruct
    {

    public:
        class header_t;
        class motion_t;
        class morph_t;
        class camera_t;

        mmd_vmd_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent = nullptr, mmd_vmd_t *p__root = nullptr);

    private:
        void _read();
        void _clean_up();

    public:
        ~mmd_vmd_t();

        class header_t : public kaitai::kstruct
        {

        public:
            header_t(kaitai::kstream *p__io, mmd_vmd_t *p__parent = nullptr, mmd_vmd_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~header_t();

        private:
            std::string m_magic;
            std::string m_name;
            mmd_vmd_t *m__root;
            mmd_vmd_t *m__parent;

        public:
            std::string magic() const { return m_magic; }
            std::string name() const { return m_name; }
            mmd_vmd_t *_root() const { return m__root; }
            mmd_vmd_t *_parent() const { return m__parent; }
        };

        class motion_t : public kaitai::kstruct
        {

        public:
            motion_t(kaitai::kstream *p__io, mmd_vmd_t *p__parent = nullptr, mmd_vmd_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~motion_t();

        private:
            std::string m_bone_name;
            uint32_t m_frame_number;
            std::unique_ptr<vector_types_t::vec3_t> m_position;
            std::unique_ptr<vector_types_t::vec4_t> m_rotation;
            std::unique_ptr<std::vector<uint8_t>> m_interpolation;
            mmd_vmd_t *m__root;
            mmd_vmd_t *m__parent;

        public:
            std::string bone_name() const { return m_bone_name; }
            uint32_t frame_number() const { return m_frame_number; }
            vector_types_t::vec3_t *position() const { return m_position.get(); }
            vector_types_t::vec4_t *rotation() const { return m_rotation.get(); }
            std::vector<uint8_t> *interpolation() const { return m_interpolation.get(); }
            mmd_vmd_t *_root() const { return m__root; }
            mmd_vmd_t *_parent() const { return m__parent; }
        };

        class morph_t : public kaitai::kstruct
        {

        public:
            morph_t(kaitai::kstream *p__io, mmd_vmd_t *p__parent = nullptr, mmd_vmd_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~morph_t();

        private:
            std::string m_morph_name;
            uint32_t m_frame_number;
            float m_weight;
            mmd_vmd_t *m__root;
            mmd_vmd_t *m__parent;

        public:
            std::string morph_name() const { return m_morph_name; }
            uint32_t frame_number() const { return m_frame_number; }
            float weight() const { return m_weight; }
            mmd_vmd_t *_root() const { return m__root; }
            mmd_vmd_t *_parent() const { return m__parent; }
        };

        class camera_t : public kaitai::kstruct
        {

        public:
            camera_t(kaitai::kstream *p__io, mmd_vmd_t *p__parent = nullptr, mmd_vmd_t *p__root = nullptr);

        private:
            void _read();
            void _clean_up();

        public:
            ~camera_t();

        private:
            uint32_t m_frame_number;
            float m_distance;
            std::unique_ptr<vector_types_t::vec3_t> m_position;
            std::unique_ptr<vector_types_t::vec3_t> m_rotation;
            std::unique_ptr<std::vector<uint8_t>> m_interpolation;
            uint32_t m_fov;
            uint8_t m_perspective;
            mmd_vmd_t *m__root;
            mmd_vmd_t *m__parent;

        public:
            uint32_t frame_number() const { return m_frame_number; }
            float distance() const { return m_distance; }
            vector_types_t::vec3_t *position() const { return m_position.get(); }
            vector_types_t::vec3_t *rotation() const { return m_rotation.get(); }
            std::vector<uint8_t> *interpolation() const { return m_interpolation.get(); }
            uint32_t fov() const { return m_fov; }
            uint8_t perspective() const { return m_perspective; }
            mmd_vmd_t *_root() const { return m__root; }
            mmd_vmd_t *_parent() const { return m__parent; }
        };

    private:
        std::unique_ptr<header_t> m_header;
        uint32_t m_num_motions;
        std::unique_ptr<std::vector<std::unique_ptr<motion_t>>> m_motions;
        uint32_t m_num_morphs;
        std::unique_ptr<std::vector<std::unique_ptr<morph_t>>> m_morphs;
        uint32_t m_num_cameras;
        std::unique_ptr<std::vector<std::unique_ptr<camera_t>>> m_cameras;
        std::string m_reserved;
        mmd_vmd_t *m__root;
        kaitai::kstruct *m__parent;

    public:
        header_t *header() const { return m_header.get(); }
        uint32_t num_motions() const { return m_num_motions; }
        std::vector<std::unique_ptr<motion_t>> *motions() const { return m_motions.get(); }
        uint32_t num_morphs() const { return m_num_morphs; }
        std::vector<std::unique_ptr<morph_t>> *morphs() const { return m_morphs.get(); }
        uint32_t num_cameras() const { return m_num_cameras; }
        std::vector<std::unique_ptr<camera_t>> *cameras() const { return m_cameras.get(); }
        std::string reserved() const { return m_reserved; }
        mmd_vmd_t *_root() const { return m__root; }
        kaitai::kstruct *_parent() const { return m__parent; }
    };
}
