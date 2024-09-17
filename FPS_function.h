#pragma once
#include "FPS.h"

POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT distance);

// 2차원 배열 리턴하는 방법
// CUSTOMVERTEX (*MakeLabyrinth(int nMapNumber))[20];
// CUSTOMVERTEX** MakeLabyrinth(int nMapNumber);
VOID MakeLabyrinth(int nMapNumber, CUSTOMVERTEX (*Labyrinth)[20]);
// CUSTOMVERTEX* MakeWallBlock(D3DXVECTOR3 position);
VOID MakeWallBlock(CUSTOMVERTEX* Block, D3DXVECTOR3 position);