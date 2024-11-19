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

#ifndef _IMPORT_ASSET_VECTOR_H_
#define _IMPORT_ASSET_VECTOR_H_ 1

#include <unordered_map>
#include <string_view>
#include "import_asset_allocator.h"
#include "import_asset_string.h"

namespace std
{
    template <>
    struct hash<import_asset_string>
    {
        inline std::size_t operator()(import_asset_string const &key) const noexcept
        {
            return std::hash<std::string_view>()(std::string_view(key.data(), key.size()));
        }
    };

    template <>
    struct hash<import_asset_wstring>
    {
        inline std::size_t operator()(import_asset_wstring const &key) const noexcept
        {
            return std::hash<std::wstring_view>()(std::wstring_view(key.data(), key.size()));
        }
    };
}

template <typename Key, typename T>
using import_asset_unordered_map = std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, import_asset_allocator<std::pair<const Key, T>>>;

#endif