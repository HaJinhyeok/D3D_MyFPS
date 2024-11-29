#pragma once
#pragma warning(disable:4996)
#include <d3dx9.h>
#include <time.h>

#define TRANSLATION_DISTANCE_TIGER 0.1f

class CXFileUtil
{
private:
	// x ���� �ε�� ����
	LPD3DXMESH g_pMesh; // �޽� ��ü
	D3DMATERIAL9* g_pMeshMaterials; // �޽��� ���� ����
	LPDIRECT3DTEXTURE9* g_pMeshTextures; // �޽��� ���� �ؽ���
	DWORD g_dwNumMaterials; // ������ ��
	// x ���� ��ü�� ��ġ�� �ٶ󺸴� ����
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXMATRIX m_TigerWorld;
	// x ���� ��ü(ȣ����)�� �ϴ� �ʿ� �� ������ �����ϵ��� ����
	// �� �� ���� ȣ���̰� ����ִ��� �ƴ��� �Ǻ�
	// �Ѿ˷� ȣ���� ���缭 ������, �� �������κ��� ���� �ð� �� ȣ���� �ٽ� ���� ��ġ���� �����ǵ��� ����
	BOOL m_IsLive;
	DWORD m_CurrentTime;

public:
	int XFileDisplay(LPDIRECT3DDEVICE9 pD3DDevice);
	int XFileLoad(LPDIRECT3DDEVICE9 pD3DDevice, char* xFileName);
	VOID Move();
	VOID Rotate();

	VOID SetPosition(D3DXVECTOR3 position)
	{
		m_Position = position;
	}
	VOID SetLookAt(D3DXVECTOR3 look)
	{
		m_LookAt = look;
	}
	VOID SetLive(BOOL isLive)
	{
		m_IsLive = isLive;
	}
	D3DXVECTOR3 GetPosition()
	{
		return m_Position;
	}
	D3DXVECTOR3 GetLookAt()
	{
		return m_LookAt;
	}
	BOOL GetLive()
	{
		return m_IsLive;
	}
	D3DXMATRIX& GetTigerWorld()
	{
		return m_TigerWorld;
	}

	CXFileUtil(D3DXVECTOR3 position);
	virtual ~CXFileUtil();
};

