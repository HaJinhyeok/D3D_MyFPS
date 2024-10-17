#pragma once
#include "CNotice.h"

class CExit :
    public CNotice
{
public:
    CExit();
    ~CExit();

    VOID MakeExit(D3DXVECTOR3 position);
};

