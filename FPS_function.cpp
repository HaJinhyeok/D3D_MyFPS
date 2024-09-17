#include "FPS_function.h"

// 각 도형이 inside, intersection, outside 중 어떤 상태인지 판별
// viewprojection에 대해 역변환 완료된 6개의 평면, 각 구체의 정점, 구체의 반지름 정보 필요
POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT radius)
{
    // 6개 면에 대해 하나라도 outside이면( distance <= -radius ) outside
    // 6개 면에 대해 하나라도 intersection이면( -radius < distance < radius ) intersection
    // 나머지는 inside
    // 어짜피 intersection도 render해야 하므로, outside인지 아닌지만 검사하면 된다
    // 점과 평면 사이의 거리 공식
    // (distance) = abs( ax_1 + by_1 + cz_1 + d ) / sqrt( a^2 + b^2 + c^2 )
    FLOAT distance;
    for (int i = 0; i < 6; i++)
    {
        distance = 0.0f;
        distance +=
            FrustumPlane[i].a * position.x + FrustumPlane[i].b * position.y +
            FrustumPlane[i].c * position.z + FrustumPlane[i].d;
        distance /= sqrtf(powf(FrustumPlane[i].a, 2.0f) + powf(FrustumPlane[i].b, 2.0f) + powf(FrustumPlane[i].c, 2.0f));
        if (distance < -radius) return POSITION_WITH_FRUSTUM::outside;
        else if (distance >= -radius && distance <= radius) return POSITION_WITH_FRUSTUM::intersection;
    }
    return POSITION_WITH_FRUSTUM::inside;
}

// 몇 번째 맵을 만들건지 주어지면, (맵 외곽을 제외한) 맵 내부를 구성하는 벽들의 vertex 좌표를 생성하고 그 list를 return한다
// 일단 맵은 하나만...
// 맵 내부 구조를 ' '과 '*'로 이루어진 문자열로 표현하고, 이 문자열을 받으면 내부 vertex를 생성할 수 있게 만들면 베스트일듯...
// 벽멱을 전부 vertex 정보로 저장할지, 아니면 한 칸 한 칸의 블록형태로 만들지 미지수
// CUSTOMVERTEX (*MakeLabyrinth(int nMapNumber))[20]
// CUSTOMVERTEX** MakeLabyrinth(int nMapNumber)
VOID MakeLabyrinth(int nMapNumber, CUSTOMVERTEX(*Labyrinth)[20])
{
    int i,j;
    int nBlockNum = 0;
    // 그냥 LabyrinthWallVertices랑 MapNumber를 받아와서 동적 할당 때려버릴까
    if (nMapNumber == 1)
    {
        // CUSTOMVERTEX** Labyrinth = new CUSTOMVERTEX*[72];
        // LabyrinthWallVertices = new CUSTOMVERTEX * [72];
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < NUM_OF_COLUMN; j++)
            {
                if (chMap1[i][j] == '*')
                {
                    MakeWallBlock(Labyrinth[nBlockNum++], D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));
                    //LabyrinthWallVertices[nBlockNum++] = MakeWallBlock(D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));

                    //Labyrinth[nBlockNum++] = MakeWallBlock(D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));
                }
            }
        }

        //return Labyrinth;
    }
    else
        return;
}

// CUSTOMVERTEX* MakeWallBlock(D3DXVECTOR3 position)
VOID MakeWallBlock(CUSTOMVERTEX* Block, D3DXVECTOR3 position)
{
    int i;
    // CUSTOMVERTEX* Block = new CUSTOMVERTEX[20];
    // vector<CUSTOMVERTEX> Block(20);
    // 밑면을 제외한 5개 면의 vertex만 생성해주자
    // 여기서 겹치는 면 vertex는 없애주는 과정이 필요할듯?
    for (i = 0; i < 20; i++)
    {
        if (i % 4 == 0)
            Block[i].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
        else if (i % 4 == 1)
            Block[i].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
        else if (i % 4 == 2)
            Block[i].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
        else
            Block[i].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);

        if (i / 4 == 0)
            Block[i].v3VerNormal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
        else if (i / 4 == 1)
            Block[i].v3VerNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
        else if (i / 4 == 2)
            Block[i].v3VerNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
        else
            Block[i].v3VerNormal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
    }
    Block[0].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[1].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[2].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[3].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    
    Block[4].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[5].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[6].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[7].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    
    Block[8].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[9].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[10].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[11].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    
    Block[12].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[13].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[14].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[15].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y - LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    
    Block[16].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[17].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z + LENGTH_OF_TILE / 2);
    Block[18].v3VerPos = D3DXVECTOR3(position.x + LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    Block[19].v3VerPos = D3DXVECTOR3(position.x - LENGTH_OF_TILE / 2, position.y + LENGTH_OF_TILE / 2, position.z - LENGTH_OF_TILE / 2);
    
    // return Block;
}