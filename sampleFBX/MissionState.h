#pragma once
#include "State.h"


/**
 * @enum MISSION_STATE
 * @brief ゲームミッションの状態列挙
 */
enum class MISSION_STATE {
	START,			// ミッション開始
	UPDATE,			// ミッション更新
	END_SUCCESS,	// ミッション終了(ミッション成功)
	END_FAILED,		// ミッション終了(ミッション失敗)
	CREATE_ENEMY,	// 敵生成
};


class MissionState : public StateMachine<MISSION_STATE> {
private:

public:
	void Awake() { Initialize(); }
	void Update();

	~MissionState() {}
	void Initialize() override;

	// ミッション開始
	struct StartState : public State<MISSION_STATE> {
		MissionState* m_parent;
		StartState(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::START), m_parent(parent) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ミッション更新
	struct UpdateState : public State<MISSION_STATE> {
		MissionState* m_parent;
		bool m_isEnd;	//!< 終了条件を満たしてる場合true
		UpdateState(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::UPDATE), m_parent(parent) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ミッション終了(成功)
	struct EndSuccess : public State<MISSION_STATE> {
		MissionState* m_parent;
		EndSuccess(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::END_SUCCESS), m_parent(parent) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// ミッション終了(失敗)
	struct EndFailed : public State<MISSION_STATE> {
		MissionState* m_parent;
		EndFailed(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::END_FAILED), m_parent(parent) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};

	// 敵生成
	struct CreateEnemy : public State<MISSION_STATE> {
		MissionState* m_parent;
		CreateEnemy(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::CREATE_ENEMY), m_parent(parent) {}
		void Start()	override;		// 状態移行直後
		void Update()	override;		// 状態の最中
		void OnDestoy() override;		// 状態終了時
	};
};


// EOF