#include "CPlayer.h"
CPlayer::CPlayer()
{
	D3DXMATRIX tmpMatrix;
	m_Position = D3DXVECTOR3(55.0f, LENGTH_OF_TILE / 2, -65.0f); // v3EyeDefault
	m_LookAt = m_Position;
	m_LookAt.z += LOOKAT_DISTANCE;
	// D3DXMatrixIdentity(&m_PlayerWorld);
	D3DXMatrixTranslation(&tmpMatrix, m_Position.x, m_Position.y, m_Position.z);
	// D3DXMatrixMultiply(&m_PlayerWorld, &m_PlayerWorld, &tmpMatrix);
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

    m_CurrentTime = timeGetTime();
}
CPlayer::~CPlayer()
{

}

// PLAYER MOVE
// 바꾸어야 할 것: 플레이어 위치, 플레이어 LookAt
// 필요한 것: 플레이어가 움직일 방향 벡터, 거리, 플에이어 현 위치, 플레이어 현 LookAt, 맵 정보
VOID CPlayer::Move(MOVE_DIRECTION direction, const char(*map)[NUM_OF_COLUMN + 1], BOOL NoClip)
{
    DWORD currentTime = timeGetTime();
    if (currentTime - m_CurrentTime < 100) return;

    D3DXVECTOR3 vecDirection, tmpPosition; // 벽을 생각하지 않고 이동된 위치. 주변 8개 벽과 이것을 대조해 최종 위치 결정
    FLOAT fCoefficient = TRANSLATION_DISTANCE;
    int i, nCoX, nCoZ;

    // y축 움직임은 없으므로 y축 정보는 걍 빼고 계산
    // => 자유시점의 경우도 생각해서 y축도 이동
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
    if (!NoClip)
    {
        // 충돌을 검사할 블록의 왼쪽아래(minX, minZ)와 오른쪽위(maxX,maxZ) 두 점
        D3DXVECTOR2 WallPoint[2];
        // 현재 좌표
        nCoX = floorf(m_Position.x / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
        nCoZ = NUM_OF_ROW / 2 - floorf(m_Position.z / LENGTH_OF_TILE) - 1;
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
						if (map[nCoZ][nCoX - 1] == '*')
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
						if (map[nCoZ][nCoX + 1] == '*')
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
						tmpPosition.z = WallPoint[0].y - PLAYER_RADIUS - 0.1f;
						break;
					}
				}
			}
		}
    }
    // LookAt은 Position이 이동한 만큼만 더하거나 빼주면 됨
    m_LookAt.x += tmpPosition.x - m_Position.x;
    m_LookAt.z += tmpPosition.z - m_Position.z;
    if (NoClip)
    {
        m_LookAt.y += tmpPosition.y - m_Position.y;
    }

    if (!NoClip)
	{
        tmpPosition.y = LENGTH_OF_TILE / 2;
	}
    SetPosition(tmpPosition);

    m_FlashLight.Position = m_Position;
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

    m_FlashLight.Direction = D3DXVECTOR3(m_PlayerAxis._31, m_PlayerAxis._32, m_PlayerAxis._33);
}
VOID CPlayer::Rotate(BOOL bIsCCW, BOOL bIsUpDown, FLOAT angle)
{
    // 위아래일때 ccw면 위, !ccw면 아래
    D3DXMATRIX mtRotation;
    D3DXVECTOR3 v3RotationAxis;
    FLOAT fCoefficient = LOOKAT_DISTANCE;

    // 좌우 회전
    if (bIsUpDown == FALSE)
    {
        v3RotationAxis = D3DXVECTOR3(m_PlayerAxis._21, m_PlayerAxis._22, m_PlayerAxis._23);
        if (bIsCCW == TRUE)
        {
            // D3DXMatrixRotationY(&mtRotation, -angle);
            D3DXMatrixRotationAxis(&mtRotation, &v3RotationAxis, -angle);
        }
        else
        {
            // D3DXMatrixRotationY(&mtRotation, angle);
            D3DXMatrixRotationAxis(&mtRotation, &v3RotationAxis, angle);
        }
    }
    // 상하 회전
    else
    {
        v3RotationAxis = D3DXVECTOR3(m_PlayerAxis._11, m_PlayerAxis._12, m_PlayerAxis._13);
        if (bIsCCW == TRUE)
        {
            // D3DXMatrixRotationX(&mtRotation, angle);
            D3DXMatrixRotationAxis(&mtRotation, &v3RotationAxis, angle);
        }
        else
        {
            // D3DXMatrixRotationX(&mtRotation, -angle);
            D3DXMatrixRotationAxis(&mtRotation, &v3RotationAxis, -angle);
        }
    }
    D3DXMatrixMultiply(&m_PlayerAxis, &m_PlayerAxis, &mtRotation);

    fCoefficient /= sqrtf(m_PlayerAxis._31 * m_PlayerAxis._31 + m_PlayerAxis._32 * m_PlayerAxis._32 + m_PlayerAxis._33 * m_PlayerAxis._33);
    m_LookAt.x = m_Position.x + m_PlayerAxis._31 * fCoefficient;
    m_LookAt.y = m_Position.y + m_PlayerAxis._32 * fCoefficient;
    m_LookAt.z = m_Position.z + m_PlayerAxis._33 * fCoefficient;

    m_FlashLight.Direction = D3DXVECTOR3(m_PlayerAxis._31, m_PlayerAxis._32, m_PlayerAxis._33);
}

