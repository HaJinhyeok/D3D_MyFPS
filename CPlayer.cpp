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

    //// light setting
    m_IsLightOn = TRUE;
    ZeroMemory(&m_FlashLight, sizeof(D3DLIGHT9));
    m_FlashLight.Type = D3DLIGHT_SPOT;
    m_FlashLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_FlashLight.Ambient.r = 1.0f;
    m_FlashLight.Ambient.g = 1.0f;
    m_FlashLight.Ambient.b = 1.0f;
    m_FlashLight.Specular.r = 1.0f;
    m_FlashLight.Specular.g = 1.0f;
    m_FlashLight.Specular.b = 1.0f;
    m_FlashLight.Position = m_Position;
    m_FlashLight.Direction = D3DXVECTOR3(m_PlayerAxis._31, m_PlayerAxis._32, m_PlayerAxis._33);
    D3DXVec3Normalize((D3DXVECTOR3*)&m_FlashLight.Direction, (D3DXVECTOR3*)&m_FlashLight.Direction);
    m_FlashLight.Range = 1500.0f;
    m_FlashLight.Attenuation0 = 1.0f;
    m_FlashLight.Attenuation1 = 0.01f;
    m_FlashLight.Attenuation2 = 0.0001f;
    m_FlashLight.Falloff = 2.0f;
    m_FlashLight.Phi = D3DXToRadian(60.0f);
    m_FlashLight.Theta = D3DXToRadian(30.0f);
    //m_FlashLight.Phi = D3DXToRadian(100.0f);
    //m_FlashLight.Theta = D3DXToRadian(100.0f);
    //m_FlashLight.Phi = 1.0f;
    //m_FlashLight.Theta = 0.7f;
}
CPlayer::~CPlayer()
{

}

// PLAYER MOVE
// �ٲپ�� �� ��: �÷��̾� ��ġ, �÷��̾� LookAt
// �ʿ��� ��: �÷��̾ ������ ���� ����, �Ÿ�, �ÿ��̾� �� ��ġ, �÷��̾� �� LookAt, �� ����
VOID CPlayer::Move(MOVE_DIRECTION direction, const char(*map)[NUM_OF_COLUMN + 1])
{
    D3DXVECTOR3 vecDirection, tmpPosition; // ���� �������� �ʰ� �̵��� ��ġ. �ֺ� 8�� ���� �̰��� ������ ���� ��ġ ����
    D3DXVECTOR2 WallPoint[2]; // �浹�� �˻��� ����� ���ʾƷ�(minX, minZ)�� ��������(maxX,maxZ) �� ��
    FLOAT fCoefficient = TRANSLATION_DISTANCE;
    int i, nCoX, nCoZ;

    // ���� ��ǥ
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
    // �ذ�: �ܰ� ���� �ε����� ��� �߰�, ��� ������ ��� ������ �������� ���� �����ϱ�
    // x�� ���� �������� �̵��� ���
    if (vecDirection.x < 0)
    {
        for (i = 0; i < 3; i++)
        {
            // ���� ��ġ ���� ���� 3�� ��Ͽ� ���� �˻�
            // ���� �ܰ� ���� ��� ���� �˻�
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

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
					if (map[nCoZ][nCoX - 1] == '*')
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

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
				{
					if (map[nCoZ][nCoX + 1] == '*')
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

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
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

                //�浹 ��
                if (WallPoint[0].x <= tmpPosition.x + PLAYER_RADIUS && WallPoint[1].x >= tmpPosition.x - PLAYER_RADIUS
                    && WallPoint[0].y <= tmpPosition.z + PLAYER_RADIUS && WallPoint[1].y >= tmpPosition.z - PLAYER_RADIUS)
                {
                    tmpPosition.z = WallPoint[0].y - PLAYER_RADIUS - 0.1f;
                    break;
                }
            }
        }
    }
    // LookAt�� Position�� �̵��� ��ŭ�� ���ϰų� ���ָ� ��
    m_LookAt.x += tmpPosition.x - m_Position.x;
    m_LookAt.z += tmpPosition.z - m_Position.z;
    SetPosition(tmpPosition);

    m_FlashLight.Position = m_Position;
}

VOID CPlayer::Rotate(BOOL bIsCCW)
{
	// bIsCCW�� q���� e���� �����ϰ�, angle��ŭ ȸ���� �ϸ�, LookAt�� Position ���� ������ distance
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

    m_FlashLight.Direction = D3DXVECTOR3(m_PlayerAxis._31, m_PlayerAxis._32, m_PlayerAxis._33);
}