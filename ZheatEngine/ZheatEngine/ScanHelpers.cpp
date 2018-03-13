#include "ScanHelpers.h"
#include "Log.h"
#include "Settings.h"

namespace ScanHelpers
{

void CollectRegions(std::list<Region>& regions, void * remoteStart, void * remoteStop, HANDLE processHandle, int alignment, size_t typeSize, const QDir & dir)
{
	assert(regions.empty());

	MEMORY_BASIC_INFORMATION info = {};

	for (void* remote = remoteStart;
		remote <= remoteStop && 0 != VirtualQueryEx(processHandle, remote, &info, sizeof info);
		remote = (char*)remote + info.RegionSize)
	{
		remote = info.BaseAddress;

		// TODO
		// check more
		// add settings
		if (info.State != MEM_COMMIT)
			continue;
		if (info.Protect & PAGE_GUARD || info.Protect& PAGE_NOACCESS)
			continue;

		if (!gScanMEM_PRIVATE && info.Type == MEM_PRIVATE)
			continue;
		if (!gScanMEM_IMAGE && info.Type == MEM_IMAGE)
			continue;
		if (!gScanMEM_MAPPED && info.Type == MEM_MAPPED)
			continue;

		if (!regions.empty())
		{
			auto& last = regions.back();
			auto lastEnd = (char*)last.remote + last.size;
			if ((void*)lastEnd == remote)
			{
				last.size += info.RegionSize;
				continue;
			}
		}
		regions.emplace_back(remote, info.RegionSize, alignment, typeSize, regions.size(), processHandle, dir);
	}
}

}
