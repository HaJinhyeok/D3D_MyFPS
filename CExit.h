#pragma once
#include "CNotice.h"

class CExit :
    public CNotice
{
private:
    UI_VERTEX m_ExitButtonVertices[4] =
    {
        D3DXVECTOR3(300.0f, 450.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(128, 128, 128), D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR3(400.0f, 450.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(128, 128, 128), D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR3(400.0f, 500.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(128, 128, 128), D3DXVECTOR2(0.0f, 0.0f),
        D3DXVECTOR3(300.0f, 500.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(128, 128, 128), D3DXVECTOR2(0.0f, 0.0f),

    };

public:
    CExit();
    ~CExit();

    VOID MakeExit(D3DXVECTOR3 position);
    VOID DrawExitButton(LPDIRECT3DDEVICE9 device);
};

