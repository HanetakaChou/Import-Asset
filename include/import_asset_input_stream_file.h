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

#ifndef _IMPORT_ASSET_INPUT_STREAM_FILE_H_
#define _IMPORT_ASSET_INPUT_STREAM_FILE_H_ 1

#include "import_asset_input_stream.h"

#if defined(__GNUC__)

#elif defined(_MSC_VER)

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>

class import_asset_input_stream_file : public import_asset_input_stream
{
	HANDLE m_file;

public:
	import_asset_input_stream_file();
	void init(char const *file_name);
	void uninit();
	~import_asset_input_stream_file();
	int stat_size(int64_t *size) override;
	intptr_t read(void *data, size_t size) override;
	int64_t seek(int64_t offset, int whence) override;
};

#else
#error Unknown Compiler
#endif

#endif