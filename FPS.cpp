
#include "FPS_function.h"
#include "Input.h"
#include "CPlayer.h"

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pTileVB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pTileIB = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pWallVB = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pWallVB2 = NULL;
LPDIRECT3DTEXTURE9 g_pTileTexture = NULL;
LPDIRECT3DTEXTURE9 g_pWallTexture = NULL;
LPD3DXFONT g_pFont = NULL;
LPD3DXMESH g_pSphere = NULL;

CPlayer player;
RECT rt;
char testSTR[255];
wchar_t test2[255];
CUSTOMVERTEX* tmpBlock;
LPDIRECT3DVERTEXBUFFER9 g_pTmpBlockVB = NULL;

HRESULT InitD3D(HWND hWnd)
{
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
        return E_FAIL;

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    return S_OK;
}

VOID InitGeometry()
{
    InitInput();
    int i, j;
    D3DXCreateSphere(g_pd3dDevice, 1.0f, 10, 10, &g_pSphere, 0);
    D3DXCreateFont(g_pd3dDevice, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont);
    D3DXCreateTextureFromFile(g_pd3dDevice, TEXTURE_TILE, &g_pTileTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, TEXTURE_WALL, &g_pWallTexture);

    tmpBlock = MakeWallBlock(D3DXVECTOR3(25.0f, 5.0f, 25.0f));
    g_pd3dDevice->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * 20, 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pTmpBlockVB, NULL);
    VOID** tmpVertices;
    g_pTmpBlockVB->Lock(0, sizeof(CUSTOMVERTEX) * 20, (void**)&tmpVertices, 0);
    memcpy(tmpVertices, tmpBlock, sizeof(CUSTOMVERTEX) * 20);
    g_pTmpBlockVB->Unlock();


    // tile vertex 좌표 입력
    {
        for (i = 0; i < NUM_OF_ROW * NUM_OF_COLUMN; i++)
        {
            FLOAT fCoX = (FLOAT)((i % NUM_OF_COLUMN - NUM_OF_COLUMN / 2.0f) * LENGTH_OF_TILE);
            FLOAT fCoZ = (FLOAT)((NUM_OF_ROW / 2.0f - i / NUM_OF_COLUMN) * LENGTH_OF_TILE);
            // D3DFVF_NORMAL
            for (j = 0; j < 4; j++)
            {
                TileVertices[i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            }
            // D3DFVF_XYZ
            TileVertices[i * 4].v3VerPos = D3DXVECTOR3(fCoX, 0.0f, fCoZ);
            TileVertices[i * 4 + 1].v3VerPos = D3DXVECTOR3(fCoX + LENGTH_OF_TILE, 0.0f, fCoZ);
            TileVertices[i * 4 + 2].v3VerPos = D3DXVECTOR3(fCoX + LENGTH_OF_TILE, 0.0f, fCoZ - LENGTH_OF_TILE);
            TileVertices[i * 4 + 3].v3VerPos = D3DXVECTOR3(fCoX, 0.0f, fCoZ - LENGTH_OF_TILE);
            // D3DFVF_TEX1
            TileVertices[i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            TileVertices[i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            TileVertices[i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            TileVertices[i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
    }
    // tile index 입력
    {
        j = 0;
        for (i = 0; i < NUM_OF_ROW * NUM_OF_COLUMN; i++)
        {
            wTileIndices[j][0] = i * 4;
            wTileIndices[j][1] = i * 4 + 1;
            wTileIndices[j][2] = i * 4 + 2;
            wTileIndices[++j][0] = i * 4;
            wTileIndices[j][1] = i * 4 + 2;
            wTileIndices[j++][2] = i * 4 + 3;
        }
    }
    // Create tile vertex & index buffer
    g_pd3dDevice->CreateVertexBuffer(sizeof(TileVertices), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pTileVB, NULL);
    VOID* pVertices2;
    g_pTileVB->Lock(0, sizeof(TileVertices), (void**)&pVertices2, 0);
    memcpy(pVertices2, TileVertices, sizeof(TileVertices));
    g_pTileVB->Unlock();

    g_pd3dDevice->CreateIndexBuffer(sizeof(wTileIndices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pTileIB, NULL);
    VOID* pIndices2;
    g_pTileIB->Lock(0, sizeof(wTileIndices), (void**)&pIndices2, 0);
    memcpy(pIndices2, wTileIndices, sizeof(wTileIndices));
    g_pTileIB->Unlock();

    //// wall vertex 좌표 입력
    {
        // 위쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[0][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

            WallVertices[0][i * 4].v3VerPos = D3DXVECTOR3((i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[0][i * 4 + 1].v3VerPos = D3DXVECTOR3((i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[0][i * 4 + 2].v3VerPos = D3DXVECTOR3((i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 0.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[0][i * 4 + 3].v3VerPos = D3DXVECTOR3((i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 0.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

            WallVertices[0][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices[0][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices[0][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices[0][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // 아래쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[1][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

            WallVertices[1][i * 4].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[1][i * 4 + 1].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[1][i * 4 + 2].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE, 0.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices[1][i * 4 + 3].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE, 0.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

            WallVertices[1][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices[1][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices[1][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices[1][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // 왼쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
				WallVertices[2][i * 4 + j].v3VerNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

			WallVertices[2][i * 4].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 10.0f, (i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
			WallVertices[2][i * 4 + 1].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 10.0f, (i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
			WallVertices[2][i * 4 + 2].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 0.0f, (i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
			WallVertices[2][i * 4 + 3].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 0.0f, (i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

			WallVertices[2][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
			WallVertices[2][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
			WallVertices[2][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
			WallVertices[2][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
		}
		// 오른쪽 면
		for (i = 0; i < NUM_OF_ROW; i++)
		{
			for (j = 0; j < 4; j++)
				WallVertices[3][i * 4 + j].v3VerNormal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

			WallVertices[3][i * 4].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE);
			WallVertices[3][i * 4 + 1].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE);
			WallVertices[3][i * 4 + 2].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 0.0f, (NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE);
			WallVertices[3][i * 4 + 3].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 0.0f, (NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE);

            WallVertices[3][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices[3][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices[3][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices[3][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // Create wall vertex buffer
        g_pd3dDevice->CreateVertexBuffer(sizeof(WallVertices), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pWallVB, NULL);
        VOID* pWallVertices;
        g_pWallVB->Lock(0, sizeof(WallVertices), (void**)&pWallVertices, 0);
        memcpy(pWallVertices, WallVertices, sizeof(WallVertices));
        g_pWallVB->Unlock();
    }

    //// wall vertex 2 좌표 입력
    {
        // 위쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[0][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            WallVertices2[0][i * 4].v3VerPos = D3DXVECTOR3((i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE);
            WallVertices2[0][i * 4 + 1].v3VerPos = D3DXVECTOR3((i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f+1) * LENGTH_OF_TILE);
            WallVertices2[0][i * 4 + 2].v3VerPos = D3DXVECTOR3((i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices2[0][i * 4 + 3].v3VerPos = D3DXVECTOR3((i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

            WallVertices2[0][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices2[0][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices2[0][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices2[0][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // 아래쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[1][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            WallVertices2[1][i * 4].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f-1) * LENGTH_OF_TILE);
            WallVertices2[1][i * 4 + 1].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f-1) * LENGTH_OF_TILE);
            WallVertices2[1][i * 4 + 2].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices2[1][i * 4 + 3].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE, 10.0f, (-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

            WallVertices2[1][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices2[1][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices2[1][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices2[1][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // 왼쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[2][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            WallVertices2[2][i * 4].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices2[2][i * 4 + 1].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices2[2][i * 4 + 2].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 10.0f, (i + 1 - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);
            WallVertices2[2][i * 4 + 3].v3VerPos = D3DXVECTOR3((-NUM_OF_ROW / 2.0f + 1) * LENGTH_OF_TILE, 10.0f, (i - NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE);

            WallVertices2[2][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices2[2][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices2[2][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices2[2][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // 오른쪽 면
        for (i = 0; i < NUM_OF_ROW; i++)
        {
            for (j = 0; j < 4; j++)
                WallVertices[3][i * 4 + j].v3VerNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            WallVertices2[3][i * 4].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE);
            WallVertices2[3][i * 4 + 1].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE);
            WallVertices2[3][i * 4 + 2].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i - 1) * LENGTH_OF_TILE);
            WallVertices2[3][i * 4 + 3].v3VerPos = D3DXVECTOR3((NUM_OF_ROW / 2.0f - 1) * LENGTH_OF_TILE, 10.0f, (NUM_OF_ROW / 2.0f - i) * LENGTH_OF_TILE);

            WallVertices2[3][i * 4].v2VerTex = D3DXVECTOR2(0.0f, 0.0f);
            WallVertices2[3][i * 4 + 1].v2VerTex = D3DXVECTOR2(1.0f, 0.0f);
            WallVertices2[3][i * 4 + 2].v2VerTex = D3DXVECTOR2(1.0f, 1.0f);
            WallVertices2[3][i * 4 + 3].v2VerTex = D3DXVECTOR2(0.0f, 1.0f);
        }
        // Create wall vertex buffer
        g_pd3dDevice->CreateVertexBuffer(sizeof(WallVertices2), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pWallVB2, NULL);
        VOID* pWallVertices2;
        g_pWallVB2->Lock(0, sizeof(WallVertices2), (void**)&pWallVertices2, 0);
        memcpy(pWallVertices2, WallVertices2, sizeof(WallVertices2));
        g_pWallVB2->Unlock();
    }
}

VOID CleanUp()
{
    if (tmpBlock != NULL)
        delete tmpBlock;
    if (g_pTmpBlockVB != NULL)
        g_pTmpBlockVB->Release();
    if (g_pSphere != NULL)
        g_pSphere->Release();
    if (g_pFont != NULL)
        g_pFont->Release();
    if (g_pWallTexture != NULL)
        g_pWallTexture->Release();
    if (g_pTileTexture != NULL)
        g_pTileTexture->Release();
    if (g_pWallVB2 != NULL)
        g_pWallVB2->Release();
    if (g_pWallVB != NULL)
        g_pWallVB->Release();
    if (g_pTileIB != NULL)
        g_pTileIB->Release();
    if (g_pTileVB != NULL)
        g_pTileVB->Release();
    if (g_pd3dDevice != NULL)
        g_pd3dDevice->Release();
    if (g_pD3D != NULL)
        g_pD3D->Release();
}

VOID __KeyProc()
{
    UpdateInput();
    // wasd 또는 방향키 : 플레이어 앞뒤좌우 움직임
    D3DXVECTOR3 v3CurrentPosition = player.GetPosition();
    D3DXVECTOR3 v3CurrentLookAt = player.GetLookAt();
    D3DXMATRIX mtPlayerWorld = player.GetPlayerWorld();
    D3DXMATRIX mtPlayerAxis = player.GetPlayerAxis();

    D3DXVECTOR3 v3Axis;
    
    D3DXMATRIX mtTranslation, mtRotation, tmpMatrix;
    FLOAT fCoefficient;
    if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
    {
        v3Axis.x = -mtPlayerAxis._11;
        v3Axis.y = -mtPlayerAxis._12;
        v3Axis.z = -mtPlayerAxis._13;
        fCoefficient = TRANSLATION_DISTANCE / sqrtf(powf(v3Axis.x, 2.0f) + powf(v3Axis.y, 2.0f) + powf(v3Axis.z, 2.0f));

        v3CurrentPosition.x += v3Axis.x * fCoefficient;
        v3CurrentPosition.y += v3Axis.y * fCoefficient;
        v3CurrentPosition.z += v3Axis.z * fCoefficient;
        player.SetPosition(v3CurrentPosition);

        v3CurrentLookAt.x += v3Axis.x * fCoefficient;
        v3CurrentLookAt.y += v3Axis.y * fCoefficient;
        v3CurrentLookAt.z += v3Axis.z * fCoefficient;
        player.SetLookAt(v3CurrentLookAt);

        D3DXMatrixTranslation(&mtTranslation, v3Axis.x * fCoefficient, v3Axis.y * fCoefficient, v3Axis.z * fCoefficient);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtTranslation);
        player.SetPlayerWorld(mtPlayerWorld);
    }
    if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
    {
        v3Axis.x = mtPlayerAxis._11;
        v3Axis.y = mtPlayerAxis._12;
        v3Axis.z = mtPlayerAxis._13;
        fCoefficient = TRANSLATION_DISTANCE / sqrtf(powf(v3Axis.x, 2.0f) + powf(v3Axis.y, 2.0f) + powf(v3Axis.z, 2.0f));

        v3CurrentPosition.x += v3Axis.x * fCoefficient;
        v3CurrentPosition.y += v3Axis.y * fCoefficient;
        v3CurrentPosition.z += v3Axis.z * fCoefficient;
        player.SetPosition(v3CurrentPosition);

        v3CurrentLookAt.x += v3Axis.x * fCoefficient;
        v3CurrentLookAt.y += v3Axis.y * fCoefficient;
        v3CurrentLookAt.z += v3Axis.z * fCoefficient;
        player.SetLookAt(v3CurrentLookAt);

        D3DXMatrixTranslation(&mtTranslation, v3Axis.x * fCoefficient, v3Axis.y * fCoefficient, v3Axis.z * fCoefficient);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtTranslation);
        player.SetPlayerWorld(mtPlayerWorld);
    }
    if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
    {
        v3Axis.x = mtPlayerAxis._31;
        v3Axis.y = mtPlayerAxis._32;
        v3Axis.z = mtPlayerAxis._33;
        fCoefficient = TRANSLATION_DISTANCE / sqrtf(powf(v3Axis.x, 2.0f) + powf(v3Axis.y, 2.0f) + powf(v3Axis.z, 2.0f));

        v3CurrentPosition.x += v3Axis.x * fCoefficient;
        v3CurrentPosition.y += v3Axis.y * fCoefficient;
        v3CurrentPosition.z += v3Axis.z * fCoefficient;
        player.SetPosition(v3CurrentPosition);

        v3CurrentLookAt.x += v3Axis.x * fCoefficient;
        v3CurrentLookAt.y += v3Axis.y * fCoefficient;
        v3CurrentLookAt.z += v3Axis.z * fCoefficient;
        player.SetLookAt(v3CurrentLookAt);

        D3DXMatrixTranslation(&mtTranslation, v3Axis.x * fCoefficient, v3Axis.y * fCoefficient, v3Axis.z * fCoefficient);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtTranslation);
        player.SetPlayerWorld(mtPlayerWorld);
    }
    if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
    {
        v3Axis.x = -mtPlayerAxis._31;
        v3Axis.y = -mtPlayerAxis._32;
        v3Axis.z = -mtPlayerAxis._33;
        fCoefficient = TRANSLATION_DISTANCE / sqrtf(powf(v3Axis.x, 2.0f) + powf(v3Axis.y, 2.0f) + powf(v3Axis.z, 2.0f));

        v3CurrentPosition.x += v3Axis.x * fCoefficient;
        v3CurrentPosition.y += v3Axis.y * fCoefficient;
        v3CurrentPosition.z += v3Axis.z * fCoefficient;
        player.SetPosition(v3CurrentPosition);

        v3CurrentLookAt.x += v3Axis.x * fCoefficient;
        v3CurrentLookAt.y += v3Axis.y * fCoefficient;
        v3CurrentLookAt.z += v3Axis.z * fCoefficient;
        player.SetLookAt(v3CurrentLookAt);

        D3DXMatrixTranslation(&mtTranslation, v3Axis.x * fCoefficient, v3Axis.y * fCoefficient, v3Axis.z * fCoefficient);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtTranslation);
        player.SetPlayerWorld(mtPlayerWorld);
    }
    
    // Q/E : 플레이어 CCW/CW 회전
    if (GetAsyncKeyState('Q'))
    {
        D3DXMatrixRotationY(&mtRotation, -ROTATION_AMOUNT);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtRotation);
        player.SetPlayerWorld(mtPlayerWorld);
        D3DXMatrixMultiply(&mtPlayerAxis, &mtPlayerAxis, &mtRotation);
        player.SetPlayerAxis(mtPlayerAxis);

        // position은 그대로지만, look at은 회전해야함!
        D3DXMatrixIdentity(&tmpMatrix);
        tmpMatrix._11 = v3CurrentLookAt.x - v3CurrentPosition.x;
        tmpMatrix._12 = v3CurrentLookAt.y - v3CurrentPosition.y;
        tmpMatrix._13 = v3CurrentLookAt.z - v3CurrentPosition.z;
        D3DXMatrixMultiply(&tmpMatrix, &tmpMatrix, &mtRotation);
        v3CurrentLookAt.x = tmpMatrix._11 + v3CurrentPosition.x;
        v3CurrentLookAt.y = tmpMatrix._12 + v3CurrentPosition.y;
        v3CurrentLookAt.z = tmpMatrix._13 + v3CurrentPosition.z;
        player.SetLookAt(v3CurrentLookAt);
    }
    if (GetAsyncKeyState('E'))
    {
        D3DXMatrixRotationY(&mtRotation, ROTATION_AMOUNT);
        D3DXMatrixMultiply(&mtPlayerWorld, &mtPlayerWorld, &mtRotation);
        player.SetPlayerWorld(mtPlayerWorld);
        D3DXMatrixMultiply(&mtPlayerAxis, &mtPlayerAxis, &mtRotation);
        player.SetPlayerAxis(mtPlayerAxis);
        // position은 그대로지만, look at은 회전해야함!
        D3DXMatrixIdentity(&tmpMatrix);
        tmpMatrix._11 = v3CurrentLookAt.x - v3CurrentPosition.x;
        tmpMatrix._12 = v3CurrentLookAt.y - v3CurrentPosition.y;
        tmpMatrix._13 = v3CurrentLookAt.z - v3CurrentPosition.z;
        D3DXMatrixMultiply(&tmpMatrix, &tmpMatrix, &mtRotation);
        v3CurrentLookAt.x = tmpMatrix._11 + v3CurrentPosition.x;
        v3CurrentLookAt.y = tmpMatrix._12 + v3CurrentPosition.y;
        v3CurrentLookAt.z = tmpMatrix._13 + v3CurrentPosition.z;
        player.SetLookAt(v3CurrentLookAt);
    }
    //// 추후 생각해서 추가해볼 기능
    {
        // 점프?는 일단 제외
        if (GetAsyncKeyState(VK_SPACE))
        {

        }
        // 낮밤 변경
        if (GetKeyDown('1') == TRUE)
        {
            char chTest[100];
            wsprintf(chTest, "숫자 1 입력\n");
            OutputDebugString(chTest);
        }
        // 카메라 시점 변경
        if (GetKeyDown('2') == TRUE)
        {
            if (bIsSkyView == FALSE)
                bIsSkyView = TRUE;
            else
                bIsSkyView = FALSE;
        }
        // 절두체 컬링
        if (GetKeyDown('3') == TRUE)
        {

        }
    }
    
}

VOID Render()
{
    if (NULL == g_pd3dDevice)
        return;

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        int i, j;
        D3DXMATRIX mtWorld;
        D3DXMatrixIdentity(&mtWorld);
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &mtWorld);

        D3DXMATRIX mtView;
        D3DXVECTOR3 v3CurrentPosition = player.GetPosition();
        D3DXVECTOR3 v3CurrentLookAt = player.GetLookAt();
        // 1인칭 시점
        if (bIsSkyView == FALSE)
        {
            D3DXMatrixLookAtLH(&mtView, &v3CurrentPosition, &v3CurrentLookAt, &v3Up);
            g_pd3dDevice->SetTransform(D3DTS_VIEW, &mtView);
        }
        // 전지적 시점
        else
        {
            D3DXMatrixLookAtLH(&mtView, &v3EyeCeiling, &v3DefaultPosition, &v3UpCeiling);
            g_pd3dDevice->SetTransform(D3DTS_VIEW, &mtView);
        }

        D3DXMATRIX mtProjection;
        D3DXMatrixPerspectiveFovLH(&mtProjection, D3DX_PI / 4, 1.0f, 0.0f, 1000.0f);
        g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &mtProjection);

        D3DXPLANE FrustumPlane[6];
        FrustumPlane[0] = D3DXPLANE(1.0f, 0.0f, 0.0f, 1.0f);
        FrustumPlane[1] = D3DXPLANE(-1.0f, 0.0f, 0.0f, 1.0f);
        FrustumPlane[2] = D3DXPLANE(0.0f, 1.0f, 0.0f, 1.0f);
        FrustumPlane[3] = D3DXPLANE(0.0f, -1.0f, 0.0f, 1.0f);
        FrustumPlane[4] = D3DXPLANE(0.0f, 0.0f, 1.0f, 0.0f);
        FrustumPlane[5] = D3DXPLANE(0.0f, 0.0f, -1.0f, 1.0f);
        for (i = 0; i < 6; i++)
        {
            D3DXPlaneNormalize(&FrustumPlane[i], &FrustumPlane[i]);
        }
        // frustum plane을 계산할, view matrix와 projection matrix의 곱
        D3DXMATRIX mtViewProjection;
        // 하늘에서 바라볼 때, 오브젝트의 LookAt matrix를 따로 계산해야함
        if (bIsSkyView == TRUE)
        {
            D3DXMATRIX mtViewTMP;
            D3DXMatrixLookAtLH(&mtViewTMP, &v3CurrentPosition, &v3CurrentLookAt, &v3Up);
            D3DXMatrixMultiply(&mtViewProjection, &mtViewTMP, &mtProjection);
        }
        else
        {
            D3DXMatrixMultiply(&mtViewProjection, &mtView, &mtProjection);
        }
        D3DXMatrixInverse(&mtViewProjection, NULL, &mtViewProjection);
        D3DXMatrixTranspose(&mtViewProjection, &mtViewProjection);
        D3DXPlaneTransformArray(FrustumPlane, sizeof(D3DXPLANE), FrustumPlane, sizeof(D3DXPLANE), &mtViewProjection, 6);

        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
        g_pd3dDevice->SetTexture(0, g_pTileTexture);
        g_pd3dDevice->SetStreamSource(0, g_pTileVB, 0, sizeof(CUSTOMVERTEX));
        g_pd3dDevice->SetIndices(g_pTileIB);
        //// tile rendering
        // frustum plane과 사각형 단위로 비교한다
        // 네 꼭짓점 중 하나라도 inside이면 rendering한다.
        for (i = 0; i < NUM_OF_ROW * NUM_OF_COLUMN; i++)
        {
            // g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
            // checkFrustumCulling(plane, position, 0.0f)로 판별 가능하겠다
            for (j = 0; j < 4; j++)
            {
                if (CheckFrustumCulling(FrustumPlane, TileVertices[i * 4 + j].v3VerPos, 0.0f) != POSITION_WITH_FRUSTUM::outside)
                {
                    g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
                    break;
                }
            }
        }
        //// wall rendering
        g_pd3dDevice->SetTexture(0, g_pWallTexture);
        g_pd3dDevice->SetStreamSource(0, g_pWallVB, 0, sizeof(CUSTOMVERTEX));
        //// wall culling도 추후 필요
        for (i = 0; i < NUM_OF_ROW * 4; i++)
        {
            g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
        }
        g_pd3dDevice->SetStreamSource(0, g_pWallVB2, 0, sizeof(CUSTOMVERTEX));
        for (i = 0; i < NUM_OF_ROW * 4; i++)
        {
            g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
        }
        g_pd3dDevice->SetStreamSource(0, g_pTmpBlockVB, 0, sizeof(CUSTOMVERTEX));
        for (i = 0; i < 5; i++)
        {
            g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
        }

        // 위치 표시용 구체
        D3DXMATRIX tmpTranspose;
        D3DXMatrixTranslation(&tmpTranspose, v3CurrentPosition.x, v3CurrentPosition.y, v3CurrentPosition.z);
        D3DXMatrixMultiply(&mtWorld, &mtWorld, &tmpTranspose);
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &mtWorld);
        g_pd3dDevice->SetTexture(0, NULL);
        g_pSphere->DrawSubset(0);
        // world matrix 다시 제자리로 돌려주기(필요없음)
        // D3DXMatrixIdentity(&mtWorld);
        // g_pd3dDevice->SetTransform(D3DTS_WORLD, &mtWorld);

        //// 좌상단 UI
        if(FALSE)
        {
            //// Transformed Vertex
            g_pd3dDevice->SetTexture(0, NULL);
            g_pd3dDevice->SetFVF(D3DFVF_UI_VERTEX);
            g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, UIVertices, sizeof(UI_VERTEX));

            //// DrawText
            SetRect(&rt, 20, 20, 0, 0);
            swprintf_s(test2, 255, L"position: %f, %f, %f\nlook at : %f, %f, %f", v3CurrentPosition.x, v3CurrentPosition.y, v3CurrentPosition.z, v3CurrentLookAt.x, v3CurrentLookAt.y, v3CurrentLookAt.z);
            wsprintf(testSTR, "%ws", test2);
            g_pFont->DrawTextA(NULL, testSTR, -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
        
        g_pd3dDevice->EndScene();
    }    
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        CleanUp();
        PostQuitMessage(0);
        return 0;

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}


/*------------------------------------------------------------------------------
 * 이 프로그램의 시작점
 *------------------------------------------------------------------------------
 */
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    // 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "D3D_MyFPS", NULL };
    RegisterClassEx(&wc);
    // 윈도우 생성
    HWND hWnd = CreateWindow("D3D_MyFPS", "D3D_MyFPS",
        WS_OVERLAPPEDWINDOW, 100, 100, 700, 700,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);
    // Direct3D 초기화
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        {
            InitGeometry();
            // 윈도우 출력
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);
            // 메시지 루프
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));

            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    __KeyProc();
                    Render();
                }
            }
        }
    }
    // 등록된 클래스 소거
    UnregisterClass("D3D_MyFPS", wc.hInstance);
    return 0;
}