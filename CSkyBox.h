#pragma once
#include "FPS.h"
extern LPDIRECT3DDEVICE9 g_pd3dDevice;

class CSkyBox
{
private:
	// skybox�� ���� texture�� ��ġ�� ���� vertex buffer
	LPDIRECT3DTEXTURE9 m_BoxTextures[6];
	LPDIRECT3DVERTEXBUFFER9 m_BoxVertexBuffer;

	CUSTOMVERTEX m_BoxVertices[24];

public:
	CSkyBox();
	~CSkyBox();
	VOID LoadTexture();
	VOID CreateVertexBuffer();
	VOID Render();
	
};

