#pragma once

#include <Windows.h>

void PauseProcess(HANDLE);
void ResumeProcess(HANDLE);

class ProcessPauseManager
{
public:
	ProcessPauseManager(HANDLE);
	~ProcessPauseManager();
private:
	HANDLE handle;
};
