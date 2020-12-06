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
#include "Character.h"



/**
 * @class Bullet : inheritance Character
 */
class Bullet : public Character {
private:
	int	m_nLife;	//!< ����

public:

	void Start();		// ������
	void Update();		// �X�V

	void OnCollision(GameObject* obj);
};


// EOF