#include "maze_function.h"

D3DXVECTOR3 CalculateMidPoint(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
    return D3DXVECTOR3((pos1 + pos2).x / 2, (pos1 + pos2).y / 2, (pos1 + pos2).z / 2);
}
vector<D3DXVECTOR3> CalculateDivisionPoints(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int div)
{
    vector<D3DXVECTOR3> points;
    for (int i = 1;i <= div - 1;i++)
    {
        points.push_back(D3DXVECTOR3((pos1.x * i + pos2.x * (div - 1)) / div, (pos1.y * i + pos2.y * (div - 1)) / div, (pos1.z * i + pos2.z * (div - 1)) / div));
    }
    return points;
}

// �� ��° ���� ������� �־�����, (�� �ܰ��� ������) �� ���θ� �����ϴ� ������ vertex ��ǥ�� ����
// �ϴ� ���� �ϳ���...
// �� ���� ������ ' '�� '*'�� �̷���� ���ڿ��� ǥ���ϰ�, �� ���ڿ��� ������ ���� vertex�� ������ �� �ְ� ����� ����Ʈ�ϵ�...
// ������ ���� vertex ������ ��������, �ƴϸ� �� ĭ �� ĭ�� ������·� ������ ������
VOID GenerateMazeWall(int nMapNumber, CUSTOMVERTEX(*Maze)[20], vector<CNotice>* notice, CExit* Exit)
{
    int i,j;
    int nBlockNum = 0, nNoticeNum = 0;
    // ���� ���� �ϳ� ����
    if (nMapNumber == 1)
    {
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < NUM_OF_COLUMN; j++)
            {
                if (chMap1[i][j] == '*')
                {
                    MakeWallBlock(Maze[nBlockNum++], D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));
                }
                else if (chMap1[i][j] == '@')
                {
                    CNotice tmpNotice;
                    tmpNotice.MakeNotice(D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));
                    notice->push_back(tmpNotice);
                }
                // Ż�ⱸ�� ��� �ʸ��� �� �ϳ��� ����
                else if (chMap1[i][j] == 'X')
                {
                    Exit->MakeExit(D3DXVECTOR3((-NUM_OF_COLUMN / 2 + j + 0.5f) * LENGTH_OF_TILE, 5.0f, (NUM_OF_ROW / 2 - i - 0.5f) * LENGTH_OF_TILE));
                }
            }
        }
    }
    else
        return;
}

VOID MakeWallBlock(CUSTOMVERTEX* Block, D3DXVECTOR3 position)
{
    int i;
    // �ظ��� ������ 5�� ���� vertex�� ����������
    // ���⼭ ��ġ�� �� vertex�� �����ִ� ������ �ʿ��ҵ�?
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
}
