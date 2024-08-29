#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define TILE_TEXTURE_NAME "tex_tile.bmp"
#define TRANSLATION_DISTANCE 1.0f
#define ROTATION_AMOUNT D3DX_PI/90
#define NUM_OF_TILE 10
#define LENGTH_OF_TILE 10


struct CUSTOMVERTEX
{
    D3DXVECTOR3 v3VerPos;       // 정점의 x, y, z 좌표
    D3DXVECTOR3 v3VerNormal;    // 정점의 수직 벡터
    D3DXVECTOR2 v2VerTex;       // 정점의 텍스처 좌표
};
enum POSITION_WITH_FRUSTUM
{
    outside = 0,
    inside = 1,
    intersection = 2
};

const static D3DXVECTOR3 v3EyeCeiling(0.0f, 200.0f, 0.0f);
const static D3DXVECTOR3 v3EyeDefault(0.0f, 10.0f, 0.0f);
const static D3DXVECTOR3 v3UpCeiling(0.0f, 0.0f, 1.0f);
const static D3DXVECTOR3 v3UpDefault(0.0f, 1.0f, 0.0f);

// 시점 변환 확인 변수
static BOOL bIsSkyView = FALSE;

static D3DXVECTOR3 v3CurrentLookAt(0.0f, 5.0f, 10.0f);
static D3DXVECTOR3 v3Eye(v3EyeDefault);
static D3DXVECTOR3 v3LookAt(v3CurrentLookAt);
static D3DXVECTOR3 v3Up(0.0f, 1.0f, 0.0f);

static CUSTOMVERTEX TileVertices[4 * NUM_OF_TILE * NUM_OF_TILE];

// 만들어야 할? 객체: billboard(나무 혹은 악당 texture), 발사체(총알 같은?st)