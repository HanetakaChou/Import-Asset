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

#ifndef _IMPORT_ASSET_INPUT_STREAM_H_
#define _IMPORT_ASSET_INPUT_STREAM_H_ 1

#include <stddef.h>
#include <stdint.h>

class import_asset_input_stream_factory;
class import_asset_input_stream;

enum
{
	IMPORT_ASSET_INPUT_STREAM_SEEK_SET = 0,
	IMPORT_ASSET_INPUT_STREAM_SEEK_CUR = 1,
	IMPORT_ASSET_INPUT_STREAM_SEEK_END = 2
};

extern import_asset_input_stream_factory *import_asset_init_file_input_stream_factory();

extern void import_asset_destroy_file_input_stream_factory(import_asset_input_stream_factory *input_stream_factory);

extern import_asset_input_stream_factory *import_asset_init_memory_input_stream_factory(size_t input_stream_count, char const *const *input_stream_file_names, void const *const *input_stream_memory_range_bases, size_t const *input_stream_memory_range_sizes);

extern void import_asset_destroy_memory_input_stream_factory(import_asset_input_stream_factory *input_stream_factory);

class import_asset_input_stream_factory
{
public:
	virtual import_asset_input_stream *create_instance(char const *file_name) = 0;
	virtual void destory_instance(import_asset_input_stream *input_stream) = 0;
};

class import_asset_input_stream
{
public:
	virtual int stat_size(int64_t *size) = 0;
	virtual intptr_t read(void *data, size_t size) = 0;
	virtual int64_t seek(int64_t offset, int whence) = 0;
};

#endif
