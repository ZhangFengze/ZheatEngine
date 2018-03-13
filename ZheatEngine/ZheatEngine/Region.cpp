#include <fstream>

#include <QString>
#include <QFile>

#include "Region.h"
#include "Buffer.h"
#include "Settings.h"
#include "Log.h"
#include "HelperFunctions.h"

Region::Region(
	void * remote_, size_t size_, size_t alignment_,
	size_t typeSize_, size_t id_, HANDLE h, const QDir& dir)
	:
	remote(remote_), size(size_), alignment(alignment_), typeSize(typeSize_), id(id_), handle(h),
	path(dir.filePath(QString("Region-%1.MEM").arg(id_)))
{
	auto end = size - typeSize + 1;
	auto n = end % alignment == 0 ? end / alignment : end / alignment + 1;
	bitset = BitSet(n, true);
}

Region::~Region()
{
	ReleaseMemory();
}

void * Region::PosToRemote(size_t pos)
{
	return (char*)remote + PosToOffset(pos);
}

size_t Region::PosToOffset(size_t pos)
{
	return pos * alignment;
}

std::shared_ptr<Buffer> Region::ReadRemoteMemory()
{
	auto buffer = Buffer::Alloc(size);

	// TODO
	// don't read all
	// TODO
	// check API result
	ReadProcessMemory(handle, remote, buffer->Get(), size, nullptr);

	return buffer;
}

void Region::LoadMemory()
{
	if (!memory)
	{
		std::ifstream ifs(path.toStdString(), std::ios_base::binary);
		if (!ifs)
			ErrorExit("can't open " + path);

		memory = Buffer::Alloc(size);
		char* p = (char*)memory->Get();

		ifs.read(p, size);
		ifs.close();
		if (!QFile::remove(path))
			ErrorExit("can't remove " + path);
	}
}

void Region::SaveMemory()
{
	RemoveIfExist(path);

	if (Buffer::AllocatedSize() <= gPreferredMemoryUsage * 1024 * 1024)
		return;

	std::ofstream ofs(path.toStdString(), std::ios_base::binary);
	if (!ofs)
		ErrorExit("can't open " + path);

	char* p = (char*)memory->Get();
	ofs.write(p, size);
	ofs.flush();

	memory.reset();
}

void Region::SaveMemory(std::shared_ptr<Buffer> buffer)
{
	memory = buffer;
	SaveMemory();
}

void Region::ReleaseMemory()
{
	memory.reset();
	RemoveIfExist(path);
}
