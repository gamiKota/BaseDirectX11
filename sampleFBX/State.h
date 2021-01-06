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
	bool m_active;	//!< �X�e�[�g�L���Ǘ�
	T m_id;			//!< ���݂̏��(�񋓑̗̂v�f)

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
	virtual void Start(bool active) {}

	/**
	 * @brief �X�V����
	 */
	virtual void Update() {}
	//virtual void Draw() const {}

	/**
	 * @brief �I������
	 */
	virtual void OnDestoy() {}


	/**
	 * @brief Active�l���Z�b�g
	 */
	void SetActive(bool active) { m_active = active; }

	/**
	 * @brief Active�l�̎擾
	 */
	bool GetActive() { return m_active; }
};


/**
 * @class StateMachine<typename T>
 * @brief ��ԊǗ��N���X
 * @details �e���v���[�g�͗񋓑̂�z��
 *			���ݗL���ȏ�Ԃ��������݂���ꍇ��z��
 */
template<typename T>
class StateMachine : public Component {

private:
	std::list<State<T>*>	m_stateList;	//!< ��ԃ��X�g

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
	 * @brief �X�V����
	 */
	void Update() {
		for (auto state : m_stateList) {
			if (state->GetActive()) {	// �A�N�e�B�u�l���L���̏��
				state->Update();	// �X�V����
			}
		}
	}
	
	/**
	 * @brief ���݂̏�Ԃ�ID�̎擾
	 * @return ���݂̏�Ԃ�ID
	 */
	const bool GetCurrentState(T stateID) const {
		for (auto state : m_stateList) {
			if (state->Id() == stateID && state->GetActive()) {
				return true;	// ��Ԃ��L��
			}
			else if (state->Id() == stateID && !state->GetActive())  {
				return false;	// ��Ԃ�����
			}
		}
		return false;	// ��Ԃ����X�g�ɓo�^����ĂȂ�
	}


	/**
	 * @brief ��̎擾
	 * @return ���(list != classT > nullptr)
	 */
	template<class classT>
	classT* GetState() const {
		for (auto state : m_stateList) {
			classT* buff = dynamic_cast<classT*>(state);
			if (buff != nullptr)
				return buff;
		}
		return nullptr;	// ��Ԃ����X�g�ɓo�^����ĂȂ�
	}

	
	/**
	 * @brief �w��̏�Ԃ̃A�N�e�B�u�ݒ�
	 * @param[in] �A�N�e�B�u���X�V���������
	 * @param[in] �A�N�e�B�u�l
	 */
	void SetStateActive(T stateID, bool active) {
		for (auto state : m_stateList) {
			if (state->Id() == stateID && state->GetActive() != active) {	// ���X�g�ɏ�Ԃ��o�^����āAActive��Ԃ̍X�V��������ꍇ
				state->SetActive(active);
				if (active) {	// ��������L���ɐ؂�ւ�
					state->Start();
				}
				else {	// �L�����疳���ɐ؂�ւ�
					state->OnDestoy();
				}
				break;
			}
			else if (state->Id() == stateID && state->GetActive() == active) {	// �ēx�A�N�e�B�u�l�̍X�V�����������ꍇ
				state->Start(active);
			}
		}
	}
	
	/**
	 * @brief ��Ԃ̒ǉ�
	 * @param[in] �ǉ�������
	 * @param[in] �������̏�Ԃ̃A�N�e�B�u�l�̏����l
	 */
	void AddState(State<T>* state, bool active = false) {
		if (state == nullptr)	return;
		m_stateList.push_back(state);
		SetStateActive(state->Id(), active);
	}
};




// EOF