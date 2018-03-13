#pragma once

#include <QDir>

#include <Windows.h>

#include "BitSet.h"
#include "Buffer.h"

// no need to encapsulate
// just all public members

struct Region
{
public:
	Region(void* remote, size_t size, size_t alignment, size_t typeSize, size_t id, HANDLE h, const QDir& dir);
	~Region();

	void* PosToRemote(size_t pos);
	size_t PosToOffset(size_t pos);

	std::shared_ptr<Buffer> ReadRemoteMemory();

	void LoadMemory();

	void SaveMemory();
	void SaveMemory(std::shared_ptr<Buffer>);

	void ReleaseMemory();

public:
	void* remote;
	size_t size;
	size_t alignment;
	size_t typeSize;
	size_t id;
	HANDLE handle;
	QString path;
	BitSet bitset;
	std::shared_ptr<Buffer> memory;
};