// ���E�{�b�N�X �f�o�b�O�\�� [Box.h]
#pragma once
#include "main.h"
#include "Component.h"
#include "ModelManager.h"

class Collision : public Component {
public:
	E_MODEL		m_model;
	XMFLOAT4	m_color;
	XMFLOAT3	m_vCenter;		// ���E�{�b�N�X���S���W
	XMFLOAT3	m_vBBox;		// ���E�{�b�N�X �T�C�Y
	XMFLOAT3	m_vPosBBox;		// ���E�{�b�N�X���S���W(���[���h���)
	XMFLOAT4X4	m_world;		//!< �����蔻��p�s��
	bool		m_bHit;			// �Փ˗L��

	ID3D11Buffer* m_pVertexBuffer;			// ���_�o�b�t�@
	ID3D11Buffer* m_pConstantBuffer[2];		// �萔�o�b�t�@
	ID3D11VertexShader* m_pVertexShader;	// ���_�V�F�[�_
	ID3D11InputLayout* m_pInputLayout;		// ���_�t�H�[�}�b�g
	ID3D11PixelShader* m_pPixelShader;		// �s�N�Z���V�F�[�_
	ID3D11Buffer* m_pIndexBuffer;			// �C���f�b�N�X�o�b�t�@				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

public:
	Collision();
	~Collision();

	void Awake();
	void Start();
	void Uninit();
	void Update();
	void LastUpdate();
	void DebugDraw();

	XMFLOAT4& GetColor() { return m_color; }
	void SetColor(XMFLOAT4* pColor) { m_color = *pColor; }

	void SetWorld(XMFLOAT4X4 world);

	void Init(E_MODEL model);
	static bool AABB(Collision obj1, Collision obj2);
};