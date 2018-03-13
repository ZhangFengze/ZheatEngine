#pragma once

#include <QList>
#include <QVariant>
#include <QTemporaryDir>

#include <Windows.h>

class Session;

struct ScanOption
{
	void* start;
	void* stop;
	int alignment;
	bool pause;
};

class Scan
{
public:
	enum class ValueType
	{
		Scan1Bytes, Scan2Bytes, Scan4Bytes, Scan8Bytes,
		ScanFloat, ScanDouble
	};

	virtual ~Scan() {};

	virtual void InitialScan(int type, const QList<QVariant>& param) = 0;

	virtual void NextScan(int type, const QList<QVariant>& param) = 0;

	virtual size_t NumResults() const = 0;

public:
	Scan(HANDLE handle, const ScanOption& option);

	QTemporaryDir& TempDir();

protected:
	// no need to close
	HANDLE handle;

	ScanOption option;

	QTemporaryDir tempDir;
};

inline Scan::ValueType ScanValueTypes[]{
	Scan::ValueType::Scan1Bytes,
	Scan::ValueType::Scan2Bytes,
	Scan::ValueType::Scan4Bytes,
	Scan::ValueType::Scan8Bytes,
	Scan::ValueType::ScanFloat,
	Scan::ValueType::ScanDouble
};

inline QString ScanValueStrings[]{ "Byte","2 Bytes","4 Bytes","8 Bytes","Float","Double" };

inline QString ScanValueTypeToString(Scan::ValueType v)
{
	return ScanValueStrings[(int)v];
}

