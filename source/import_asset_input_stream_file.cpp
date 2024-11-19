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

#include "../include/import_asset_input_stream_file.h"
#include <cstring>
#include <assert.h>

#if defined(__GNUC__)

#elif defined(_MSC_VER)

#include "../../thirdparty/ConvertUTF/include/ConvertUTF.h"

import_asset_input_stream_file::import_asset_input_stream_file() : m_file(INVALID_HANDLE_VALUE)
{
}

void import_asset_input_stream_file::init(char const *file_name)
{
	assert(INVALID_HANDLE_VALUE == this->m_file);

	std::wstring file_name_utf16;
	{
		std::string file_name_utf8 = file_name;
		bool res_convert_utf8_To_utf16_string = llvm::convertUTF8ToUTF16String(file_name_utf8, file_name_utf16);
		assert(res_convert_utf8_To_utf16_string);
	}

	this->m_file = CreateFileW(file_name_utf16.c_str(), FILE_READ_DATA | FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(INVALID_HANDLE_VALUE != this->m_file);
}

void import_asset_input_stream_file::uninit()
{
	assert(INVALID_HANDLE_VALUE != this->m_file);

	BOOL res_close_handle = CloseHandle(this->m_file);
	assert(FALSE != res_close_handle);

	this->m_file = INVALID_HANDLE_VALUE;
}

import_asset_input_stream_file::~import_asset_input_stream_file()
{
	assert(INVALID_HANDLE_VALUE == this->m_file);
}

int import_asset_input_stream_file::stat_size(int64_t *size)
{
	assert(INVALID_HANDLE_VALUE != this->m_file);

	LARGE_INTEGER length;
	BOOL res_get_file_size_ex = GetFileSizeEx(this->m_file, &length);

	(*size) = static_cast<int64_t>(length.QuadPart);

	return ((FALSE != res_get_file_size_ex) ? (0) : (-1));
}

intptr_t import_asset_input_stream_file::read(void *data, size_t size)
{
	assert(INVALID_HANDLE_VALUE != this->m_file);

	DWORD read_size;
	BOOL res_read_file = ReadFile(this->m_file, data, static_cast<DWORD>(size), &read_size, NULL);

	return (FALSE != res_read_file) ? (static_cast<intptr_t>(read_size)) : (-1);
}

int64_t import_asset_input_stream_file::seek(int64_t offset, int whence)
{
	assert(INVALID_HANDLE_VALUE != this->m_file);

	DWORD move_method;
	switch (whence)
	{
	case IMPORT_ASSET_INPUT_STREAM_SEEK_SET:
	{
		move_method = FILE_BEGIN;
	}
	break;
	case IMPORT_ASSET_INPUT_STREAM_SEEK_CUR:
	{
		move_method = FILE_CURRENT;
	}
	break;
	case IMPORT_ASSET_INPUT_STREAM_SEEK_END:
	{
		move_method = FILE_END;
	}
	break;
	default:
		assert(false);
		move_method = -1;
	}

	LARGE_INTEGER distance_to_move;
	distance_to_move.QuadPart = static_cast<LONGLONG>(offset);

	LARGE_INTEGER new_file_pointer;
	BOOL res_set_file_pointer_ex = SetFilePointerEx(this->m_file, distance_to_move, &new_file_pointer, move_method);
	return (FALSE != res_set_file_pointer_ex) ? (static_cast<int64_t>(new_file_pointer.QuadPart)) : (-1);
}

#else
#error Unknown Compiler
#endif