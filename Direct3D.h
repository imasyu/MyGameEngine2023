#pragma once
#include <d3d11.h>
#include <assert.h>

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Direct3D
{
	extern ID3D11Device* pDevice;		                //デバイス
	extern ID3D11DeviceContext* pContext;		        //デバイスコンテキスト

	//初期化
	void Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	void InitShader();

	//描画開始
	void BeginDraw();



	//描画終了
	void EndDraw();



	//解放
	void Release();

};

