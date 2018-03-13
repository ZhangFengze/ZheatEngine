#pragma once

#include <memory>
#include <functional>

#include <QList>
#include <QVariant>

#include <Windows.h>

#include "Scan.h"

class Scan;

void HandleDeleter(HANDLE* h);

class Session
{
public:
	// nullptr if failed
	static Session* NewSession(int pid);

public:
	void AbortScan();

	void NewScan(Scan::ValueType type,const ScanOption& option);
	Scan* Scan();

	// process handle
	HANDLE Handle() const;

private:
	Session(HANDLE h);

private:
	std::unique_ptr<HANDLE, decltype(&HandleDeleter)> handle;
	std::unique_ptr<::Scan> scan;
};
