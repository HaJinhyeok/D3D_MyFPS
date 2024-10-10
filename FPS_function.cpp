#include "FPS_function.h"

// �� ������ inside, intersection, outside �� � �������� �Ǻ�
// viewprojection�� ���� ����ȯ �Ϸ�� 6���� ���, �� ��ü�� ����, ��ü�� ������ ���� �ʿ�
POSITION_WITH_FRUSTUM CheckFrustumCulling(D3DXPLANE* FrustumPlane, D3DXVECTOR3 position, FLOAT radius)
{
    // 6�� �鿡 ���� �ϳ��� outside�̸�( distance <= -radius ) outside
    // 6�� �鿡 ���� �ϳ��� intersection�̸�( -radius < distance < radius ) intersection
    // �������� inside
    // ��¥�� intersection�� render�ؾ� �ϹǷ�, outside���� �ƴ����� �˻��ϸ� �ȴ�
    // ���� ��� ������ �Ÿ� ����
    // (distance) = abs( ax_1 + by_1 + cz_1 + d ) / sqrt( a^2 + b^2 + c^2 )
    FLOAT distance;
    for (int i = 0; i < 6; i++)
    // int i = 2;
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
D3DXVECTOR3 CalculateMidPoint(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
    return D3DXVECTOR3((pos1 + pos2).x / 2, (pos1 + pos2).y / 2, (pos1 + pos2).z / 2);
}

// �� ��° ���� ������� �־�����, (�� �ܰ��� ������) �� ���θ� �����ϴ� ������ vertex ��ǥ�� �����ϰ� �� list�� return�Ѵ�
// �ϴ� ���� �ϳ���...
// �� ���� ������ ' '�� '*'�� �̷���� ���ڿ��� ǥ���ϰ�, �� ���ڿ��� ������ ���� vertex�� ������ �� �ְ� ����� ����Ʈ�ϵ�...
// ������ ���� vertex ������ ��������, �ƴϸ� �� ĭ �� ĭ�� ������·� ������ ������
// CUSTOMVERTEX (*MakeLabyrinth(int nMapNumber))[20]
// CUSTOMVERTEX** MakeLabyrinth(int nMapNumber)
VOID MakeLabyrinth(int nMapNumber, CUSTOMVERTEX(*Labyrinth)[20])
{
    int i,j;
    int nBlockNum = 0;
    // �׳� LabyrinthWallVertices�� MapNumber�� �޾ƿͼ� ���� �Ҵ� ����������
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
    
    // return Block;
}

// PLAYER MOVE
// �ٲپ�� �� ��: �÷��̾� ��ġ, �÷��̾� LookAt
// �ʿ��� ��: �÷��̾ ������ ���� ����, �Ÿ�, �ÿ��̾� �� ��ġ, �÷��̾� �� LookAt, �� ����

VOID PlayerMove(CPlayer* player, MOVE_DIRECTION direction, FLOAT distance, const char (*map)[NUM_OF_COLUMN+1])
{
    D3DXVECTOR3 vecDirection, tmpPosition; // ���� �������� �ʰ� �̵��� ��ġ. �ֺ� 8�� ���� �̰��� ������ ���� ��ġ ����
    D3DXVECTOR3 vecPosition = player->GetPosition(), vecLookAt = player->GetLookAt();
    D3DXVECTOR2 WallPoint[2]; // �浹�� �˻��� ����� ���ʾƷ�(minX, minZ)�� ��������(maxX,maxZ) �� ��
    FLOAT fCoefficient = distance;
    int i, j, nCoX, nCoZ;
    // ���� ��ǥ
    nCoX = (int)(vecPosition.x / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
    nCoZ = NUM_OF_ROW / 2 - (int)(vecPosition.z / LENGTH_OF_TILE);

    if (direction == MOVE_DIRECTION::left)
    {
        vecDirection.x = -player->GetPlayerAxis()._11;
        vecDirection.y = -player->GetPlayerAxis()._12;
        vecDirection.z = -player->GetPlayerAxis()._13;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = vecPosition.x + vecDirection.x * fCoefficient;
        tmpPosition.y = vecPosition.y + vecDirection.y * fCoefficient;
        tmpPosition.z = vecPosition.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::right)
    {
        vecDirection.x = player->GetPlayerAxis()._11;
        vecDirection.y = player->GetPlayerAxis()._12;
        vecDirection.z = player->GetPlayerAxis()._13;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = vecPosition.x + vecDirection.x * fCoefficient;
        tmpPosition.y = vecPosition.y + vecDirection.y * fCoefficient;
        tmpPosition.z = vecPosition.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::front)
    {
        vecDirection.x = player->GetPlayerAxis()._31;
        vecDirection.y = player->GetPlayerAxis()._32;
        vecDirection.z = player->GetPlayerAxis()._33;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = vecPosition.x + vecDirection.x * fCoefficient;
        tmpPosition.y = vecPosition.y + vecDirection.y * fCoefficient;
        tmpPosition.z = vecPosition.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::back)
    {
        vecDirection.x = -player->GetPlayerAxis()._31;
        vecDirection.y = -player->GetPlayerAxis()._32;
        vecDirection.z = -player->GetPlayerAxis()._33;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = vecPosition.x + vecDirection.x * fCoefficient;
        tmpPosition.y = vecPosition.y + vecDirection.y * fCoefficient;
        tmpPosition.z = vecPosition.z + vecDirection.z * fCoefficient;

    }
    // �ܰ� ���� �ε����� ��� �߰�, ��� ������ ��� ������ �������� ���� �����ϱ�
    // x�� ���� �������� �̵��� ���
    if (vecDirection.x < 0)
    {
        for (i = 0; i < 3; i++)
        {
            // ���� ��ġ ���� ���� 3�� ��Ͽ� ���� �˻�
            // ���� �ܰ� ���� ��� ���� �˻�
            //if()
            if (map[nCoZ - 1 + i][nCoX - 1] == '*')
            {
                WallPoint[0].x = (nCoX - 1 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i)) * LENGTH_OF_TILE;

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
                    OutputDebugStringA("Left Collision!!!\n");
                    tmpPosition.x = WallPoint[1].x + PLAYER_RADIUS + 0.1f;
                    break;
                }
            }
        }
    }
    // x�� ���� �������� �̵��� ���
    else if (vecDirection.x > 0)
    {
        for (i = 0; i < 3; i++)
        {
            // ���� ��ġ ���� ������ 3�� ��Ͽ� ���� �˻�
            // ���� �ܰ� ���� ��� ���� �˻�
            //if()
            if (map[nCoZ - 1 + i][nCoX + 1] == '*')
            {
                WallPoint[0].x = (nCoX + 1 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + 2 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i)) * LENGTH_OF_TILE;

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
                    OutputDebugStringA("Right Collision!!!\n");
                    tmpPosition.x = WallPoint[0].x - PLAYER_RADIUS - 0.1f;
                    break;
                }
            }
        }
    }

    // z�� ���� �������� �̵��� ���
    if (vecDirection.z < 0)
    {
        for (i = 0; i < 3; i++)
        {
            // ���� ��ġ ���� �Ʒ��� 3�� ��Ͽ� ���� �˻�
            // ���� �ܰ� ���� ��� ���� �˻�
            //if()
            if (map[nCoZ + 1][nCoX - 1 + i] == '*')
            {
                WallPoint[0].x = (nCoX - 1 + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ + 1) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ + 1)) * LENGTH_OF_TILE;

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
                    OutputDebugStringA("Back Collision!!!\n");
                    tmpPosition.z = WallPoint[1].y + PLAYER_RADIUS + 0.1f;
                    break;
                }
            }
        }
    }
    // z�� ���� �������� �̵��� ���
    else if (vecDirection.z > 0)
    {
        for (i = 0; i < 3; i++)
        {
            // ���� ��ġ ���� ���� 3�� ��Ͽ� ���� �˻�
            // ���� �ܰ� ���� ��� ���� �˻�
            //if()
            if (map[nCoZ - 1][nCoX - 1 + i] == '*')
            {
                WallPoint[0].x = (nCoX - 1 + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1)) * LENGTH_OF_TILE;

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
                    OutputDebugStringA("Front Collision!!!\n");
                    tmpPosition.z = WallPoint[0].y - PLAYER_RADIUS - 0.1f;
                    break;
                }
            }
        }
    }
    // LookAt�� Position�� �̵��� ��ŭ�� ���ϰų� ���ָ� ��
    vecLookAt.x += tmpPosition.x - vecPosition.x;
    vecLookAt.z += tmpPosition.z - vecPosition.z;
    player->SetPosition(tmpPosition);
    player->SetLookAt(vecLookAt);
}
BOOL CollisionCheck(char** map, D3DXVECTOR3 CharacterPositon, FLOAT distance, BOOL direction)
{
    // character�� position�� �־�����, xz ������� �̷���� map �󿡼� � ĭ�� ��ġ�� �ִ��� �� �� �ִ�.
    // �׷��� �� ���� 8ĭ�� ���ؼ��� üũ�� �ϸ�...? �����ʳ�
    // x�� �̵����� z�� �̵������� �Ű������� �޾Ƽ� Ȯ��
    int nCoX, nCoZ, i, j;
    FLOAT fCurrentX = CharacterPositon.x, fCurrentZ = CharacterPositon.z;
    if (direction == X_MOVE)
    {
        nCoX = (int)(fCurrentX / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
        for (i = -1; i < 2; i++)
        {
            if (i == 0) continue;
            // �ܰ� ������ �浹 �˻�
            if (nCoX + i < 0)
            {

            }
            else if (nCoX + i >= NUM_OF_COLUMN)
            {

            }
        }
    }
    else
    {
        nCoZ = (int)(fCurrentZ / LENGTH_OF_TILE) - NUM_OF_ROW / 2 + 1;

    }
    for (i = -1; i < 2; i++)
    {
        for (j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0) continue;
            // �ܰ� ������ �浹 �˻�
            if (nCoX + i < 0)
            {

            }
            else if (nCoX + i >= NUM_OF_COLUMN)
            {

            }
            if (nCoZ + i < 0)
            {

            }
            else if (nCoZ + i >= NUM_OF_ROW)
            {

            }
            // '*' ǥ�õ� ��� ĭ���� �浹 �˻�

        }
    }

    // �浹�ϸ� TRUE
    return TRUE;
}