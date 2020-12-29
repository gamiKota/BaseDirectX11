/**
 * @file State.h
 */


/**
 * @include guard
 */
#pragma once


/**
 * @include
 */
#include <list>
#include "Component.h"



/**
 * @class Statet<typename T>
 * @brief ��Ԃ̊��N���X
 * @details �e���v���[�g�͗񋓑̗̂v�f��z��
 */
template<typename T>
class State {
private:

	T m_id;	//!< ���݂̏��(�񋓑̗̂v�f)

public:

	/**
	 * @brief �f�t�H���g�R���X�g���N�g
	 * @details �폜�֐�
	 */
	State() = delete;

	/**
	 * @brief �R���X�g���N�g
	 * @param id = �e���v���[�g����
	 */
	explicit State(T id) :m_id(id) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~State() = default;

	/**
	 * @brief ���݂̏��
	 * @return ���(�񋓑̗̂v�f)
	 */
	const T& Id() { return m_id; }

	/**
	 * @brief ����������
	 */
	virtual void Start() {}

	/**
	 * @brief �X�V����
	 */
	virtual void Update() {}
	//virtual void Draw() const {}

	/**
	 * @brief �I������
	 */
	virtual void OnDestoy() {}

};


/**
 * @class StateMachine<typename T>
 * @brief ��ԊǗ��N���X
 * @details �e���v���[�g�͗񋓑̂�z��
 */
template<typename T>
class StateMachine : public Component {

private:

	std::list<State<T>*>	m_stateList;	//!< ��ԃ��X�g
	State<T>*				m_state;		//!< ���݂̏��

public:

	/**
	 * @brief �f�t�H���g�R���X�g���N�g
	 */
	StateMachine() = default;
	
	/**
	 * @brief �f�t�H���g�f�X�g���N�^
	 */
	virtual ~StateMachine() {
		for (auto state : m_stateList) {
			delete state;
		}
		m_stateList.clear();
	}

	/**
	 * @brief �X�e�[�g�}�V���̏�����
	 * @details �������z�֐�
	 */
	virtual void Initialize() = 0;

	/**
	 * @�X�V����
	 */
	void Update() {
		if (m_state == nullptr) {
			return;
		}
		m_state->Update();
	}
	
	/**
	 * @brief ���݂̏�Ԃ�ID�̎擾
	 * @return ���݂̏�Ԃ�ID
	 */
	const T GetCurrentStateName() const {
		if (m_state == nullptr) {
			return nullptr;
		}
		return m_state->Id();	// �񋓑̗̂v�f��Ԃ�
	}
	
	/**
	 * @brief �w��̏�Ԃֈڍs
	 * @return �ڍs��̏��ID
	 */
	void GoToState(T nextStateId) {
		for (auto state : m_stateList) {
			if (state->Id() == nextStateId) {
				if (m_state != nullptr) {
					m_state->OnDestoy();
				}
				m_state = state;
				m_state->Start();
			}
		}
	}
	
	/**
	 * @brief ��Ԃ̒ǉ�
	 * @param[in] �ǉ�������
	 * @param[in] ���݂̏�Ԃ�������̏�ԂɍX�V
	 */
	void AddState(State<T>* state, bool isNextState = false) {
		if (state == nullptr)	return;
		m_stateList.push_back(state);
		if (isNextState) {
			GoToState(state->Id());
		}
	}
};




// EOF