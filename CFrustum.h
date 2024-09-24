#pragma once
#include "FPS.h"
class CFrustum
{
private:
	D3DXVECTOR3 m_vertex[8];
	D3DXPLANE m_plane[6];

public:
	CFrustum();
	~CFrustum();
	VOID MakeFrustum(D3DXMATRIX* pMatViewProj);
	BOOL bIsInFrustum(D3DXVECTOR3* position, FLOAT distance);
};

