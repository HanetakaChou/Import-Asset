//
// Copyright (C) YuqiaoZhang(HanetakaChou)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef _IMPORT_SCENE_ASSET_H_
#define _IMPORT_SCENE_ASSET_H_ 1

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <string>
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#include <DirectXMath.h>
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#include "import_asset_input_stream.h"

class scene_animation_joint
{
    DirectX::XMFLOAT4 m_quaternion;
    DirectX::XMFLOAT3 m_translation;

public:
    inline void init()
    {
        DirectX::XMStoreFloat4(&this->m_quaternion, DirectX::XMQuaternionIdentity());
        DirectX::XMStoreFloat3(&this->m_translation, DirectX::XMVectorZero());
    }

    inline void set_transform(DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation)
    {
        this->m_quaternion = quaternion;
        this->m_translation = translation;
    }

    inline DirectX::XMFLOAT4 get_quaternion() const
    {
        return this->m_quaternion;
    }

    inline DirectX::XMFLOAT3 get_translation() const
    {
        return this->m_translation;
    }
};

class scene_animation_pose
{
    std::vector<scene_animation_joint> m_joints;

public:
    inline void init(size_t joint_count)
    {
        this->m_joints.resize(joint_count);

        for (size_t joint_index = 0; joint_index < joint_count; ++joint_index)
        {
            this->m_joints[joint_index].init();
        }
    }

    inline void set_transform(size_t joint_index, DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation)
    {
        this->m_joints[joint_index].set_transform(quaternion, translation);
    }

    inline size_t get_joint_count() const
    {
        return this->m_joints.size();
    }

    inline DirectX::XMFLOAT4 get_quaternion(size_t joint_index) const
    {
        assert(joint_index < this->m_joints.size());
        return this->m_joints[joint_index].get_quaternion();
    }

    inline DirectX::XMFLOAT3 get_translation(size_t joint_index) const
    {
        assert(joint_index < this->m_joints.size());
        return this->m_joints[joint_index].get_translation();
    }
};

// Bake Animation
// Since this demo is for rendering, we do NOT care too much about the animation. And we simply sample the poses for each frame.
class scene_animation_skeleton
{
    std::vector<scene_animation_pose> m_poses;

public:
    inline void init(size_t frame_count, size_t joint_count)
    {
        this->m_poses.resize(frame_count);

        for (size_t frame_index = 0; frame_index < frame_count; ++frame_index)
        {
            this->m_poses[frame_index].init(joint_count);
        }
    }

    inline void set_transform(size_t frame_index, size_t joint_index, DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation)
    {
        this->m_poses[frame_index].set_transform(joint_index, quaternion, translation);
    }

    inline size_t get_joint_count() const
    {
        return ((this->m_poses.size() > 0) ? (this->m_poses[0].get_joint_count()) : 0);
    }

    inline scene_animation_pose const &get_pose(size_t frame_index) const
    {
        size_t const frame_count = this->m_poses.size();
        assert(frame_count > 0);

        size_t const pose_index = frame_index % frame_count;
        return this->m_poses[pose_index];
    }
};

struct scene_mesh_vertex_position_binding
{
    // R32G32B32_FLOAT
    DirectX::XMFLOAT3 m_position;
};

struct scene_mesh_vertex_varying_binding
{
    // R16G16_SNORM (octahedron map)
    uint32_t m_normal;
    // R15G15B2_SNORM (octahedron map + tangent w)
    uint32_t m_tangent;
    // R16G16_UNORM
    uint32_t m_texcoord;
};

struct scene_mesh_vertex_joint_binding
{
    // R16G16B16A16_UINT (xy)
    uint32_t m_indices_xy;
    // R16G16B16A16_UINT (wz)
    uint32_t m_indices_wz;
    // R8G8B8A8_UNORM
    uint32_t m_weights;
};

struct scene_mesh_subset_data
{
    std::vector<scene_mesh_vertex_position_binding> m_vertex_position_binding;

    std::vector<scene_mesh_vertex_varying_binding> m_vertex_varying_binding;

    std::vector<scene_mesh_vertex_joint_binding> m_vertex_joint_binding;

    std::vector<uint32_t> m_indices;

    uint32_t m_max_index;

    // we assume that the meshes with the same materials have been merged

    std::string m_normal_texture_image_uri;

    float m_normal_texture_scale;

    DirectX::XMFLOAT3 m_emissive_factor;

    std::string m_emissive_texture_image_uri;

    DirectX::XMFLOAT3 m_base_color_factor;

    std::string m_base_color_texture_image_uri;

    float m_metallic_factor;

    float m_roughness_factor;

    std::string m_metallic_roughness_texture_image_uri;
};

struct scene_mesh_instance_data
{
    DirectX::XMFLOAT4X4 m_model_transform;
    scene_animation_skeleton m_animation_skeleton;
};

struct scene_mesh_data
{
    bool m_skinned;

    std::vector<scene_mesh_subset_data> m_subsets;

    std::vector<scene_mesh_instance_data> m_instances;
};

extern bool import_gltf_scene_asset(std::vector<scene_mesh_data> &out_total_mesh_data, float frame_rate, import_asset_input_stream_factory *input_stream_factory, char const *path);

#endif