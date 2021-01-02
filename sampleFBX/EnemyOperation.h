/**
 * @file EnemyOperation.h
 * @brief �G�̏�Ԉ˗����e�[�g�}�V���ɑ���R���|�[�l���g
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
 * @class EnemyOperation : inheritance StateMachine<template T>
 * @brief �����I��AI�̕���
 */
class EnemyOperation : public Component {
public:
	//EnemyState* m_stateMachine;	//!< ��ԊǗ��N���X
	//StatusVal m_bullet;			//!< �ˌ�����
	//StatusVal m_targetDist;		//!< ���݃^�[�Q�b�g�ɂ��Ă�I�u�W�F�N�g�Ƃ̋���
	//float3 m_movement;			//!< �ړ���

public:
	void Awake();
	void Start();
	void Update();
};


// EOF