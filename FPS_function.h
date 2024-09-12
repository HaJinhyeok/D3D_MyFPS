#pragma once
#include "FPS.h"

POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT distance);

CUSTOMVERTEX* MakeLabyrinth(int nMapNumber);