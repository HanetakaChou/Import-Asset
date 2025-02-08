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

#ifndef _INTERNAL_IMPORT_PNG_IMAGE_H_
#define _INTERNAL_IMPORT_PNG_IMAGE_H_ 1

#include "../../McRT-Malloc/include/mcrt_vector.h"
#include <cstddef>
#include <cstdint>

extern bool internal_import_png_image(void const* data_base, size_t data_size, mcrt_vector<uint32_t>& out_rgba_data, uint32_t* out_width, uint32_t* out_height);

#endif