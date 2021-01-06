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
	bool m_active;	//!< ステート有効管理
	T m_id;			//!< 現在の状態(列挙体の要素)

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
	virtual void Start(bool active) {}

	/**
	 * @brief 更新処理
	 */
	virtual void Update() {}
	//virtual void Draw() const {}

	/**
	 * @brief 終了処理
	 */
	virtual void OnDestoy() {}


	/**
	 * @brief Active値をセット
	 */
	void SetActive(bool active) { m_active = active; }

	/**
	 * @brief Active値の取得
	 */
	bool GetActive() { return m_active; }
};


/**
 * @class StateMachine<typename T>
 * @brief 状態管理クラス
 * @details テンプレートは列挙体を想定
 *			現在有効な状態が複数存在する場合を想定
 */
template<typename T>
class StateMachine : public Component {

private:
	std::list<State<T>*>	m_stateList;	//!< 状態リスト

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
	 * @brief 更新処理
	 */
	void Update() {
		for (auto state : m_stateList) {
			if (state->GetActive()) {	// アクティブ値が有効の状態
				state->Update();	// 更新処理
			}
		}
	}
	
	/**
	 * @brief 現在の状態のIDの取得
	 * @return 現在の状態のID
	 */
	const bool GetCurrentState(T stateID) const {
		for (auto state : m_stateList) {
			if (state->Id() == stateID && state->GetActive()) {
				return true;	// 状態が有効
			}
			else if (state->Id() == stateID && !state->GetActive())  {
				return false;	// 状態が無効
			}
		}
		return false;	// 状態がリストに登録されてない
	}


	/**
	 * @brief 状の取得
	 * @return 状態(list != classT > nullptr)
	 */
	template<class classT>
	classT* GetState() const {
		for (auto state : m_stateList) {
			classT* buff = dynamic_cast<classT*>(state);
			if (buff != nullptr)
				return buff;
		}
		return nullptr;	// 状態がリストに登録されてない
	}

	
	/**
	 * @brief 指定の状態のアクティブ設定
	 * @param[in] アクティブを更新したい状態
	 * @param[in] アクティブ値
	 */
	void SetStateActive(T stateID, bool active) {
		for (auto state : m_stateList) {
			if (state->Id() == stateID && state->GetActive() != active) {	// リストに状態が登録されて、Active状態の更新がかかる場合
				state->SetActive(active);
				if (active) {	// 無効から有効に切り替え
					state->Start();
				}
				else {	// 有効から無効に切り替え
					state->OnDestoy();
				}
				break;
			}
			else if (state->Id() == stateID && state->GetActive() == active) {	// 再度アクティブ値の更新がかかった場合
				state->Start(active);
			}
		}
	}
	
	/**
	 * @brief 状態の追加
	 * @param[in] 追加する状態
	 * @param[in] 第一引数の状態のアクティブ値の初期値
	 */
	void AddState(State<T>* state, bool active = false) {
		if (state == nullptr)	return;
		m_stateList.push_back(state);
		SetStateActive(state->Id(), active);
	}
};




// EOF