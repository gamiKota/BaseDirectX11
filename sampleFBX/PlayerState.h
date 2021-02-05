/**
 * @file PlayerState.h
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
 * @enum PLAYER_STATE
 * @brief �v���C���[����̏�ԗ�
 */
enum class PLAYER_STATE {
	IDOL,			// �Î~
	MOVE,			// �ړ�
	TARGET_ON,		// �^�[�Q�b�gON
	TARGET_OFF,		// �^�[�Q�b�gOFF
	ATTACK_BULLET,	// �ˌ��U��
};


class GameObjectUI;


/**
 * @class PlayerState : inheritance StateMachine<template T>
 */
class PlayerState : public StateMachine<PLAYER_STATE> {
private:
	GameObject*		m_target;		//!< �^�[�Q�b�g(�G�Ƒ��݊Ǘ�)
	float3			m_rotate;		//!< �X��
	float3			m_movement;		//!< �ړ���(��΂����ɂ����ėǂ��ϐ�����Ȃ�)
	GameObjectUI*	m_targetLogo;	//!< �^�[�Q�b�g���S
	GameObjectUI*	m_HPGauge;		//!< HP�Q�[�W

public:
	void Awake() { Initialize(); }
	void Update();

	// StateMachine�̃f�X�g���N�^�ɉe������Ȃ�
	// ���������[�N���Ȃ�
	~PlayerState() {}
	void Initialize() override;


	// �Î~���
	struct Idol : public State<PLAYER_STATE> {
		PlayerState* main;
		Idol(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::IDOL), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ړ����
	struct Move : public State<PLAYER_STATE> {
		PlayerState* main;
		Move(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::MOVE), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �^�[�Q�b�g�I��
	struct TargetOn : public State<PLAYER_STATE> {
		PlayerState* main;
		int	m_cnt;
		TargetOn(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::TARGET_ON), main(_main), m_cnt(0) {}
		void Start()		override;		// ��Ԉڍs����
		void Start(bool)	override;		// �ēx�Ăяo��
		void Update()		override;		// ��Ԃ̍Œ�
		void OnDestoy()		override;		// ��ԏI����
	};
	// �^�[�Q�b�g�I�t
	struct TargetOff : public State<PLAYER_STATE> {
		PlayerState* main;
		TargetOff(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::TARGET_OFF), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ˌ����
	struct AttackBullet : public State<PLAYER_STATE> {
		PlayerState* main;
		AttackBullet(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::ATTACK_BULLET), main(_main) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};


	GameObject* GetTarget() { return m_target; }
	void SetTarget(GameObject* target = nullptr) { m_target = target; }
	void SetMovement(float3 move) { m_movement = move; }
};


// EOF