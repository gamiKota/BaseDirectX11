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

	MAX,
};


/**
 * @class EnemyState : inheritance StateMachine<template T>
 */
class EnemyState : public StateMachine<ENEMY_STATE> {
public:
	float3 m_rotate;	//!< ��]�ʂ̕ێ�

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachine�̃f�X�g���N�^�ɉe������Ȃ�
	// ���������[�N���Ȃ�
	~EnemyState() {}
	void Initialize() override;


	// �Î~���
	struct Idol : public State<ENEMY_STATE> {
		EnemyState* m_main;
		Idol(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::IDOL), m_main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ړ����
	struct Move : public State<ENEMY_STATE> {
		EnemyState* m_main;
		float3	m_movement;
		Move(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::MOVE), m_main(_main), m_movement(float3()) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �^�[�Q�b�g�I��
	struct TargetOn : public State<ENEMY_STATE> {
		EnemyState*		m_main;
		GameObject*		m_target;	//!< �^�[�Q�b�g
		float			m_maxAngle;	//!< �����p�x

		TargetOn(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_ON),
			m_main(_main), m_target(nullptr), m_maxAngle(180.f)
		{}

		void Start()		override;		// ��Ԉڍs����
		void Update()		override;		// ��Ԃ̍Œ�
		void OnDestoy()		override;		// ��ԏI����
		void SetTarget(GameObject* target = nullptr) { m_target = target; }
		void SetMaxAngle(float maxAngle) { m_maxAngle = maxAngle; }
	};
	// �^�[�Q�b�g�I�t
	struct TargetOff : public State<ENEMY_STATE> {
		EnemyState* m_main;
		TargetOff(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::TARGET_OFF), m_main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ˌ����
	struct AttackBullet : public State<ENEMY_STATE> {
		EnemyState* m_main;
		AttackBullet(EnemyState* _main) : State<ENEMY_STATE>(ENEMY_STATE::ATTACK_BULLET), m_main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};
};


// EOF