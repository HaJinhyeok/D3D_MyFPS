#include "XFileUtil.h"

CXFileUtil::CXFileUtil(D3DXVECTOR3 position)
{
	g_pMesh = NULL; // �޽� ��ü
	g_pMeshMaterials = NULL; // �޽��� ���� ����
	g_pMeshTextures = NULL; // �޽��� ���� �ؽ���
	g_dwNumMaterials = 0L; // �޽� ������ ����

	// scale ���� �ϰ� translation
	D3DXMatrixScaling(&m_TigerWorld, SCALE_AMOUNT_TIGER, SCALE_AMOUNT_TIGER, SCALE_AMOUNT_TIGER * 2.0f / 3.0f);
	D3DXMATRIX tmpMat;
	D3DXMatrixTranslation(&tmpMat, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpMat);
	m_IsLive = TRUE;
	m_IsRotating = FALSE;
	m_IsClockwise = TRUE;
	for (int i = 0; i < 4; i++)
	{
		m_IsWallOpen[i] = TRUE;
	}
	m_CurrentTime = timeGetTime();
	m_RotationAmount = 0;
	m_RotationCount = 0;
}
CXFileUtil::~CXFileUtil()
{
}

//---------------------------------------------------------------------------------
// �̸� : XFileLoad()
// ��� : x ������ �ε� �Ѵ�.
//---------------------------------------------------------------------------------
int CXFileUtil::XFileLoad(LPDIRECT3DDEVICE9 pD3DDevice, char* xFileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
	// x ������ �ε��Ѵ�.
	if (FAILED(D3DXLoadMeshFromX(xFileName, D3DXMESH_SYSTEMMEM, pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, &g_pMesh)))
	{
		MessageBox(NULL, "X���� �ε� ����", "�޽��ε� ����", MB_OK);
		return E_FAIL;
	}
	// �ؽ��� ������ �ٸ� ������ ���� ��츦 ���Ͽ� �ؽ��� �н� ��ġ �ݱ�
	char texturePath[256];
	// ���� ������ ���
	if (strchr(xFileName, '\\') == NULL)
		wsprintf(texturePath, "..\\");
	else
	{
		// ��Ÿ ������ ���
		char temp[256], * pChar;
		strcpy(temp, xFileName);
		_strrev(temp);
		pChar = strchr(temp, '\\');
		strcpy(texturePath, pChar);
		_strrev(texturePath);
	}
	// x ���� �ε� �ڵ�
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials    = new D3DMATERIAL9[g_dwNumMaterials];
	g_pMeshTextures     = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		// ���� ����
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		// ������ ���� �ں��Ʈ ���� ����(D3DX �� ������ �����Ƿ�)
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
		g_pMeshTextures[i] = NULL;
		// �ؽ��� ������ �����ϴ� ���
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// �ؽ��� ����
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, d3dxMaterials[i].pTextureFilename, &g_pMeshTextures[i])))
			{
				// ��� + �ؽ��� ���� �̸� �����
				char tempFile[256];
				wsprintf(tempFile, "%s%s", texturePath, d3dxMaterials[i].pTextureFilename);
				g_pMeshTextures[i] = NULL;
				MessageBox(NULL, "Could not find texture map", "D3D_TEST.exe", MB_OK);
			}
		}
	}
	// ���� ���� ��볡 & ����
	pD3DXMtrlBuffer->Release();
	return S_OK;
}

//---------------------------------------------------------------------------------
// �̸� : XFileDisplay()
// ��� : x ������ ��� ���ش�.
//---------------------------------------------------------------------------------
int CXFileUtil::XFileDisplay(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// �޽� ���
	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		pD3DDevice->SetMaterial(&g_pMeshMaterials[i]);
		// ���� sub Set�� ���� ���� ����
		pD3DDevice->SetTexture(0, g_pMeshTextures[i]);
		// Draw the mesh subset
		g_pMesh->DrawSubset(i);
	}
	pD3DDevice->SetTexture(0, NULL);

	return 0;
}

