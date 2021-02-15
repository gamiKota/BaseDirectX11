/**
 * @file Character.h
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
 * @forward declaration
 */
class Status;
class CollisionBox;
class Collider;
class GameObjectMesh;


/**
 * @class Character : inheritance Component
 */
class Character : public Component {
public:
	void Init();

public:
	Status*		m_status;		//!< �X�e�[�^�X
	CollisionBox*	m_collision;	//!< �����蔻��
	Collider*	m_collider;		//!< �R���C�_�[
};


// EOF