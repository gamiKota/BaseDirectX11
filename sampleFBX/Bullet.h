/**
 * @file Bullet.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "Component.h"


/**
 * @class Bullet : inheritance Component
 */
class Bullet : public Component
{
private:
	int			m_nStat;		// 0:���g�p, 1:�g�p��, 2:������
	int			m_nLife;		// ����

public:

	void Start();		// ������
	void Update();		// �X�V

	void OnCollision(GameObject* obj);
	void Fire(Transform transform);	// ����
};


// EOF