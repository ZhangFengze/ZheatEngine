#pragma once

#include <atomic>
#include <memory>

class Buffer
{
public:
	static std::shared_ptr<Buffer> Alloc(size_t size);
	static size_t AllocatedSize();

public:
	~Buffer();

	void* Get();
	const void* Get() const;
	size_t Size() const;

private:
	Buffer() = default;

private:
	static std::atomic_size_t allocatedSize;
	void* buffer;
	size_t size;

};
