/**
 * @file Collision.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"
#include "ModelManager.h"
#include <d3d11.h>


/**
 * @class Collision : inheritance Component
 */
class Collision : public Component {
public:
	E_MODEL				m_model;
	DirectX::XMFLOAT4	m_color;
	DirectX::XMFLOAT3	m_vCenter;		//!< ���E�{�b�N�X���S���W
	DirectX::XMFLOAT3	m_vBBox;		//!< ���E�{�b�N�X �T�C�Y
	DirectX::XMFLOAT3	m_vPosBBox;		//!< ���E�{�b�N�X���S���W(���[���h���)
	DirectX::XMFLOAT4X4	m_world;		//!< �����蔻��p�s��
	bool				m_bHit;			//!< �Փ˗L��

	ID3D11Buffer* m_pVertexBuffer;			//!< ���_�o�b�t�@
	ID3D11Buffer* m_pConstantBuffer[2];		//!< �萔�o�b�t�@
	ID3D11VertexShader* m_pVertexShader;	//!< ���_�V�F�[�_
	ID3D11InputLayout* m_pInputLayout;		//!< ���_�t�H�[�}�b�g
	ID3D11PixelShader* m_pPixelShader;		//!< �s�N�Z���V�F�[�_
	ID3D11Buffer* m_pIndexBuffer;			//!< �C���f�b�N�X�o�b�t�@				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

private:
	bool m_isInit;

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void DebugDraw();

	DirectX::XMFLOAT4& GetColor() { return m_color; }
	void SetColor(DirectX::XMFLOAT4* pColor) { m_color = *pColor; }

	static bool AABB(Collision obj1, Collision obj2);
	static bool OBB(Collision obj1, Collision obj2);

private:
	void Init(E_MODEL model);
	void SetWorld(DirectX::XMFLOAT4X4 world);
};


// EOF