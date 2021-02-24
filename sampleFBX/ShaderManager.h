#pragma once


#include "Singleton.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "ShaderBufferManager.h"

enum E_VS {
	VS_NORMAL,		// �ʏ�`��
	VS_2D,			// 2D
	VS_MESH,		// ���b�V��
	VS_2D_FRAME,	// �g�t��
	VS_PROJSHADOW,	// �e
	VS_DEPTHWRITE,	// �[�x�l
	VS_MAX
};

enum E_GS {
	GS_NORMAL,	// �ʏ�`��
	GS_LINE,	// ���C���`��
	GS_MAX,
};

enum E_PS {
	PS_NORMAL,			// �ʏ�`��
	PS_2D,				// 2D
	PS_COLOR,			// �F
	PS_PHONG,			// �t�H��
	PS_HPGAUGE,			// �GHP�Q�[�W
	PS_AREAWALL,		// ��
	PS_2D_FRAME,		// �g�t��
	PS_2D_FRAME_GAUGE,	// �g�t���Q�[�W
	PS_DEPTHWRITE,		// �[�x�l
	PS_DEPTHSHADOW,		// �e
	PS_MAX
};



class ShaderManager : public Singleton<ShaderManager> {
public:
	void Initialize();
	void Terminate();

	void BindVS(E_VS shader);
	void BindPS(E_PS shader);
	void BindGS(E_GS shader);

	void SetTexturePS(ID3D11ShaderResourceView* pTex, UINT slot = 0U);

	void UpdateBuffer(std::string bufName, void *data);

private:
	std::vector<VertexShader*>		m_vsList;	//!< ���_�V�F�[�_���X�g
	std::vector<PixelShader*>		m_psList;	//!< �s�N�Z���V�F�[�_���X�g
	std::vector<GeometryShader*>	m_gsList;	//!< �W�I���g���V�F�[�_���X�g
};


// EOF