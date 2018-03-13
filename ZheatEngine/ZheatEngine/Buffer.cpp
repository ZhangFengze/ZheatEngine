#include "Buffer.h"

std::atomic_size_t Buffer::allocatedSize = 0;

std::shared_ptr<Buffer> Buffer::Alloc(size_t size)
{
	auto p = new Buffer();
	p->buffer = new char[size];
	p->size = size;
	allocatedSize += size;

	return std::shared_ptr<Buffer>(p);
}

size_t Buffer::AllocatedSize()
{
	return allocatedSize;
}

void * Buffer::Get()
{
	return buffer;
}

const void * Buffer::Get() const
{
	return buffer;
}

size_t Buffer::Size() const
{
	return size;
}

Buffer::~Buffer()
{
	delete[] buffer;
	allocatedSize -= size;
}
