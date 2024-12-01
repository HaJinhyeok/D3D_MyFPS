#include "XFileUtil.h"

CXFileUtil::CXFileUtil(D3DXVECTOR3 position)
{
	g_pMesh = NULL; // 메쉬 객체
	g_pMeshMaterials = NULL; // 메쉬에 대한 재질
	g_pMeshTextures = NULL; // 메쉬에 대한 텍스쳐
	g_dwNumMaterials = 0L; // 메쉬 재질의 개수

	// scale 먼저 하고 translation
	D3DXMatrixScaling(&m_TigerWorld, 7.0f, 7.0f, 7.0f);
	D3DXMATRIX tmpMat;
	D3DXMatrixTranslation(&tmpMat, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpMat);
	m_IsLive = TRUE;
	m_IsRotating = FALSE;
	m_CurrentTime = timeGetTime();
	m_RotationAmount = 0;
}
CXFileUtil::~CXFileUtil()
{
}

//---------------------------------------------------------------------------------
// 이름 : XFileLoad()
// 기능 : x 파일을 로드 한다.
//---------------------------------------------------------------------------------
int CXFileUtil::XFileLoad(LPDIRECT3DDEVICE9 pD3DDevice, char* xFileName)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
	// x 파일을 로딩한다.
	if (FAILED(D3DXLoadMeshFromX(xFileName, D3DXMESH_SYSTEMMEM, pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, &g_pMesh)))
	{
		MessageBox(NULL, "X파일 로드 실패", "메쉬로드 실패", MB_OK);
		return E_FAIL;
	}
	// 텍스쳐 파일이 다른 폴더에 있을 경우를 위하여 텍스쳐 패스 위치 닫기
	char texturePath[256];
	// 현재 폴더의 경우
	if (strchr(xFileName, '\\') == NULL)
		wsprintf(texturePath, "..\\");
	else
	{
		// 기타 폴더의 경우
		char temp[256], * pChar;
		strcpy(temp, xFileName);
		_strrev(temp);
		pChar = strchr(temp, '\\');
		strcpy(texturePath, pChar);
		_strrev(texturePath);
	}
	// x 파일 로딩 코드
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials    = new D3DMATERIAL9[g_dwNumMaterials];
	g_pMeshTextures     = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		// 재질 복사
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		// 재질에 대한 앰비언트 색상 설정(D3DX 가 해주지 않으므로)
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
		g_pMeshTextures[i] = NULL;
		// 텍스쳐 파일이 존재하는 경우
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// 텍스쳐 생성
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, d3dxMaterials[i].pTextureFilename, &g_pMeshTextures[i])))
			{
				// 경로 + 텍스쳐 파일 이름 만들기
				char tempFile[256];
				wsprintf(tempFile, "%s%s", texturePath, d3dxMaterials[i].pTextureFilename);
				g_pMeshTextures[i] = NULL;
				MessageBox(NULL, "Could not find texture map", "D3D_TEST.exe", MB_OK);
			}
		}
	}
	// 재질 버퍼 사용끝 & 해제
	pD3DXMtrlBuffer->Release();
	return S_OK;
}

