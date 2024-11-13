#pragma once
#include "FPS.h"

class CSetting
{
private:
	UI_VERTEX m_Setting_UI_Vertex[4] =
	{
		D3DXVECTOR3(100.0f, 100.0f, 0.0f), 1.0f, D3DCOLOR_RGBA(200,200,200,50), D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR3(600.0f, 100.0f, 0.0f), 1.0f, D3DCOLOR_RGBA(200,200,200,50), D3DXVECTOR2(1.0f, 0.0f),
		D3DXVECTOR3(600.0f, 600.0f, 0.0f), 1.0f, D3DCOLOR_RGBA(200,200,200,50), D3DXVECTOR2(1.0f, 1.0f),
		D3DXVECTOR3(100.0f, 600.0f, 0.0f), 1.0f, D3DCOLOR_RGBA(200,200,200,50), D3DXVECTOR2(0.0f, 1.0f)
	};

public:
	CSetting();
	~CSetting();
	VOID DrawSetting(LPDIRECT3DDEVICE9 device);
};

