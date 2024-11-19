#include "Stopwatch.h"

void Stopwatch::StartTime()
{
	m_startTimePoint = timeGetTime();
	m_bIsWorking = TRUE;
}
DWORD Stopwatch::TimeCheck()
{
	DWORD checkPoint = timeGetTime();
	DWORD duration = checkPoint - m_startTimePoint;
	return duration;
}
DWORD Stopwatch::EndTime()
{
	DWORD endTimePoint = timeGetTime();
	DWORD duration = endTimePoint - m_startTimePoint;
	m_startTimePoint = endTimePoint;
	m_bIsWorking = FALSE;
	return duration;
}
BOOL Stopwatch::IsWorking()
{
	return m_bIsWorking;
}