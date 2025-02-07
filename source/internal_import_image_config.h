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

#ifndef _INTERNAL_IMPORT_IMAGE_CONFIG_H_
#define _INTERNAL_IMPORT_IMAGE_CONFIG_H_ 1

#include <cstddef>
#include <cstdint>

// D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION	16384
static constexpr size_t const k_max_width_or_height = 16384;

static constexpr int const k_channel_size = sizeof(uint8_t);
static constexpr int const k_num_channels = 4U;

#endif
