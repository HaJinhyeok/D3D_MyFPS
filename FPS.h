#pragma once
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9shape.h>
#include <wchar.h>
#include <cmath>
#include <vector>
#include <iterator>

using namespace std;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_UI_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define TRANSPARENCY_COLOR D3DCOLOR_ARGB(0xff,0x00,0x00,0x00)
#define BUTTON_DEFAULT D3DCOLOR_XRGB(128, 128, 128)
#define BUTTON_PRESSED D3DCOLOR_XRGB(64, 64, 64)

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

#define TEXTURE_TILE "tex_tile.bmp"
#define TEXTURE_GRASS "tex_grass.jpg"
#define TEXTURE_WALL "tex_wall.jpg"
#define TEXTURE_NOTICE "tex_question.png"
#define TEXTURE_EXIT "tex_exit.png"
#define TRANSLATION_DISTANCE 0.2f   // 플레이어 이동거리
#define LOOKAT_DISTANCE 5.0f    //
#define ROTATION_AMOUNT D3DX_PI/250 // 플레이어 회전각
// #define NUM_OF_TILE 10
#define NUM_OF_COLUMN 12 // 즉, 가로 길이
#define NUM_OF_ROW 14    // 즉, 세로 길이
#define LENGTH_OF_TILE 10.0f

#define PLAYER_RADIUS 4.0f
#define X_MOVE 1
#define Z_MOVE 0


struct CUSTOMVERTEX
{
    D3DXVECTOR3 v3VerPos;       // 정점의 x, y, z 좌표
    D3DXVECTOR3 v3VerNormal;    // 정점의 수직 벡터
    D3DXVECTOR2 v2VerTex;       // 정점의 텍스처 좌표
};
struct UI_VERTEX
{
    D3DXVECTOR3 v3VerPos;
    FLOAT w;
    D3DCOLOR VerColor;
    D3DXVECTOR2 v2VerTex;
};
enum class POSITION_WITH_FRUSTUM : WORD
{
    outside,
    inside,
    intersection
};
enum class MOVE_DIRECTION :WORD
{
    left = 1,
    right,
    front,
    back
};

static UI_VERTEX UIVertices[4] =
{
    D3DXVECTOR3(10.0f, 10.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(125, 255, 255), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(360.0f, 10.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(125, 255, 255), D3DXVECTOR2(1.0f, 0.0f),
    D3DXVECTOR3(360.0f, 60.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(125, 255, 255), D3DXVECTOR2(1.0f, 1.0f),
    D3DXVECTOR3(10.0f, 60.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(125, 255, 255), D3DXVECTOR2(0.0f, 1.0f)
};

static UI_VERTEX PopUpVertices[4] =
{
    D3DXVECTOR3(100.0f, 150.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(600.0f, 150.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(600.0f, 550.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(100.0f, 550.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f)
};

// 시점 변환 시 
const static D3DXVECTOR3 v3EyeCeiling(0.0f, 200.0f, 0.0f);
const static D3DXVECTOR3 v3UpCeiling(0.0f, 0.0f, 1.0f);

const static char chMap1[NUM_OF_ROW][NUM_OF_COLUMN + 1] = {
    "X   *@ * @* ",
    "*** ** * ** ",
    " @* ** * ** ",
    " ** ** *    ",
    "    ** **** ",
    " **       * ",
    " **     * * ",
    " **  ** * * ",
    " *   ** * * ",
    " * * ** *   ",
    " * * ** * * ",
    " *   ** * * ",
    " * **** *** ",
    "@*@*        "
};

// const static D3DXVECTOR3 v3EyeDefault(0.0f, 5.0f, 0.0f);
// const static D3DXVECTOR3 v3UpDefault(0.0f, 1.0f, 0.0f);

// 시점 변환 확인 변수
static BOOL bIsSkyView = FALSE;
// 컬링 확인 변수
static BOOL bIsFrustumCulling = TRUE;
// 상호작용 확인 변수
static BOOL bIsInteractive = FALSE;
// 게임 종료 확인 변수
static BOOL bIsPlaying = TRUE;
// 버튼 클릭 확인 변수
static BOOL bIsClicked = FALSE;

static D3DXVECTOR3 v3CurrentLookAt(0.0f, 5.0f, 10.0f);
static D3DXVECTOR3 v3LookAt(v3CurrentLookAt);
static D3DXVECTOR3 v3Eye(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 v3Up(0.0f, 1.0f, 0.0f);
static D3DXVECTOR3 v3DefaultPosition(0.0f, 0.0f, 0.0f);

static CUSTOMVERTEX TileVertices[4 * NUM_OF_ROW * NUM_OF_COLUMN];
static CUSTOMVERTEX WallVertices[4][4 * NUM_OF_ROW];
static CUSTOMVERTEX WallVertices2[4][4 * NUM_OF_ROW];

static CUSTOMVERTEX LabyrinthWallVertices[72][20];

static WORD wTileIndices[2 * NUM_OF_ROW * NUM_OF_COLUMN][3];


// tile culling 수정: 정사각형 중심으로부터 거리가 변의 길이의 절반 이하(d <= LENGTH_OF_TILE / 2) culling 해주어야 함.
// 추가할 기능: 플레이어 시점이 qe가 아닌 마우스 움직임에 따라 변하면 좋을 듯?

//// 미로 찾기 게임
// 맵은 한 개, 많아야 두 개(만들기 번거로울 듯?)
// 손전등 기능?(플레이어 위치 기준, 바라보는 방향으로 spot light)

//// 필요한 객체
// <player>
// 플레이어. 위치, 바라보는 방향, 손전등 on/off
// 
// <안내문>
// 미로 막다른 골목에서 상호작용(G) 가능한 객체.
// 상호작용 시, 카메라 시점을 하늘에서 내려다보게 바꾸어 맵을 볼 수 있게 해줌.
// 안내문이 바라보고 있는 방향을 벡터값으로 갖고 있는다.(v3LookAt)
// (플레이어 위치) - (안내문 위치) 벡터(v3Distance) 활용
// ( |v3Distance| <= (특정 거리) && v3LookAt X v3Distance > 0 ) 이면 상호작용 활성화
// 
// <출구> - <안내문>의 child class
// 미로의 끝에서 상호작용(G) 가능한 객체.
// 상호작용 시, 게임 종료?