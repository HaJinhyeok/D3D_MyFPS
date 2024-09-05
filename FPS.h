#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define TILE_TEXTURE_NAME "tex_tile.bmp"
#define TEXTURE_GRASS "tex_grass.jpg"
#define TEXTURE_WALL "tex_wall.jpg"
#define TRANSLATION_DISTANCE 1.0f
#define ROTATION_AMOUNT D3DX_PI/90
// #define NUM_OF_TILE 10
#define NUM_OF_COLUMN 12
#define NUM_OF_ROW 14
#define LENGTH_OF_TILE 10


struct CUSTOMVERTEX
{
    D3DXVECTOR3 v3VerPos;       // 정점의 x, y, z 좌표
    D3DXVECTOR3 v3VerNormal;    // 정점의 수직 벡터
    D3DXVECTOR2 v2VerTex;       // 정점의 텍스처 좌표
};
enum class POSITION_WITH_FRUSTUM : WORD
{
    outside,
    inside,
    intersection
};

// 시점 변환 시 
const static D3DXVECTOR3 v3EyeCeiling(0.0f, 200.0f, 0.0f);
const static D3DXVECTOR3 v3EyeDefault(0.0f, 5.0f, 0.0f);
const static D3DXVECTOR3 v3UpCeiling(0.0f, 0.0f, 1.0f);
const static D3DXVECTOR3 v3UpDefault(0.0f, 1.0f, 0.0f);

// 시점 변환 확인 변수
static BOOL bIsSkyView = FALSE;

static D3DXVECTOR3 v3CurrentLookAt(0.0f, 5.0f, 10.0f);
static D3DXVECTOR3 v3Eye(v3EyeDefault);
static D3DXVECTOR3 v3LookAt(v3CurrentLookAt);
static D3DXVECTOR3 v3Up(0.0f, 1.0f, 0.0f);
static D3DXVECTOR3 v3PlayerPosition(0.0f, 0.0f, 0.0f);

static CUSTOMVERTEX TileVertices[4 * NUM_OF_ROW * NUM_OF_COLUMN];
static WORD wTileIndices[2 * NUM_OF_ROW * NUM_OF_COLUMN][3];

// 현재 position, look at  숫자로 화면에 띄워서 확인해보기;;

//// 미로 찾기 게임
// 맵은 한 개, 많아야 두 개(만들기 번거로울 듯?)
// 손전등 기능?(플레이어 위치 기준, 바라보는 방향으로 spot light)

//// 필요한 객체
// <player>
// 플레이어. 위치, 바라보는 방향, 손전등 on/off
// <안내문>
// 미로 막다른 골목에서 상호작용(G) 가능한 객체.
// 상호작용 시, 카메라 시점을 하늘에서 내려다보게 바꾸어 맵을 볼 수 있게 해줌.
// 
// <출구>
// 미로의 끝에서 상호작용(G) 가능한 객체.
// 상호작용 시, 게임 종료?