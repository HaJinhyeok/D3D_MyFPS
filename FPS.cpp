#include "FPS_function.h"
#include "Input.h"

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pTileVB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pTileIB = NULL;
LPDIRECT3DTEXTURE9 g_pTileTexture = NULL;

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

    D3DXCreateTextureFromFile(g_pd3dDevice, TILE_TEXTURE_NAME, &g_pTileTexture);
    // tile vertex 좌표 입력
    {
        for (i = 0; i < NUM_OF_TILE * NUM_OF_TILE; i++)
        {
            FLOAT fCoX = (FLOAT)((i % NUM_OF_TILE) * LENGTH_OF_TILE) - NUM_OF_TILE * LENGTH_OF_TILE / 2.0f;
            FLOAT fCoZ = (FLOAT)(-(i / NUM_OF_TILE) * LENGTH_OF_TILE) + NUM_OF_TILE * LENGTH_OF_TILE / 2.0f;
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
    WORD wTileIndices[2 * NUM_OF_TILE * NUM_OF_TILE][3];
    {
        j = 0;
        for (i = 0; i < NUM_OF_TILE * NUM_OF_TILE; i++)
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
}

VOID CleanUp()
{

}

VOID __KeyProc()
{

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
        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
        g_pd3dDevice->SetTexture(0, g_pTileTexture);
        g_pd3dDevice->SetStreamSource(0, g_pTileVB, 0, sizeof(CUSTOMVERTEX));
        g_pd3dDevice->SetIndices(g_pTileIB);
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &mtWorld);

        D3DXMATRIX mtView;
        D3DXMatrixLookAtLH(&mtView, &v3Eye, &v3LookAt, &v3Up);
        g_pd3dDevice->SetTransform(D3DTS_VIEW, &mtView);

        D3DXMATRIX mtProjection;
        D3DXMatrixPerspectiveFovLH(&mtProjection, D3DX_PI / 3, 1.0f, 0.0f, 1000.0f);
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
            D3DXMatrixLookAtLH(&mtViewTMP, &v3EyeDefault, &v3CurrentLookAt, &v3UpDefault);
            D3DXMatrixMultiply(&mtViewProjection, &mtViewTMP, &mtProjection);
        }
        else
        {
            D3DXMatrixMultiply(&mtViewProjection, &mtView, &mtProjection);
        }
        D3DXMatrixInverse(&mtViewProjection, NULL, &mtViewProjection);
        D3DXMatrixTranspose(&mtViewProjection, &mtViewProjection);
        D3DXPlaneTransformArray(FrustumPlane, sizeof(D3DXPLANE), FrustumPlane, sizeof(D3DXPLANE), &mtViewProjection, 6);

        //// tile culling
            // frustum plane과 사각형 단위로 비교한다
            // 네 꼭짓점 중 하나라도 inside이면 rendering한다.
        for (i = 0; i < NUM_OF_TILE * NUM_OF_TILE; i++)
        {
            // checkFrustumCulling(plane, position, 0.0f)로 판별 가능하겠다
            for (j = 0; j < 4; j++)
            {
                if (CheckFrustumCulling(FrustumPlane, TileVertices[i * 4 + j].v3VerPos, 0.0f) == TRUE)
                {
                    g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2);
                    break;
                }
            }
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