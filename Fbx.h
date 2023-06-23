#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Direct3D.h"
#include "Transform.h"
#include "Texture.h"

#pragma comment(lib, "LibFbxSDK-Md.lib")
#pragma comment(lib, "LibXml2-Md.lib")
#pragma comment(lib, "zlib-Md.lib")

class Fbx
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
	};

	struct VERTEX
	{
		XMVECTOR position;
	};
protected:
	int vertexCount_;     //���_��
	int polygonCount_;    //�|���S����
	int indexCount_;      //�C���f�b�N�X��

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	Texture* pTexture_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void PassDataToCB(Transform transform);
	void SetBufferToPipeline();
	void    Draw(Transform& transform);
	void    Release();
};


