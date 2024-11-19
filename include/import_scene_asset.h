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

class scene_animation_joint
{
    DirectX::XMFLOAT4 m_quaternion;
    DirectX::XMFLOAT3 m_translation;

public:
    void init();
    void set_transform(DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation);
    DirectX::XMFLOAT4 get_quaternion() const;
    DirectX::XMFLOAT3 get_translation() const;
};

class scene_animation_pose
{
    std::vector<scene_animation_joint> m_joints;

public:
    void init(size_t joint_count);
    void set_transform(size_t joint_index, DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation);
    size_t get_joint_count() const;
    DirectX::XMFLOAT4 get_quaternion(size_t joint_index) const;
    DirectX::XMFLOAT3 get_translation(size_t joint_index) const;
};

// Bake Animation
// Since this demo is for rendering, we do NOT care too much about the animation. And we simply sample the poses for each frame.
class scene_animation_skeleton
{
    std::vector<scene_animation_pose> m_poses;

public:
    void init(size_t frame_count, size_t joint_count);
    void set_transform(size_t frame_index, size_t joint_index, DirectX::XMFLOAT4 const &quaternion, DirectX::XMFLOAT3 const &translation);
    size_t get_joint_count() const;
    scene_animation_pose const &get_pose(size_t frame_index) const;
};

struct scene_mesh_vertex_position_binding
{
    // R32G32B32_FLOAT
    DirectX::XMFLOAT3 m_position;
};

struct scene_mesh_vertex_varying_binding
{
    // R10G10B10A2_UNORM
    uint32_t m_normal;
    // R10G10B10A2_UNORM
    uint32_t m_tangent;
    // R16G16_UNORM
    uint32_t m_texcoord;
};

struct scene_mesh_vertex_skinned_binding
{
    // R16G16B16A16_UINT
    uint32_t m_indices_xy;
    uint32_t m_indices_wz;
    // R8G8B8A8_UNORM
    uint32_t m_weights;
};

struct scene_mesh_subset_data
{
    std::vector<scene_mesh_vertex_position_binding> m_vertex_position_binding;

    std::vector<scene_mesh_vertex_varying_binding> m_vertex_varying_binding;

    std::vector<scene_mesh_vertex_skinned_binding> m_vertex_skinned_binding;

    std::vector<uint32_t> m_indices;

    uint32_t m_max_index;

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

extern bool import_scene_asset_gltf(std::vector<scene_mesh_data> &out_total_mesh_data, float frame_rate, char const *path);

#endif