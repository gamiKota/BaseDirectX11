/**
 * @file EnemyState.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include "State.h"


/**
 * @enum ENEMY_STATE
 * @brief �G�̏�ԗ�
 */
enum class ENEMY_STATE {
	IDOL,			// �Î~
	MOVE,			// �ړ�
	TARGET_ON,		// �^�[�Q�b�gON
	TARGET_OFF,		// �^�[�Q�b�gOFF
	ATTACK_BULLET,	// �ˌ��U��
};


/**
 * @class EnemyState : inheritance StateMachine<template T>
 */
class EnemyState : public StateMachine<ENEMY_STATE> {
private:
	GameObject*		m_target;		//!< �^�[�Q�b�g
	float			m_roll;			//!< �X��(Z��)
	float3			m_movement;		//!< �ړ��x�N�g��

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachine�̃f�X�g���N�^�ɉe������Ȃ�
	// ���������[�N���Ȃ�
	~EnemyState() {}
	void Initialize() override;


	// �Î~���
	struct Idol : public State<ENEMY_STATE> {
		EnemyState* main;
		Idol(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::IDOL), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ړ����
	struct Move : public State<ENEMY_STATE> {
		EnemyState* main;
		Move(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::MOVE), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �^�[�Q�b�g�I��
	struct TargetOn : public State<ENEMY_STATE> {
		EnemyState* main;
		TargetOn(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_ON), main(_main) {}
		void Start()		override;		// ��Ԉڍs����
		void Update()		override;		// ��Ԃ̍Œ�
		void OnDestoy()		override;		// ��ԏI����
	};
	// �^�[�Q�b�g�I�t
	struct TargetOff : public State<ENEMY_STATE> {
		EnemyState* main;
		TargetOff(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_OFF), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ˌ����
	struct AttackBullet : public State<ENEMY_STATE> {
		EnemyState* main;
		AttackBullet(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::ATTACK_BULLET), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	GameObject* GetTarget() { return m_target; }
	void SetTarget(GameObject* target = nullptr) { m_target = target; }
	void SetMovement(float3 move) { m_movement = move; }
};


// EOF