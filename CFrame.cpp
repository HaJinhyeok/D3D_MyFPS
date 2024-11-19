#include "CFrame.h"
void CFrame::Initialize()
{
	mFps = 0;
	mCount = 0;
	mStartTime = timeGetTime();
}
void CFrame::Frame()
{
	mCount++;
	// Sleep(1000 / nLimit);
	// Sleep(15);
	// '����' �ð��� '����' �ð����κ��� 1000ms(1��) ������ ��
	if (timeGetTime() >= mStartTime + 1000)
	{
		mFps = mCount;
		// ī��Ʈ �ʱ�ȭ
		mCount = 0;
		// '����' �ð��� �ٽ� '����' �ð����� ����
		mStartTime = timeGetTime();
	}
}
int CFrame::GetFps()
{
	return mFps;
}
void CFrame::WatchStart()
{
	mStopWatch.StartTime();
}
DWORD CFrame::WatchTimeCheck()
{
	return mStopWatch.TimeCheck();
}
DWORD CFrame::WatchEnd()
{
	return mStopWatch.EndTime();
}

BOOL CFrame::IsWatchWorking()
{
	return mStopWatch.IsWorking();
}