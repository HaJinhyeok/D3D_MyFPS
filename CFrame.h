#pragma once
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

class CFrame
{
private:
	static unsigned int m_Frame;
	static float m_FPS_Num, m_LastTime;

public:
	CFrame();
	~CFrame();
};

