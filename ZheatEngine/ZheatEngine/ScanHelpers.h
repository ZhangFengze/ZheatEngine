#pragma once

#include <cassert>
#include <algorithm>
#include <functional>
#include <list>

#include <QDir>
#include <QList>

#include <ppl.h>

#include "Region.h"

namespace ScanHelpers
{

template<typename T>
struct Result
{
	T* remote;
	T value;

	// simple slow version for GUI (typically about 15 calls per UpdateResultsValues)
	bool NowValue(HANDLE hProcess, T& v) const
	{
		return ReadProcessMemory(hProcess, remote, &v, sizeof v, nullptr);
	}

	bool SetValue(HANDLE hProcess, const T& newVal)
	{
		return WriteProcessMemory(hProcess, remote, &newVal, sizeof newVal, nullptr);
	}
};

template<typename T>
void ProcessRegion(Region& region, std::function<bool(T*)>& predNewValue, bool& shouldRemove)
{
	auto mem = region.ReadRemoteMemory();
	char* memory = (char*)mem->Get();

	for (int i = 0; i < region.bitset.Size(); ++i)
	{
		if (!region.bitset[i])
			continue;

		auto offset = region.PosToOffset(i);

		T* value = (T*)(memory + offset);

		if (!predNewValue(value))
			region.bitset.Reset(i);
	}
	if (region.bitset.CountTrue() == 0)
		shouldRemove = true;
	else
	{
		region.SaveMemory(mem);
		shouldRemove = false;
	}
}

template<typename T>
void ProcessRegion(Region& region, std::function<bool(T*, T*)>& predOldAndNewValue, bool& shouldRemove)
{
	region.LoadMemory();
	char* oldMemory = (char*)region.memory->Get();

	auto newMem = region.ReadRemoteMemory();
	char* newMemory = (char*)newMem->Get();

	for (int i = 0; i < region.bitset.Size(); ++i)
	{
		if (!region.bitset[i])
			continue;

		auto offset = region.PosToOffset(i);

		T* oldValue = (T*)(oldMemory + offset);
		T* newValue = (T*)(newMemory + offset);

		if (!predOldAndNewValue(oldValue, newValue))
			region.bitset.Reset(i);

	}
	if (region.bitset.CountTrue() == 0)
		shouldRemove = true;
	else
	{
		region.SaveMemory(newMem);
		shouldRemove = false;
	}
}

template<typename T>
size_t NumResults(const std::list<Region>& regions)
{
	return std::accumulate(regions.begin(), regions.end(), size_t(0),
		[](size_t now, const Region& region)
	{
		return now + region.bitset.CountTrue();
	});
}

template<typename T>
QList<Result<T>> Results(std::list<Region>& regions, size_t maxCount)
{
	QList<Result<T>> li;
	int cnt = 0;

	// results should be added in order
	for (auto& region : regions)
	{
		if (cnt == maxCount)
			break;

		region.LoadMemory();
		char* memory = (char*)region.memory->Get();

		for (size_t pos = 0; pos < region.bitset.Size() && cnt < maxCount; ++pos)
		{
			if (!region.bitset[pos])
				continue;

			auto remote = region.PosToRemote(pos);
			auto offset = region.PosToOffset(pos);
			T* val = (T*)(memory + offset);
			li.push_back(Result<T>{ (T*)remote, *val });
			++cnt;
		}
		region.SaveMemory();
	}
	return li;
}

void CollectRegions(std::list<Region>& regions, void* remoteStart,
	void* remoteStop, HANDLE processHandle, int alignment, size_t typeSize, const QDir& dir);

template<typename T>
void ProcessRegions(std::list<Region>& regions, std::function<bool(T*)>& predNewValue)
{
	concurrency::parallel_for_each(regions.begin(), regions.end(),
		[](Region& region)
	{
		region.ReleaseMemory();
	});

	// ProcessRegion may remove iter, causes problem in parallel_for
	std::vector<std::list<Region>::iterator> iters;
	for (auto iter = regions.begin(); iter != regions.end(); ++iter)
		iters.push_back(iter);

	concurrency::parallel_for_each(iters.begin(), iters.end(),
		[&regions, &predNewValue](std::list<Region>::iterator iter)
	{
		// flatten the memory usage
		while (Buffer::AllocatedSize() > gPreferredMemoryUsage * 1024 * 1024)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		bool shouldRemove;
		ProcessRegion<T>(*iter, predNewValue, shouldRemove);
		if (shouldRemove)
			regions.erase(iter);
	});
}

template<typename T>
void ProcessRegions(std::list<Region>& regions, std::function<bool(T*, T*)>& predOldAndNewValue)
{
	// ProcessRegion may remove iter, causes problem in parallel_for
	std::vector<std::list<Region>::iterator> iters;
	for (auto iter = regions.begin(); iter != regions.end(); ++iter)
		iters.push_back(iter);

	concurrency::parallel_for_each(iters.begin(), iters.end(),
		[&regions, &predOldAndNewValue](std::list<Region>::iterator iter)
	{
		// flatten the memory usage
		while (Buffer::AllocatedSize() > gPreferredMemoryUsage * 1024 * 1024)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		bool shouldRemove;
		ProcessRegion<T>(*iter, predOldAndNewValue, shouldRemove);
		if (shouldRemove)
			regions.erase(iter);
	});
}

}
