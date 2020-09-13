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
	int	m_nLife;	//!< Žõ–½

public:

	void Start();		// ‰Šú‰»
	void Update();		// XV

	void OnCollision(GameObject* obj);
};


// EOF