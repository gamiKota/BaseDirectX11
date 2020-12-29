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
 * @brief 状態の基底クラス
 * @details テンプレートは列挙体の要素を想定
 */
template<typename T>
class State {
private:

	T m_id;	//!< 現在の状態(列挙体の要素)

public:

	/**
	 * @brief デフォルトコンストラクト
	 * @details 削除関数
	 */
	State() = delete;

	/**
	 * @brief コンストラクト
	 * @param id = テンプレート引数
	 */
	explicit State(T id) :m_id(id) {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~State() = default;

	/**
	 * @brief 現在の状態
	 * @return 状態(列挙体の要素)
	 */
	const T& Id() { return m_id; }

	/**
	 * @brief 初期化処理
	 */
	virtual void Start() {}

	/**
	 * @brief 更新処理
	 */
	virtual void Update() {}
	//virtual void Draw() const {}

	/**
	 * @brief 終了処理
	 */
	virtual void OnDestoy() {}

};


/**
 * @class StateMachine<typename T>
 * @brief 状態管理クラス
 * @details テンプレートは列挙体を想定
 */
template<typename T>
class StateMachine : public Component {

private:

	std::list<State<T>*>	m_stateList;	//!< 状態リスト
	State<T>*				m_state;		//!< 現在の状態

public:

	/**
	 * @brief デフォルトコンストラクト
	 */
	StateMachine() = default;
	
	/**
	 * @brief デフォルトデストラクタ
	 */
	virtual ~StateMachine() {
		for (auto state : m_stateList) {
			delete state;
		}
		m_stateList.clear();
	}

	/**
	 * @brief ステートマシンの初期化
	 * @details 純粋仮想関数
	 */
	virtual void Initialize() = 0;

	/**
	 * @更新処理
	 */
	void Update() {
		if (m_state == nullptr) {
			return;
		}
		m_state->Update();
	}
	
	/**
	 * @brief 現在の状態のIDの取得
	 * @return 現在の状態のID
	 */
	const T GetCurrentStateName() const {
		if (m_state == nullptr) {
			return nullptr;
		}
		return m_state->Id();	// 列挙体の要素を返す
	}
	
	/**
	 * @brief 指定の状態へ移行
	 * @return 移行先の状態ID
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
	 * @brief 状態の追加
	 * @param[in] 追加する状態
	 * @param[in] 現在の状態を第一引数の状態に更新
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