VOID CXFileUtil::Move(const char(*map)[NUM_OF_COLUMN + 1])
{
	DWORD currentTime = timeGetTime();
	if (currentTime - m_CurrentTime >= 10)
	{
		// ȣ���̰� ��θ� ���� �����̰� �Ѵ�.
		// ȣ���̰� ���� �ٶ󺸴� ���� ������ �� ������ �̿��ؼ� ���� �������� �� �����ص� �Ǵ��� ���� �Ǵ�
		// ���� �ٶ󺸴� ������ ������ �������� ���, �ð�������� 90���� ȸ������ �ٽ� ���� ���� �Ǵ�
		// ���� �ٶ󺸴� ������ �������� �ʴ���, ���� ���� ���� ��� �����ϰ� ������� �ٲٵ��� �غ���
		// => ����(50%?)���� ���� ��ȯ�� �� �� �� �� �����ϴ�
		D3DXVECTOR3 currentLookAt = D3DXVECTOR3(-m_TigerWorld._31, -m_TigerWorld._32, -m_TigerWorld._33);
		D3DXVec3Normalize(&currentLookAt, &currentLookAt);
		D3DXVECTOR3 currentPosition = D3DXVECTOR3(m_TigerWorld._41, m_TigerWorld._42, m_TigerWorld._43);
		FLOAT tmpX = currentPosition.x / LENGTH_OF_TILE, tmpZ = currentPosition.z / LENGTH_OF_TILE;
		tmpX -= floorf(tmpX);
		tmpZ -= floorf(tmpZ);
		// �켱, ���� ȣ���̰� ��� ����� ��ġ���ִ��� Ȯ��
		if (tmpX <= 0.5f + EPSILON && tmpX >= 0.5f - EPSILON)
		{
			if (tmpZ <= 0.50f + EPSILON && tmpZ >= 0.5f - EPSILON)
			{
				OutputDebugString("center of block\n");
				if (m_IsRotating == TRUE)
				{
					this->Rotate(m_IsClockwise);
				}
				else if (m_IsRotating == FALSE && m_RotationCount == 1)
				{
					m_RotationCount--;
				}
				else
				{
					// ���� ��ǥ
					int nCoX = floorf(currentPosition.x / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
					int nCoZ = NUM_OF_ROW / 2 - floorf(currentPosition.z / LENGTH_OF_TILE) - 1;
					// random_device�� �õ尪 ������ �޸��� Ʈ����Ʈ �˰������� ���� ����
					mt19937 m_Engine(m_Random());

					// �����¿� ĭ���� ���� �������� �Ǻ�
					{
						// 0 - ��
						if (nCoZ == 0)
						{
							m_IsWallOpen[0] = FALSE;
						}
						else if (map[nCoZ - 1][nCoX] == '*')
						{
							m_IsWallOpen[0] = FALSE;
						}
						else
						{
							m_IsWallOpen[0] = TRUE;
						}
						// 1 - ��
						if (nCoZ == NUM_OF_ROW - 1)
						{
							m_IsWallOpen[1] = FALSE;
						}
						else if (map[nCoZ + 1][nCoX] == '*')
						{
							m_IsWallOpen[1] = FALSE;
						}
						else
						{
							m_IsWallOpen[1] = TRUE;
						}
						// 2 - ��
						if (nCoX == 0)
						{
							m_IsWallOpen[2] = FALSE;
						}
						else if (map[nCoZ][nCoX - 1] == '*')
						{
							m_IsWallOpen[2] = FALSE;
						}

						else
						{
							m_IsWallOpen[2] = TRUE;
						}
						// 3 - ��
						if (nCoX == NUM_OF_COLUMN - 1)
						{
							m_IsWallOpen[3] = FALSE;
						}
						else if (map[nCoZ][nCoX + 1] == '*')
						{
							m_IsWallOpen[3] = FALSE;
						}
						else
						{
							m_IsWallOpen[3] = TRUE;
						}
					}
					
					// ���� ������ �������� ���
					// ���� ������ �����ְ�, ���ʰ� �������� �������� ��� == �ϴ��� �׳� ����
					// ���� ������ �����ְ�, ���� �Ǵ� �����ʵ� �������� ��� == �ϴ��� ȸ��, �� �������� ī��Ʈ�ؼ� �����ϰ� ȸ������ ����
					if (currentLookAt.x > 0 && fabsf(currentLookAt.z) <= EPSILON)
					{
						// +x direction
						if (m_IsWallOpen[3] == FALSE)
						{
							// ���� �������� �ϴ� ������ ȸ���� ��
							m_IsRotating = TRUE;
							m_RotationCount++;
							if (!m_IsWallOpen[0] && m_IsWallOpen[1])
							{
								m_IsClockwise = TRUE;
							}
							else if (m_IsWallOpen[0] && !m_IsWallOpen[1])
							{
								m_IsClockwise = FALSE;
							}
							else if (m_IsWallOpen[0] && m_IsWallOpen[1])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsClockwise = TRUE;
								}
								else
								{
									m_IsClockwise = FALSE;
								}
							}
							else
							{
								// ���� �� �������Ƿ� ȸ�� 2ȸ
								m_RotationCount++;
							}
						}
						else 
						{
							if (!m_IsWallOpen[0] && !m_IsWallOpen[1])
							{
								m_IsRotating = FALSE;
							}
							else if (!m_IsWallOpen[0] && m_IsWallOpen[1])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
							}
							else if (m_IsWallOpen[0] && !m_IsWallOpen[1])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
							else
							{
								uniform_int_distribution<INT> dis(0, 2);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else if (dis(m_Engine) == 1)
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
						}
					}
					else if (currentLookAt.x < 0 && fabsf(currentLookAt.z) <= EPSILON)
					{
						// -x direction
						if (m_IsWallOpen[2] == FALSE)
						{
							// ���� �������� �ϴ� ������ ȸ���� ��
							m_IsRotating = TRUE;
							m_RotationCount++;
							if (!m_IsWallOpen[1] && m_IsWallOpen[0])
							{
								m_IsClockwise = TRUE;
							}
							else if (m_IsWallOpen[1] && !m_IsWallOpen[0])
							{
								m_IsClockwise = FALSE;
							}
							else if (m_IsWallOpen[1] && m_IsWallOpen[0])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsClockwise = TRUE;
								}
								else
								{
									m_IsClockwise = FALSE;
								}
							}
							else
							{
								m_RotationCount++;
							}
						}
						else
						{
							if (!m_IsWallOpen[1] && !m_IsWallOpen[0])
							{
								m_IsRotating = FALSE;
							}
							else if (!m_IsWallOpen[1] && m_IsWallOpen[0])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
							}
							else if (m_IsWallOpen[1] && !m_IsWallOpen[0])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
							else
							{
								uniform_int_distribution<INT> dis(0, 2);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else if (dis(m_Engine) == 1)
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
						}
					}
					else if (currentLookAt.z > 0 && fabsf(currentLookAt.x) <= EPSILON)
					{
						// +z direction
						if (m_IsWallOpen[0] == FALSE)
						{
							// ���� �������� �ϴ� ������ ȸ���� ��
							m_IsRotating = TRUE;
							m_RotationCount++;
							if (!m_IsWallOpen[2] && m_IsWallOpen[3])
							{
								m_IsClockwise = TRUE;
							}
							else if (m_IsWallOpen[2] && !m_IsWallOpen[3])
							{
								m_IsClockwise = FALSE;
							}
							else if (m_IsWallOpen[2] && m_IsWallOpen[3])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsClockwise = TRUE;
								}
								else
								{
									m_IsClockwise = FALSE;
								}
							}
							else
							{
								// ���� �� �������Ƿ� ȸ�� 2ȸ
								m_RotationCount++;
							}
						}
						else
						{
							if (!m_IsWallOpen[2] && !m_IsWallOpen[3])
							{
								m_IsRotating = FALSE;
							}
							else if (!m_IsWallOpen[2] && m_IsWallOpen[3])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
							}
							else if (m_IsWallOpen[2] && !m_IsWallOpen[3])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
							else
							{
								uniform_int_distribution<INT> dis(0, 2);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else if (dis(m_Engine) == 1)
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
						}
					}
					else if (currentLookAt.z < 0 && fabsf(currentLookAt.x) <= EPSILON)
					{
						// -z direction
						if (m_IsWallOpen[1] == FALSE)
						{
							// ���� �������� �ϴ� ������ ȸ���� ��
							m_IsRotating = TRUE;
							m_RotationCount++;
							if (!m_IsWallOpen[3] && m_IsWallOpen[2])
							{
								m_IsClockwise = TRUE;
							}
							else if (m_IsWallOpen[3] && !m_IsWallOpen[2])
							{
								m_IsClockwise = FALSE;
							}
							else if (m_IsWallOpen[3] && m_IsWallOpen[2])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsClockwise = TRUE;
								}
								else
								{
									m_IsClockwise = FALSE;
								}
							}
							else
							{
								// ���� �� �������Ƿ� ȸ�� 2ȸ
								m_RotationCount++;
							}
						}
						else
						{
							// ������ �Ұ������� �����Ƿ� ȸ���� �ϴ��� 1ȸ��
							if (!m_IsWallOpen[3] && !m_IsWallOpen[2])
							{
								// ���� ������ �׳� ����
								m_IsRotating = FALSE;
							}
							else if (!m_IsWallOpen[3] && m_IsWallOpen[2])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
							}
							else if (m_IsWallOpen[3] && !m_IsWallOpen[2])
							{
								uniform_int_distribution<INT> dis(0, 1);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
							else
							{
								uniform_int_distribution<INT> dis(0, 2);
								if (dis(m_Engine) == 0)
								{
									m_IsRotating = FALSE;
								}
								else if (dis(m_Engine) == 1)
								{
									m_IsRotating = TRUE;
									m_IsClockwise = TRUE;
									m_RotationCount++;
								}
								else
								{
									m_IsRotating = TRUE;
									m_IsClockwise = FALSE;
									m_RotationCount++;
								}
							}
						}
					}
				}
				
			}
		}
		if (m_RotationAmount == 90)
		{
			if (m_RotationCount == 2)
			{
				m_RotationCount--;
			}
			else
			{
				m_IsRotating = FALSE;
			}
			// 90�� ȸ�� �� ���Ͱ� ����?
			m_RotationAmount = 0;
		}
		else if (m_IsRotating == FALSE)
		{
			D3DXMATRIX tmpTranslation;
			FLOAT fCoefficient = TRANSLATION_DISTANCE_TIGER;
			fCoefficient /= sqrtf(currentLookAt.x * currentLookAt.x + currentLookAt.y * currentLookAt.y + currentLookAt.z * currentLookAt.z);
			D3DXMatrixTranslation(&tmpTranslation, currentLookAt.x * fCoefficient, currentLookAt.y * fCoefficient, currentLookAt.z * fCoefficient);
			D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
			m_CurrentTime = currentTime;
		}
		
	}
}
// 90�� ȸ����Ű�� ���� ���Ͱ� ������ �ʿ��� ��
// 
VOID CXFileUtil::Rotate(BOOL clockwise)
{
	D3DXMATRIX tmpRotation, tmpTranslation;
	D3DXVECTOR3 tmpPosition = D3DXVECTOR3(m_TigerWorld._41, m_TigerWorld._42, m_TigerWorld._43);
	// �������� �̵�
	D3DXMatrixTranslation(&tmpTranslation, -tmpPosition.x, -tmpPosition.y, -tmpPosition.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
	// 3�� ȸ��
	if (clockwise == TRUE)
	{
		D3DXMatrixRotationY(&tmpRotation, D3DXToRadian(3.0f));
	}
	else
	{
		D3DXMatrixRotationY(&tmpRotation, D3DXToRadian(-3.0f));
	}
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpRotation);
	m_RotationAmount += 3;
	// �ٽ� ���ڸ��� �̵�
	D3DXMatrixTranslation(&tmpTranslation, tmpPosition.x, tmpPosition.y, tmpPosition.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
}