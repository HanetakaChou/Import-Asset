// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "vector_types.h"
namespace kaitai
{

    vector_types_t::vector_types_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = this;
        _read();
    }

    void vector_types_t::_read()
    {
    }

    vector_types_t::~vector_types_t()
    {
        _clean_up();
    }

    void vector_types_t::_clean_up()
    {
    }

    vector_types_t::vec4_t::vec4_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = p__root;
        _read();
    }

    void vector_types_t::vec4_t::_read()
    {
        m_x = m__io->read_f4le();
        m_y = m__io->read_f4le();
        m_z = m__io->read_f4le();
        m_w = m__io->read_f4le();
    }

    vector_types_t::vec4_t::~vec4_t()
    {
        _clean_up();
    }

    void vector_types_t::vec4_t::_clean_up()
    {
    }

    vector_types_t::color4_t::color4_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = p__root;
        _read();
    }

    void vector_types_t::color4_t::_read()
    {
        m_r = m__io->read_f4le();
        m_g = m__io->read_f4le();
        m_b = m__io->read_f4le();
        m_a = m__io->read_f4le();
    }

    vector_types_t::color4_t::~color4_t()
    {
        _clean_up();
    }

    void vector_types_t::color4_t::_clean_up()
    {
    }

    vector_types_t::color3_t::color3_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = p__root;
        _read();
    }

    void vector_types_t::color3_t::_read()
    {
        m_r = m__io->read_f4le();
        m_g = m__io->read_f4le();
        m_b = m__io->read_f4le();
    }

    vector_types_t::color3_t::~color3_t()
    {
        _clean_up();
    }

    void vector_types_t::color3_t::_clean_up()
    {
    }

    vector_types_t::vec2_t::vec2_t(kaitai::kstream *p__io, mmd_pmx_t::vertex_t *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = p__root;
        _read();
    }

    void vector_types_t::vec2_t::_read()
    {
        m_x = m__io->read_f4le();
        m_y = m__io->read_f4le();
    }

    vector_types_t::vec2_t::~vec2_t()
    {
        _clean_up();
    }

    void vector_types_t::vec2_t::_clean_up()
    {
    }

    vector_types_t::vec3_t::vec3_t(kaitai::kstream *p__io, kaitai::kstruct *p__parent, vector_types_t *p__root) : kaitai::kstruct(p__io)
    {
        m__parent = p__parent;
        m__root = p__root;
        _read();
    }

    void vector_types_t::vec3_t::_read()
    {
        m_x = m__io->read_f4le();
        m_y = m__io->read_f4le();
        m_z = m__io->read_f4le();
    }

    vector_types_t::vec3_t::~vec3_t()
    {
        _clean_up();
    }

    void vector_types_t::vec3_t::_clean_up()
    {
    }
}
