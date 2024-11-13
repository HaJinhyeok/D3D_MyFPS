#pragma once
#include "CPlayer.h"
#include "CExit.h"
#include "CSetting.h"

POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT distance);
D3DXVECTOR3 CalculateMidPoint(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
// 2차원 배열 리턴하는 방법
// CUSTOMVERTEX (*MakeLabyrinth(int nMapNumber))[20];
// CUSTOMVERTEX** MakeLabyrinth(int nMapNumber);
VOID MakeLabyrinth(int nMapNumber, CUSTOMVERTEX (*Labyrinth)[20], vector<CNotice>* notice, CExit* exit);
// CUSTOMVERTEX* MakeWallBlock(D3DXVECTOR3 position);
VOID MakeWallBlock(CUSTOMVERTEX* Block, D3DXVECTOR3 position);