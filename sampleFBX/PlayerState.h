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

	// StateMachineのデストラクタに影響されない
	// メモリリークしない
	~PlayerState() {

	}

	void Initialize() override;

	// 静止状態
	struct Idol : public State<PLAYER_STATE> {

		PlayerState* main;
		Idol(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::IDOL), main(_main) {}

		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 移動状態。
	struct Move : public State<PLAYER_STATE> {

		PlayerState* main;
		Move(PlayerState* _main) : State<PLAYER_STATE>(PLAYER_STATE::MOVE), main(_main) {}

		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};
};

