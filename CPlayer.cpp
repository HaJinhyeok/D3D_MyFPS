#include "CPlayer.h"
CPlayer::CPlayer()
{
	D3DXMATRIX tmpMatrix;
	m_Position = D3DXVECTOR3(55.0f, 5.0f, -65.0f); // v3EyeDefault
	m_LookAt = m_Position;
	m_LookAt.z += LOOKAT_DISTANCE;
	D3DXMatrixIdentity(&m_PlayerWorld);
	D3DXMatrixTranslation(&tmpMatrix, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixMultiply(&m_PlayerWorld, &m_PlayerWorld, &tmpMatrix);
	D3DXMatrixIdentity(&m_PlayerAxis);
	m_Flashlight = TRUE;
}
CPlayer::~CPlayer()
{

}

// PLAYER MOVE
// 바꾸어야 할 것: 플레이어 위치, 플레이어 LookAt
// 필요한 것: 플레이어가 움직일 방향 벡터, 거리, 플에이어 현 위치, 플레이어 현 LookAt, 맵 정보
VOID CPlayer::Move(MOVE_DIRECTION direction, const char(*map)[NUM_OF_COLUMN + 1])
{
    D3DXVECTOR3 vecDirection, tmpPosition; // 벽을 생각하지 않고 이동된 위치. 주변 8개 벽과 이것을 대조해 최종 위치 결정
    D3DXVECTOR2 WallPoint[2]; // 충돌을 검사할 블록의 왼쪽아래(minX, minZ)와 오른쪽위(maxX,maxZ) 두 점
    FLOAT fCoefficient = TRANSLATION_DISTANCE;
    int i, nCoX, nCoZ;

    // 현재 좌표
    nCoX = floorf(m_Position.x / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
    nCoZ = NUM_OF_ROW / 2 - floorf(m_Position.z / LENGTH_OF_TILE) - 1;

    if (direction == MOVE_DIRECTION::left)
    {
        vecDirection.x = -m_PlayerAxis._11;
        vecDirection.y = -m_PlayerAxis._12;
        vecDirection.z = -m_PlayerAxis._13;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = m_Position.x + vecDirection.x * fCoefficient;
        tmpPosition.y = m_Position.y + vecDirection.y * fCoefficient;
        tmpPosition.z = m_Position.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::right)
    {
        vecDirection.x = m_PlayerAxis._11;
        vecDirection.y = m_PlayerAxis._12;
        vecDirection.z = m_PlayerAxis._13;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = m_Position.x + vecDirection.x * fCoefficient;
        tmpPosition.y = m_Position.y + vecDirection.y * fCoefficient;
        tmpPosition.z = m_Position.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::front)
    {
        vecDirection.x = m_PlayerAxis._31;
        vecDirection.y = m_PlayerAxis._32;
        vecDirection.z = m_PlayerAxis._33;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = m_Position.x + vecDirection.x * fCoefficient;
        tmpPosition.y = m_Position.y + vecDirection.y * fCoefficient;
        tmpPosition.z = m_Position.z + vecDirection.z * fCoefficient;
    }
    else if (direction == MOVE_DIRECTION::back)
    {
        vecDirection.x = -m_PlayerAxis._31;
        vecDirection.y = -m_PlayerAxis._32;
        vecDirection.z = -m_PlayerAxis._33;
        fCoefficient /= sqrtf(vecDirection.x * vecDirection.x + vecDirection.y * vecDirection.y + vecDirection.z * vecDirection.z);

        tmpPosition.x = m_Position.x + vecDirection.x * fCoefficient;
        tmpPosition.y = m_Position.y + vecDirection.y * fCoefficient;
        tmpPosition.z = m_Position.z + vecDirection.z * fCoefficient;

    }
    // 해결: 외곽 벽에 부딪히는 경우 추가, 몇몇 곳에서 블록 속으로 들어가버리는 버그 수정하기
    // x축 음의 방향으로 이동일 경우
    if (vecDirection.x < 0)
    {
        for (i = 0; i < 3; i++)
        {
            // 현재 위치 기준 왼쪽 3개 블록에 대해 검사
            // 제일 외곽 벽일 경우 따로 검사
            if (nCoX == 0)
            {
                if (tmpPosition.x - PLAYER_RADIUS <= -NUM_OF_COLUMN / 2 * LENGTH_OF_TILE)
                {
                    tmpPosition.x = -NUM_OF_COLUMN / 2 * LENGTH_OF_TILE + PLAYER_RADIUS;
                }
            }
            else if (map[nCoZ - 1 + i][nCoX - 1] == '*')
            {
                WallPoint[0].x = (nCoX - 1 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i)) * LENGTH_OF_TILE;

                //충돌 시
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
    // x축 양의 방향으로 이동일 경우
    else if (vecDirection.x > 0)
    {
        for (i = 0; i < 3; i++)
        {
            // 현재 위치 기준 오른쪽 3개 블록에 대해 검사
            // 제일 외곽 벽일 경우 따로 검사
            if (nCoX == NUM_OF_COLUMN - 1)
            {
                if (tmpPosition.x + PLAYER_RADIUS >= NUM_OF_COLUMN / 2 * LENGTH_OF_TILE)
                {
                    tmpPosition.x = NUM_OF_COLUMN / 2 * LENGTH_OF_TILE - PLAYER_RADIUS;
                }
            }
            else if (map[nCoZ - 1 + i][nCoX + 1] == '*')
            {
                WallPoint[0].x = (nCoX + 1 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + 2 - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1 + i)) * LENGTH_OF_TILE;

                //충돌 시
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

    // z축 음의 방향으로 이동일 경우
    if (vecDirection.z < 0)
    {
        for (i = 0; i < 3; i++)
        {
            // 현재 위치 기준 아래쪽 3개 블록에 대해 검사
            // 제일 외곽 벽일 경우 따로 검사
            if (nCoZ == NUM_OF_ROW - 1)
            {
                if (tmpPosition.z - PLAYER_RADIUS <= -NUM_OF_ROW / 2 * LENGTH_OF_TILE)
                {
                    tmpPosition.z = -NUM_OF_ROW / 2 * LENGTH_OF_TILE + PLAYER_RADIUS;
                }
            }
            else if (map[nCoZ + 1][nCoX - 1 + i] == '*')
            {
                WallPoint[0].x = (nCoX - 1 + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ + 1) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ + 1)) * LENGTH_OF_TILE;

                //충돌 시
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
    // z축 양의 방향으로 이동일 경우
    else if (vecDirection.z > 0)
    {
        for (i = 0; i < 3; i++)
        {
            // 현재 위치 기준 위쪽 3개 블록에 대해 검사
            // 제일 외곽 벽일 경우 따로 검사
            if (nCoZ == 0)
            {
                if (tmpPosition.z + PLAYER_RADIUS >= NUM_OF_ROW / 2 * LENGTH_OF_TILE)
                {
                    tmpPosition.z = NUM_OF_ROW / 2 * LENGTH_OF_TILE - PLAYER_RADIUS;
                }
            }
            else if (map[nCoZ - 1][nCoX - 1 + i] == '*')
            {
                WallPoint[0].x = (nCoX - 1 + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[0].y = (NUM_OF_ROW / 2 - (nCoZ - 1) - 1) * LENGTH_OF_TILE;
                WallPoint[1].x = (nCoX + i - NUM_OF_COLUMN / 2) * LENGTH_OF_TILE;
                WallPoint[1].y = (NUM_OF_ROW / 2 - (nCoZ - 1)) * LENGTH_OF_TILE;

                //충돌 시
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
    // LookAt은 Position이 이동한 만큼만 더하거나 빼주면 됨
    m_LookAt.x += tmpPosition.x - m_Position.x;
    m_LookAt.z += tmpPosition.z - m_Position.z;
    SetPosition(tmpPosition);
}

VOID CPlayer::Rotate(BOOL bIsCCW)
{
	// bIsCCW로 q인지 e인지 구분하고, angle만큼 회전을 하며, LookAt과 Position 사이 간격은 distance
	D3DXMATRIX mtRotation;
	FLOAT fCoefficient = LOOKAT_DISTANCE;

	if (bIsCCW == TRUE)
	{
		D3DXMatrixRotationY(&mtRotation, -ROTATION_AMOUNT);
	}
	else
	{
		D3DXMatrixRotationY(&mtRotation, ROTATION_AMOUNT);
	}
	D3DXMatrixMultiply(&m_PlayerAxis, &m_PlayerAxis, &mtRotation);
	
	fCoefficient /= sqrtf(m_PlayerAxis._31 * m_PlayerAxis._31 + m_PlayerAxis._32 * m_PlayerAxis._32 + m_PlayerAxis._33 * m_PlayerAxis._33);
	m_LookAt.x = m_Position.x + m_PlayerAxis._31 * fCoefficient;
	m_LookAt.y = m_Position.y + m_PlayerAxis._32 * fCoefficient;
	m_LookAt.z = m_Position.z + m_PlayerAxis._33 * fCoefficient;
}