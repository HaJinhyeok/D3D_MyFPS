#include "XFileUtil.h"

CXFileUtil::CXFileUtil(D3DXVECTOR3 position)
{
	g_pMesh = NULL; // �޽� ��ü
	g_pMeshMaterials = NULL; // �޽��� ���� ����
	g_pMeshTextures = NULL; // �޽��� ���� �ؽ���
	g_dwNumMaterials = 0L; // �޽� ������ ����

	D3DXMatrixScaling(&m_TigerWorld, 7.0f, 7.0f, 7.0f);
	D3DXMATRIX tmpMat;
	D3DXMatrixTranslation(&tmpMat, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpMat);
	m_IsLive = TRUE;
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

VOID CXFileUtil::Move()
{
	DWORD currentTime = timeGetTime();
	if (currentTime - m_CurrentTime >= 10)
	{
		// ȣ���̰� ��θ� ���� �����̰� �Ѵ�.
		// ȣ���̰� ���� �ٶ󺸴� ���� ������ �� ������ �̿��ؼ� ���� �������� �� �����ص� �Ǵ��� ���� �Ǵ�
		// ���� �ٶ󺸴� ������ ������ �������� ���, �ð�������� 90���� ȸ������ �ٽ� ���� ���� �Ǵ�
		// ���� �ٶ󺸴� ������ �������� �ʴ���, ���� ���� ���� ��� �����ϰ� ������� �ٲٵ��� �غ���
		D3DXVECTOR3 currentLookAt = D3DXVECTOR3(m_TigerWorld._31, m_TigerWorld._32, -m_TigerWorld._33);
		D3DXMATRIX tmpTranslation;
		FLOAT fCoefficient = TRANSLATION_DISTANCE_TIGER;
		fCoefficient /= sqrtf(currentLookAt.x * currentLookAt.x + currentLookAt.y * currentLookAt.y + currentLookAt.z * currentLookAt.z);
		D3DXMatrixTranslation(&tmpTranslation, currentLookAt.x * fCoefficient, currentLookAt.y * fCoefficient, currentLookAt.z * fCoefficient);
		D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpTranslation);
		m_CurrentTime = currentTime;
	}
}

VOID CXFileUtil::Rotate()
{
	// �ð�������� ȸ��
	D3DXMATRIX tmpRotation;
	// 3�� ȸ��
	D3DXMatrixRotationY(&tmpRotation, D3DXToRadian(3.0f));
	D3DXMatrixMultiply(&m_TigerWorld, &m_TigerWorld, &tmpRotation);
}