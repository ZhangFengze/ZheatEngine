#include "Log.h"
#include "PauseProcess.h"

ProcessPauseManager::ProcessPauseManager(HANDLE processHandle)
	:handle(processHandle)
{
	PauseProcess(handle);
}

ProcessPauseManager::~ProcessPauseManager()
{
	ResumeProcess(handle);
}

void PauseProcess(HANDLE handle)
{
	typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
	static NtSuspendProcess p = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle("ntdll"), "NtSuspendProcess");
	if (NULL == p)
		ErrorExit("Get NtSuspendProcess from ntdll");

	// without any documents, don't know how to check result..
	p(handle);
}

void ResumeProcess(HANDLE handle)
{
	typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);
	static NtResumeProcess p = (NtResumeProcess)GetProcAddress(
		GetModuleHandle("ntdll"), "NtResumeProcess");
	if (NULL == p)
		ErrorExit("Get NtResumeProcess from ntdll");

	// without any documents, don't know how to check result..
	p(handle);
}
