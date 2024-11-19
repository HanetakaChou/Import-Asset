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

#include "../include/import_asset_input_stream_memory.h"
#include <cstring>
#include <assert.h>

import_asset_input_stream_memory::import_asset_input_stream_memory() : m_memory_range_base(NULL), m_memory_range_size(0), m_memory_range_offset(0)
{
}

void import_asset_input_stream_memory::init(void const *memory_range_base, size_t memory_range_size)
{
	assert(NULL == this->m_memory_range_base);
	this->m_memory_range_base = memory_range_base;

	assert(0 == this->m_memory_range_size);
	this->m_memory_range_size = memory_range_size;

	assert(0 == this->m_memory_range_offset);
}

void import_asset_input_stream_memory::uninit()
{
	assert(NULL != this->m_memory_range_base);
	this->m_memory_range_base = NULL;

	assert(0 != this->m_memory_range_size);
	this->m_memory_range_size = 0;

	this->m_memory_range_offset = 0;
}

import_asset_input_stream_memory::~import_asset_input_stream_memory()
{
	assert(NULL == this->m_memory_range_base);
	assert(0 == this->m_memory_range_size);
	assert(0 == this->m_memory_range_offset);
}

int import_asset_input_stream_memory::stat_size(int64_t *size)
{
	(*size) = this->m_memory_range_size;
	return 0;
}

intptr_t import_asset_input_stream_memory::read(void *data, size_t size)
{
	intptr_t size_read = ((this->m_memory_range_offset + static_cast<int64_t>(size)) <= this->m_memory_range_size) ? static_cast<int64_t>(size) : ((this->m_memory_range_offset < this->m_memory_range_size) ? (this->m_memory_range_size - this->m_memory_range_offset) : 0);
	if (size_read > 0)
	{
		std::memcpy(data, reinterpret_cast<void const *>(reinterpret_cast<intptr_t>(this->m_memory_range_base) + this->m_memory_range_offset), size_read);
		this->m_memory_range_offset += size_read;
	}
	return size_read;
}

int64_t import_asset_input_stream_memory::seek(int64_t offset, int whence)
{
	switch (whence)
	{
	case IMPORT_ASSET_INPUT_STREAM_SEEK_SET:
		this->m_memory_range_offset = offset;
		break;
	case IMPORT_ASSET_INPUT_STREAM_SEEK_CUR:
		this->m_memory_range_offset += offset;
		break;
	case IMPORT_ASSET_INPUT_STREAM_SEEK_END:
		this->m_memory_range_offset = this->m_memory_range_size + offset;
		break;
	default:
		this->m_memory_range_offset = -1;
	}

	return this->m_memory_range_offset;
}
