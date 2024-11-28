#include "XFileUtil.h"

CXFileUtil::CXFileUtil()
{
	g_pMesh = NULL; // 메쉬 객체
	g_pMeshMaterials = NULL; // 메쉬에 대한 재질
	g_pMeshTextures = NULL; // 메쉬에 대한 텍스쳐
	g_dwNumMaterials = 0L; // 메쉬 재질의 개수
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