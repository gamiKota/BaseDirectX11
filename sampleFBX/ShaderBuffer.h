#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <map>
#include "Singleton.h"


/**
 * @brief シェーダの定数バッファにデータを渡す
 */
class ShaderBuffer
{
public:
	/**
	 * @brief コンストラクタ
	 */
	ShaderBuffer();

	/**
	 * @brief デストラクタ
	 */
	virtual ~ShaderBuffer();

	/**
	 * @brief 生成
	 * @return HRESULT
	 */
	HRESULT Create(UINT size, bool f = true);
	void UpdateSource(void* pData);
	void BindVS(UINT slot);
	void BindPS(UINT slot);
	void BindGS(UINT slot);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }

private:
	ID3D11Buffer* m_pBuffer;
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


// EOF