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
    D3DXVECTOR3 v3VerPos;       // ������ x, y, z ��ǥ
    D3DXVECTOR3 v3VerNormal;    // ������ ���� ����
    D3DXVECTOR2 v2VerTex;       // ������ �ؽ�ó ��ǥ
};
enum class POSITION_WITH_FRUSTUM : WORD
{
    outside,
    inside,
    intersection
};

// ���� ��ȯ �� 
const static D3DXVECTOR3 v3EyeCeiling(0.0f, 200.0f, 0.0f);
const static D3DXVECTOR3 v3EyeDefault(0.0f, 5.0f, 0.0f);
const static D3DXVECTOR3 v3UpCeiling(0.0f, 0.0f, 1.0f);
const static D3DXVECTOR3 v3UpDefault(0.0f, 1.0f, 0.0f);

// ���� ��ȯ Ȯ�� ����
static BOOL bIsSkyView = FALSE;

static D3DXVECTOR3 v3CurrentLookAt(0.0f, 5.0f, 10.0f);
static D3DXVECTOR3 v3Eye(v3EyeDefault);
static D3DXVECTOR3 v3LookAt(v3CurrentLookAt);
static D3DXVECTOR3 v3Up(0.0f, 1.0f, 0.0f);
static D3DXVECTOR3 v3PlayerPosition(0.0f, 0.0f, 0.0f);

static CUSTOMVERTEX TileVertices[4 * NUM_OF_ROW * NUM_OF_COLUMN];
static WORD wTileIndices[2 * NUM_OF_ROW * NUM_OF_COLUMN][3];

// ���� position, look at  ���ڷ� ȭ�鿡 ����� Ȯ���غ���;;

//// �̷� ã�� ����
// ���� �� ��, ���ƾ� �� ��(����� ���ŷο� ��?)
// ������ ���?(�÷��̾� ��ġ ����, �ٶ󺸴� �������� spot light)

//// �ʿ��� ��ü
// <player>
// �÷��̾�. ��ġ, �ٶ󺸴� ����, ������ on/off
// <�ȳ���>
// �̷� ���ٸ� ��񿡼� ��ȣ�ۿ�(G) ������ ��ü.
// ��ȣ�ۿ� ��, ī�޶� ������ �ϴÿ��� �����ٺ��� �ٲپ� ���� �� �� �ְ� ����.
// 
// <�ⱸ>
// �̷��� ������ ��ȣ�ۿ�(G) ������ ��ü.
// ��ȣ�ۿ� ��, ���� ����?