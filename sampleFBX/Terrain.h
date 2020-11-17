// �n�ʃN���X [Land.h]
#pragma once
#include "main.h"
#include "FbxModel.h"

class CLand
{
private:
	CFbxModel*	m_pModel;	// ���f��

	TFbxVertex*	m_pVertex;	// ���_�z��
	int			m_nVertex;	// ���_��
	int*		m_pIndex;	// �C���f�b�N�X�z��
	int			m_nIndex;	// �C���f�b�N�X��

public:
	CLand();
	~CLand();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool Collision(XMFLOAT3* pP0, XMFLOAT3* pW,
		XMFLOAT3* pX, XMFLOAT3* pN = nullptr);
};
