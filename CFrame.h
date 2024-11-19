#pragma once
#include "Stopwatch.h"

class CFrame
{
private:
	int mFps, mCount;
	unsigned long mStartTime;

	Stopwatch mStopWatch;

public:
	CFrame() = default;
	~CFrame() = default;
	void Initialize();
	void Frame();
	int GetFps();

	void WatchStart();
	DWORD WatchTimeCheck();
	DWORD WatchEnd();

	BOOL IsWatchWorking();

};