VOID CPlayer::Attack(LPPOINT CursorPosition)
{
    // 마우스 클릭 시, 클릭한 방향으로 벡터를 설정하고 일직선으로 날아가는 투사체 발사
    // world 상에서 플레이어 위치를 (0,0,0)으로 생각하고, 마우스가 클릭되는 평면을 
    // 총알 크기, 속도, 연사(마우스 꾹 누르고 있을 때), 벽이나 오브젝트에 닿으면 사라지도록
    Bullet tmpBullet;
    tmpBullet.v3Position = this->GetPosition();
    tmpBullet.v3Direction = this->GetLookAt() - this->GetPosition();
    tmpBullet.Time = timeGetTime();
    m_Bullet.push_back(tmpBullet);
}
VOID CPlayer::MoveBullet()
{
    // 매 프레임마다 호출해서 총알의 이동 및 충돌 후 제거 연산 수행 (x)
    // 프레임마다 호출하면 프레임 낮은 똥컴에서는 총알 속도가 느려지는 말도 안 되는 상황 발생한다.
    // 프레임이 아닌, 실제 시간을 기준으로 이동시켜야함
    // 발사된 총알이 없을 시 건너뜀
    DWORD currentTime = timeGetTime();
    if (m_Bullet.size() == 0)
        return;
    else
    {
        FLOAT fCoefficient;
        for (int i = 0; i < m_Bullet.size(); i++)
        {
            fCoefficient = this->m_BulletVelocity * (currentTime - m_Bullet[i].Time);
            m_Bullet[i].Time = currentTime;
            // 우선 이동
            fCoefficient /= sqrtf(m_Bullet[i].v3Direction.x * m_Bullet[i].v3Direction.x + m_Bullet[i].v3Direction.y * m_Bullet[i].v3Direction.y + m_Bullet[i].v3Direction.z * m_Bullet[i].v3Direction.z);
            m_Bullet[i].v3Position.x += fCoefficient * m_Bullet[i].v3Direction.x;
            m_Bullet[i].v3Position.y += fCoefficient * m_Bullet[i].v3Direction.y;
            m_Bullet[i].v3Position.z += fCoefficient * m_Bullet[i].v3Direction.z;
            // 벽 또는 장애물과 충돌 검사
            // 일단은 플레이어로부터 100만큼 떨어지면 제거되게
            if (Length(m_Bullet[i].v3Position - this->m_Position) >= 100.0f)
            {
                // 만약 이렇게 해서 맨 앞 원소를 pop_front하게 되면, 원래 두번째 원소였던 애가 첫번째 원소가 되어버린다.
                // 근데 i값은 반복문 진행되면서 ++되므로 이러면 나가린데
                m_Bullet.pop_front();
                // 그러면 i 값을 하나 줄여주면 되겠네
                i--;
            }
        }
    }
}
VOID CPlayer::DrawBullet(LPDIRECT3DDEVICE9 device, LPD3DXMESH sphere)
{
    D3DXMATRIX world;
    for (int i = 0; i < m_Bullet.size(); i++)
    {
        D3DXMatrixIdentity(&world);
        D3DXMatrixTranslation(&world, m_Bullet[i].v3Position.x, m_Bullet[i].v3Position.y, m_Bullet[i].v3Position.z);
        device->SetTransform(D3DTS_WORLD, &world);
        sphere->DrawSubset(0);
    }
}