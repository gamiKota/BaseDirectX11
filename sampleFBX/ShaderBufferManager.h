#pragma once

#include <Windows.h>
#include <map>
#include <DirectXMath.h>
#include "Singleton.h"


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
struct SHADER_LIGHT_SETTING {
	DirectX::XMFLOAT4 light;
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