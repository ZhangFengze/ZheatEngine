#include "Scan.h"
#include "Settings.h"
#include "Log.h"

Scan::Scan(HANDLE h, const ScanOption& op)
	:handle(h), option(op), tempDir(gTempDir)
{
	if (!tempDir.isValid())
		ErrorExit("QTemporaryDir() failed");
	tempDir.setAutoRemove(true);
}

QTemporaryDir & Scan::TempDir()
{
	return tempDir;
}