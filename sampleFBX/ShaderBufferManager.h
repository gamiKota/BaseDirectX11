#pragma once

#include <Windows.h>
#include <map>
#include <DirectXMath.h>
#include "Singleton.h"

#define MAX_BONE_MATRIX	256

class ShaderBuffer;


// カメラの定数バッファ
struct SHADER_CAMERA {
	DirectX::XMVECTOR	vEye;	// 視点座標
	DirectX::XMMATRIX	mV;		// ビュー
	DirectX::XMMATRIX	mP;		// プロジェクション
};

// ライトの定数バッファ
struct SHADER_LIGHT {
	DirectX::XMVECTOR	vLightDir;	// 光源方向
	DirectX::XMVECTOR	vLa;		// 光源色(アンビエント)
	DirectX::XMVECTOR	vLd;		// 光源色(ディフューズ)
	DirectX::XMVECTOR	vLs;		// 光源色(スペキュラ)
};

// ライトのON/OFF(1/16)
// bool型なんでダメなん
struct SHADER_LIGHT_SETTING {
	//bool light;
	//bool dummy[15];
	DirectX::XMFLOAT4 light;
};

// 描画オブジェクトの行列情報
struct SHADER_WORLD {
	DirectX::XMMATRIX mWorld;	// ワールド行列
	DirectX::XMMATRIX mTexture;	// テクスチャ行列
};

// マテリアル情報
struct SHADER_MATERIAL {
	DirectX::XMVECTOR	vAmbient;	// アンビエント色
	DirectX::XMVECTOR	vDiffuse;	// ディフューズ色
	DirectX::XMVECTOR	vSpecular;	// スペキュラ色
	DirectX::XMVECTOR	vEmissive;	// エミッシブ色
};

// シェーダに渡すボーン行列配列
struct SHADER_BONE {
	DirectX::XMMATRIX mBone[MAX_BONE_MATRIX];
	SHADER_BONE()
	{
		for (int i = 0; i < MAX_BONE_MATRIX; i++) {
			mBone[i] = DirectX::XMMatrixIdentity();
		}
	}
};


class ShaderBufferManager : public Singleton<ShaderBufferManager> {
public:
	std::map<std::string, int> m_registerMap;
	std::map<std::string, ShaderBuffer *> m_bufferMap;

	void Initialize();
	void Terminate();

	void Update(std::string bufName, void* pData);
	void Bind(std::string bufName);

private:
	void Create(std::string bufName, UINT size, int regNum);
};