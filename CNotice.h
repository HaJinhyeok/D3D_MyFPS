#pragma once
#include"FPS.h"

class CNotice
{
private:
	D3DXVECTOR3 m_Position;

public:
	BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
};

