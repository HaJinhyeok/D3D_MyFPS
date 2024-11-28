#pragma once
#pragma warning(disable:4996)
#include <d3dx9.h>

class CXFileUtil
{
private:
	// x ���� �ε�� ����
	LPD3DXMESH g_pMesh; // �޽� ��ü
	D3DMATERIAL9* g_pMeshMaterials; // �޽��� ���� ����
	LPDIRECT3DTEXTURE9* g_pMeshTextures; // �޽��� ���� �ؽ���
	DWORD g_dwNumMaterials; // ������ ��

public:
	int XFileDisplay(LPDIRECT3DDEVICE9 pD3DDevice);
	int XFileLoad(LPDIRECT3DDEVICE9 pD3DDevice, char* xFileName);
	CXFileUtil();
	virtual ~CXFileUtil();
};

