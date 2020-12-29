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


enum PLAYER_STATE {
	IDOL,
	MOVE,
	ATTACK_BULLET,
};


class PlayerState : public StateMachine<PLAYER_STATE> {

public:


	void Awake() {
		Initialize();
	}

	// StateMachine�̃f�X�g���N�^�ɉe������Ȃ�
	// ���������[�N���Ȃ�
	~PlayerState() {

	}

	void Initialize() override;

	// �Î~���
	struct Idol : public State<PLAYER_STATE> {

		PlayerState* main;
		Idol(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::IDOL), main(_main) {}

		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �ړ���ԁB
	struct Move : public State<PLAYER_STATE> {

		PlayerState* main;
		Move(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::MOVE), main(_main) {}

		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};
};

