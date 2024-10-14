#pragma once
#include "CNotice.h"
class CExit :
    public CNotice
{
public:
    CExit(D3DXVECTOR3 position);
    ~CExit();
    BOOL IsPossibleInteraction(D3DXVECTOR3 playerPosition);
};

