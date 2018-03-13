#pragma once

#include <cassert>
#include <memory>
#include <algorithm>
#include <list>

#include <QList>
#include <QVariant>

#include <Windows.h>
#include <ppl.h>

#include "Scan.h"
#include "Session.h"
#include "Region.h"
#include "Settings.h"
#include "ScanHelpers.h"
#include "Log.h"
#include "HelperFunctions.h"
#include "PauseProcess.h"

class Session;

template<typename T>
class ScanFloatType :public Scan
{
public:
	ScanFloatType(HANDLE processHandle, const ScanOption& option);

	using Result = ScanHelpers::Result<T>;

	enum class InitialScanType
	{
		Equal, NotEqual,
		GreaterThan, LessThan,
		GreaterThanOrEqualTo, LessThanOrEqualTo,
		Between, NotBetween,
		Unknown
	};

	void InitialScan(int initialScanType, const QList<QVariant>& param) override;

	enum class ScanType
	{
		Equal, NotEqual,
		GreaterThan, LessThan,
		GreaterThanOrEqualTo, LessThanOrEqualTo,
		Between, NotBetween,
		IncreasedValue, IncreasedBy, NotIncreasedBy,
		DecreasedValue, DecreasedBy, NotDecreasedBy,
		ChangedValue, UnChangedValue
	};

	void NextScan(int scanType, const QList<QVariant>& param) override;

	size_t NumResults() const override;

	QList<Result> Results(size_t maxCount = -1);

private:

	bool NeedOldValue(int scanType);

	std::function<bool(T*)> PredNewValue(ScanType type, const QList<QVariant>& parameters);
	std::function<bool(T*, T*)> PredOldAndNewValue(ScanType type, const QList<QVariant>& parameters);

private:
	std::list<Region> regions;
};

using ScanFloat = ScanFloatType<float>;
using ScanDouble = ScanFloatType<double>;

template<typename T>
ScanFloatType<T>::ScanFloatType(HANDLE h, const ScanOption & op)
	:Scan(h, op)
{
}

template<typename T>
void ScanFloatType<T>::InitialScan(int t, const QList<QVariant> & param)
{
	auto pause = std::make_shared<ProcessPauseManager>(option.pause ? handle : nullptr);

	ScanHelpers::CollectRegions(regions, option.start, option.stop,
		handle, option.alignment, sizeof T, QDir(tempDir.path()));

	if ((InitialScanType)t == InitialScanType::Unknown)
	{
		concurrency::parallel_for_each(regions.begin(), regions.end(),
			[](Region& region)
		{
			// flatten the memory usage
			while (Buffer::AllocatedSize() > gPreferredMemoryUsage * 1024 * 1024)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

			auto mem = region.ReadRemoteMemory();
			region.SaveMemory(mem);
		});
	}
	else
		NextScan(t, param);
}

template<typename T>
void ScanFloatType<T>::NextScan(int t, const QList<QVariant>& param)
{
	auto pause = std::make_shared<ProcessPauseManager>(option.pause ? handle : nullptr);

	auto type = (ScanType)t;

	if (NeedOldValue(t))
	{
		auto pred = PredOldAndNewValue(type, param);
		ScanHelpers::ProcessRegions<T>(regions, pred);
	}
	else
	{
		auto pred = PredNewValue(type, param);
		ScanHelpers::ProcessRegions<T>(regions, pred);
	}
}

template<typename T>
size_t ScanFloatType<T>::NumResults() const
{
	return ScanHelpers::NumResults<T>(regions);
}

template<typename T>
QList<typename ScanFloatType<T>::Result> ScanFloatType<T>::Results(size_t maxCount)
{
	return ScanHelpers::Results<T>(regions, maxCount);
}

