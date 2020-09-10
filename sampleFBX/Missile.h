// �z�[�~���O�~�T�C���N���X [Missile.h]
#pragma once
#include "Component.h"


class Bullet : public Component
{
private:
	int			m_nStat;		// 0:���g�p, 1:�g�p��, 2:������
	int			m_nLife;		// ����

public:
	Bullet();		// �R���X�g���N�^
	~Bullet();	// �f�X�g���N�^

	void Awake();
	void Start();		// ������
	void Uninit();		// �I������
	void Update();		// �X�V

	void OnCollision(GameObject* obj);

	void Fire(Transform transform);	// ����


	//XMFLOAT3 GetPos();		// ���W�擾
	//void SetPos(XMFLOAT3& vPos);
	//void SetWorld(XMFLOAT4X4& mWorld);
	//XMFLOAT4X4& GetWorld();	// ���[���h�ϊ��擾
	//void GetBBoxInfo(XMFLOAT3& vPos, XMFLOAT3& vBox);
	//void SetHit(bool bHit);
};
