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

#ifndef _IMPORT_ASSET_STRING_H_
#define _IMPORT_ASSET_VECTOR_H_ 1

#include <string>
#include "import_asset_allocator.h"

template <typename CharT>
using import_asset_basic_string = std::basic_string<CharT, std::char_traits<CharT>, import_asset_allocator<CharT>>;

using import_asset_string = import_asset_basic_string<char>;
using import_asset_wstring = import_asset_basic_string<wchar_t>;

#endif