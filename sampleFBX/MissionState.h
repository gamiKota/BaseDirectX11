#pragma once
#include "State.h"


/**
 * @enum MISSION_STATE
 * @brief �Q�[���~�b�V�����̏�ԗ�
 */
enum class MISSION_STATE {
	START,			// �~�b�V�����J�n
	UPDATE,			// �~�b�V�����X�V
	END_SUCCESS,	// �~�b�V�����I��(�~�b�V��������)
	END_FAILED,		// �~�b�V�����I��(�~�b�V�������s)
	CREATE_ENEMY,	// �G����
};


class MissionState : public StateMachine<MISSION_STATE> {
private:

public:
	void Awake() { Initialize(); }
	void Update();

	~MissionState() {}
	void Initialize() override;

	// �~�b�V�����J�n
	struct StartState : public State<MISSION_STATE> {
		MissionState* m_parent;
		StartState(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::START), m_parent(parent) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �~�b�V�����X�V
	struct UpdateState : public State<MISSION_STATE> {
		MissionState* m_parent;
		bool m_isEnd;	//!< �I�������𖞂����Ă�ꍇtrue
		UpdateState(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::UPDATE), m_parent(parent) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �~�b�V�����I��(����)
	struct EndSuccess : public State<MISSION_STATE> {
		MissionState* m_parent;
		EndSuccess(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::END_SUCCESS), m_parent(parent) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �~�b�V�����I��(���s)
	struct EndFailed : public State<MISSION_STATE> {
		MissionState* m_parent;
		EndFailed(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::END_FAILED), m_parent(parent) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};

	// �G����
	struct CreateEnemy : public State<MISSION_STATE> {
		MissionState* m_parent;
		CreateEnemy(MissionState* parent) : State<MISSION_STATE>(MISSION_STATE::CREATE_ENEMY), m_parent(parent) {}
		void Start()	override;		// ��Ԉڍs����
		void Update()	override;		// ��Ԃ̍Œ�
		void OnDestoy() override;		// ��ԏI����
	};
};


// EOF