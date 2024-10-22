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
#define TRANSLATION_DISTANCE 0.5f   // �÷��̾� �̵��Ÿ�
#define LOOKAT_DISTANCE 5.0f    //
#define ROTATION_AMOUNT D3DX_PI/150 // �÷��̾� ȸ����
// #define NUM_OF_TILE 10
#define NUM_OF_COLUMN 12 // ��, ���� ����
#define NUM_OF_ROW 14    // ��, ���� ����
#define LENGTH_OF_TILE 10.0f

#define PLAYER_RADIUS 3.0f
#define X_MOVE 1
#define Z_MOVE 0


struct CUSTOMVERTEX
{
    D3DXVECTOR3 v3VerPos;       // ������ x, y, z ��ǥ
    D3DXVECTOR3 v3VerNormal;    // ������ ���� ����
    D3DXVECTOR2 v2VerTex;       // ������ �ؽ�ó ��ǥ
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
    D3DXVECTOR3(10.0f, 10.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(255, 0, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(130.0f, 10.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 0, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(130.0f, 50.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 0, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(10.0f, 50.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(255, 0, 0), D3DXVECTOR2(0.0f, 0.0f)
};

static UI_VERTEX PopUpVertices[4] =
{
    D3DXVECTOR3(100.0f, 150.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(600.0f, 150.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(600.0f, 550.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f),
    D3DXVECTOR3(100.0f, 550.0f, 0.0f), 1.0f, D3DCOLOR_XRGB(0, 255, 0), D3DXVECTOR2(0.0f, 0.0f)
};

// ���� ��ȯ �� 
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

// ���� ��ȯ Ȯ�� ����
static BOOL bIsSkyView = FALSE;
// �ø� Ȯ�� ����
static BOOL bIsFrustumCulling = TRUE;
// ��ȣ�ۿ� Ȯ�� ����
static BOOL bIsInteractive = FALSE;
// ���� ���� Ȯ�� ����
static BOOL bIsPlaying = TRUE;

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


// tile culling ����: ���簢�� �߽����κ��� �Ÿ��� ���� ������ ���� ����(d <= LENGTH_OF_TILE / 2) culling ���־�� ��.
// �߰��� ���: �÷��̾� ������ qe�� �ƴ� ���콺 �����ӿ� ���� ���ϸ� ���� ��?

//// �̷� ã�� ����
// ���� �� ��, ���ƾ� �� ��(����� ���ŷο� ��?)
// ������ ���?(�÷��̾� ��ġ ����, �ٶ󺸴� �������� spot light)

//// �ʿ��� ��ü
// <player>
// �÷��̾�. ��ġ, �ٶ󺸴� ����, ������ on/off
// 
// <�ȳ���>
// �̷� ���ٸ� ��񿡼� ��ȣ�ۿ�(G) ������ ��ü.
// ��ȣ�ۿ� ��, ī�޶� ������ �ϴÿ��� �����ٺ��� �ٲپ� ���� �� �� �ְ� ����.
// �ȳ����� �ٶ󺸰� �ִ� ������ ���Ͱ����� ���� �ִ´�.(v3LookAt)
// (�÷��̾� ��ġ) - (�ȳ��� ��ġ) ����(v3Distance) Ȱ��
// ( |v3Distance| <= (Ư�� �Ÿ�) && v3LookAt X v3Distance > 0 ) �̸� ��ȣ�ۿ� Ȱ��ȭ
// 
// <�ⱸ> - <�ȳ���>�� child class
// �̷��� ������ ��ȣ�ۿ�(G) ������ ��ü.
// ��ȣ�ۿ� ��, ���� ����?