template<typename T>
bool ScanFloatType<T>::NeedOldValue(int scanType)
{
	ScanType type = (ScanType)scanType;
	switch (type)
	{
	case Scan4Bytes::ScanType::IncreasedValue:
	case Scan4Bytes::ScanType::IncreasedBy:
	case Scan4Bytes::ScanType::NotIncreasedBy:

	case Scan4Bytes::ScanType::DecreasedValue:
	case Scan4Bytes::ScanType::DecreasedBy:
	case Scan4Bytes::ScanType::NotDecreasedBy:

	case Scan4Bytes::ScanType::ChangedValue:
	case Scan4Bytes::ScanType::UnChangedValue:
		return true;

	case Scan4Bytes::ScanType::Equal:
	case Scan4Bytes::ScanType::NotEqual:

	case Scan4Bytes::ScanType::GreaterThan:
	case Scan4Bytes::ScanType::LessThan:

	case Scan4Bytes::ScanType::GreaterThanOrEqualTo:
	case Scan4Bytes::ScanType::LessThanOrEqualTo:

	case Scan4Bytes::ScanType::Between:
	case Scan4Bytes::ScanType::NotBetween:
		return false;

	default:
		ErrorExit("Unknown ScanFloatType::ScanType");
	}
}

template<typename T>
std::function<bool(T*)> ScanFloatType<T>::PredNewValue(ScanType type, const QList<QVariant>& parameters)
{
	T param0 = parameters.size() > 0 ? ToType<T>(parameters[0]) : T();
	T param1 = parameters.size() > 1 ? ToType<T>(parameters[1]) : T();

	switch (type)
	{
	case ScanType::Equal:
	{
		return [param0](T* val) -> bool
		{
			return *val == param0;
		};
	}
	case ScanType::NotEqual:
	{
		return [param0](T* val) -> bool
		{
			return *val != param0;
		};
	}
	case ScanType::GreaterThan:
	{
		return [param0](T* val) -> bool
		{
			return *val > param0;
		};
	}
	case ScanType::LessThan:
	{
		return [param0](T* val) -> bool
		{
			return *val < param0;
		};
	}
	case ScanType::GreaterThanOrEqualTo:
	{
		return [param0](T* val) -> bool
		{
			return *val >= param0;
		};
	}
	case ScanType::LessThanOrEqualTo:
	{
		return [param0](T* val) -> bool
		{
			return *val <= param0;
		};
	}
	case ScanType::Between:
	{
		if (param0 > param1)
			std::swap(param0, param1);
		return [param0, param1](T* val) -> bool
		{
			return  param0 < *val && *val < param1;
		};
	}
	case ScanType::NotBetween:
	{
		if (param0 > param1)
			std::swap(param0, param1);
		return [param0, param1](T* val) -> bool
		{
			return   *val < param0 || param1 < *val;
		};
	}
	default:
		ErrorExit("Illegal ScanType");
	}
}

template<typename T>
std::function<bool(T*, T*)> ScanFloatType<T>::PredOldAndNewValue(ScanType type, const QList<QVariant>& parameters)
{
	T param = parameters.size() > 0 ? ToType<T>(parameters[0]) : T();

	switch (type)
	{
	case ScanType::IncreasedValue:
	{
		return [](T* oldVal, T* newVal) -> bool
		{
			return *newVal > *oldVal;
		};
	}
	case ScanType::IncreasedBy:
	{
		return [param](T* oldVal, T* newVal) -> bool
		{
			return *newVal = *oldVal + param;
		};
	}
	case ScanType::NotIncreasedBy:
	{
		return [param](T* oldVal, T* newVal) -> bool
		{
			return *newVal != *oldVal + param;
		};
	}
	case ScanType::DecreasedValue:
	{
		return [](T* oldVal, T* newVal) -> bool
		{
			return *newVal < *oldVal;
		};
	}
	case ScanType::DecreasedBy:
	{
		return [param](T* oldVal, T* newVal) -> bool
		{
			return *newVal = *oldVal - param;
		};
	}
	case ScanType::NotDecreasedBy:
	{
		return [param](T* oldVal, T* newVal) -> bool
		{
			return *newVal != *oldVal - param;
		};
	}
	case ScanType::ChangedValue:
	{
		return [](T* oldVal, T* newVal) -> bool
		{
			return *newVal != *oldVal;
		};
	}
	case ScanType::UnChangedValue:
	{
		return [](T* oldVal, T* newVal) -> bool
		{
			return *newVal == *oldVal;
		};
	}
	default:
		ErrorExit("Illegal ScanType");
	}
}
