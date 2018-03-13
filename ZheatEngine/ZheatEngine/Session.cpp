#include <algorithm>
#include <cassert>

#include "Session.h"
#include "ScanNBytes.h"
#include "ScanFloatType.h"
#include "Log.h"

Session * Session::NewSession(int pid)
{
	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL)
	{
		LogWarning(QString("OpenProcess %1 failed, error code %2").arg(pid).arg(GetLastError()));
		return nullptr;
	}
	return new Session(hProcess);
}

void Session::AbortScan()
{
	scan.reset(nullptr);
}

void Session::NewScan(Scan::ValueType type, const ScanOption & option)
{
	switch (type)
	{
	case Scan::ValueType::Scan1Bytes:
		scan.reset(new Scan1Bytes(Handle(), option));
		return;
	case Scan::ValueType::Scan2Bytes:
		scan.reset(new Scan2Bytes(Handle(), option));
		return;
	case Scan::ValueType::Scan4Bytes:
		scan.reset(new Scan4Bytes(Handle(), option));
		return;
	case Scan::ValueType::Scan8Bytes:
		scan.reset(new Scan8Bytes(Handle(), option));
		return;

	case Scan::ValueType::ScanFloat:
		scan.reset(new ScanFloat(Handle(), option));
		return;
	case Scan::ValueType::ScanDouble:
		scan.reset(new ScanDouble(Handle(), option));
		return;

	default:
		ErrorExit("Unknown ScanType");
	}
}

Scan * Session::Scan()
{
	return scan.get();
}

HANDLE Session::Handle() const
{
	return *handle;
}

Session::Session(HANDLE h)
	:handle(new HANDLE(h), &HandleDeleter)
{
}

void HandleDeleter(HANDLE* h)
{
	if (!CloseHandle(*h))
		ErrorExit("CloseHandle");
	delete h;
}