//---------------------------------------------------------------------------------
// 이름 : XFileDisplay()
// 기능 : x 파일을 출력 해준다.
//---------------------------------------------------------------------------------
int CXFileUtil::XFileDisplay(LPDIRECT3DDEVICE9 pD3DDevice)
{
	// 메쉬 출력
	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		pD3DDevice->SetMaterial(&g_pMeshMaterials[i]);
		// 현재 sub Set에 대한 재질 설정
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
		// 호랑이가 통로를 따라 움직이게 한다.
		// 호랑이가 현재 바라보는 방향 정보와 맵 정보를 이용해서 현재 방향으로 더 전진해도 되는지 여부 판단
		// 현재 바라보는 방향이 벽으로 막혀있을 경우, 시계방향으로 90도씩 회전시켜 다시 전진 여부 판단
		// 현재 바라보는 방향이 막혀있지 않더라도, 여러 갈래 길일 경우 랜덤하게 진행방향 바꾸도록 해볼까
		// => 랜덤(50%?)으로 방향 전환할 지 안 할 지 결정하는
		D3DXVECTOR3 currentLookAt = D3DXVECTOR3(-m_TigerWorld._31, -m_TigerWorld._32, -m_TigerWorld._33);
		D3DXVECTOR3 currentPosition = D3DXVECTOR3(m_TigerWorld._41, m_TigerWorld._42, m_TigerWorld._43) / 10.0f;
		FLOAT tmpX = currentPosition.x - floorf(currentPosition.x), tmpZ = currentPosition.z - floorf(currentPosition.z);
		// 우선, 현재 호랑이가 블록 정가운데 위치해있는지 확인
		if (tmpX <= 0.5001f && tmpX >= 0.4999f)
		{
			if (tmpZ <= 0.5001f && tmpZ >= 0.4999f)
			{
				OutputDebugString("center of block\n");
				if (m_IsRotating == TRUE)
				{
					this->Rotate();
					if (m_RotationAmount == 90)
					{
						// 90도 회전했으므로 다시 확인하게 해야함
						m_IsRotating = FALSE;
						m_RotationAmount = 0;
					}
				}
				else
				{
					// 현재 좌표
					int nCoX = floorf(currentPosition.x / LENGTH_OF_TILE) + NUM_OF_COLUMN / 2;
					int nCoZ = NUM_OF_ROW / 2 - floorf(currentPosition.z / LENGTH_OF_TILE) - 1;
					// 진행 방향이 막혀있을 경우
					// 진행 방향이 열려있고, 왼쪽과 오른쪽이 막혀있을 경우 == 일단은 그냥 직진
					// 진행 방향이 열려있고, 왼쪽 또는 오른쪽도 열려있을 경우 == 일단은 회전, 몇 갈래인지 카운트해서 랜덤하게 회전시켜 진행
					if (currentLookAt.x > 0 && fabsf(currentLookAt.z) <= EPSILON)
					{
						// 우
						if (nCoX <= NUM_OF_COLUMN / 2 || map[nCoZ][nCoX + 1] == '*')
						{
							// 막혀있으므로 돌아야함
							m_IsRotating = TRUE;
						}
						else
						{
							if (map[nCoZ - 1][nCoX] != '*' || map[nCoZ + 1][nCoX] != '*')
							{
								m_IsRotating = TRUE;
							}
						}
					}
					else if (currentLookAt.x < 0 && fabsf(currentLookAt.z) <= EPSILON)
					{
						// 좌
						if (nCoX >= -NUM_OF_COLUMN / 2 || map[nCoZ][nCoX - 1] == '*')
						{
							// 막혀있으므로 돌아야함
							m_IsRotating = TRUE;
						}
						else
						{
							if (map[nCoZ - 1][nCoX] != '*' || map[nCoZ + 1][nCoX] != '*')
							{
								m_IsRotating = TRUE;
							}
						}
					}
					else if (currentLookAt.z > 0 && fabsf(currentLookAt.x) <= EPSILON)
					{
						// 위
						if (nCoZ <= NUM_OF_ROW / 2 || map[nCoZ + 1][nCoX] == '*')
						{
							// 막혀있으므로 돌아야함
							m_IsRotating = TRUE;
						}
						else
						{
							if (map[nCoZ][nCoX - 1] != '*' || map[nCoZ][nCoX + 1] != '*')
							{
								m_IsRotating = TRUE;
							}
						}
					}
					else if (currentLookAt.z < 0 && fabsf(currentLookAt.x) <= EPSILON)
					{
						// 아래
						if (nCoZ >= -NUM_OF_ROW / 2 || map[nCoZ - 1][nCoX] == '*')
						{
							// 막혀있으므로 돌아야함
							m_IsRotating = TRUE;
						}
						else
						{
							if (map[nCoZ][nCoX - 1] != '*' || map[nCoZ][nCoX + 1] != '*')
							{
								m_IsRotating = TRUE;
							}
						}
					}
				}
				
			}
		}
		if (m_IsRotating == FALSE)
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

VOID CXFileUtil::Rotate()
{
	// 시계방향으로 회전
	D3DXMATRIX tmpRotation, tmpTranslation;
	D3DXVECTOR3 tmpPosition = D3DXVECTOR3(m_TigerWorld._41, m_TigerWorld._42, m_TigerWorld._43);
	// 원점으로 이동
	D3DXMatrixTranslation(&tmpTranslation, -tmpPosition.x, -tmpPosition.y, -tmpPosition.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
	// 3도 회전
	D3DXMatrixRotationY(&tmpRotation, D3DXToRadian(3.0f));
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpRotation);
	m_RotationAmount += 3;
	// 다시 제자리로 이동
	D3DXMatrixTranslation(&tmpTranslation, tmpPosition.x, tmpPosition.y, tmpPosition.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
}