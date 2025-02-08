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

#ifndef _IMPORT_ASSET_MEMORY_INPUT_STREAM_H_
#define _IMPORT_ASSET_MEMORY_INPUT_STREAM_H_ 1

#include "../include/import_asset_input_stream.h"
#include "../../McRT-Malloc/include/mcrt_unordered_map.h"
#include "../../McRT-Malloc/include/mcrt_string.h"

class import_asset_memory_input_stream_factory final : public import_asset_input_stream_factory
{
	struct import_asset_memory_range
	{
		void const *m_memory_range_base;
		size_t m_memory_range_size;
	};

	mcrt_unordered_map<mcrt_string, import_asset_memory_range> m_memory_ranges;

public:
	import_asset_memory_input_stream_factory();
	void init(size_t input_stream_count, char const *const *input_stream_file_names, void const *const *input_stream_memory_range_bases, size_t const *input_stream_memory_range_sizes);
	void uninit();
	~import_asset_memory_input_stream_factory();

private:
	virtual import_asset_input_stream *create_instance(char const *file_name) override;
	virtual void destory_instance(import_asset_input_stream *input_stream) override;
};

class import_asset_memory_input_stream final : public import_asset_input_stream
{
	void const *m_memory_range_base;
	int64_t m_memory_range_size;
	int64_t m_memory_range_offset;

public:
	import_asset_memory_input_stream();
	void init(void const *memory_range_base, size_t memory_range_size);
	void uninit();
	~import_asset_memory_input_stream();
	int stat_size(int64_t *size) override;
	intptr_t read(void *data, size_t size) override;
	int64_t seek(int64_t offset, int whence) override;
};

#endif