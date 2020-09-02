// 敵機クラス [Enemy.cpp]
#include "Enemy.h"
#include "input.h"
#include "D3DClass.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "System.h"

// マクロ定義
#define SPEED			15.0f	// 速さ


// コンストラクタ
CEnemy::CEnemy() : Component() {
}

// デストラクタ
CEnemy::~CEnemy() {
}


void CEnemy::Awake() {
	m_transform = &m_gameObject->m_transform;
}


// 初期化
void CEnemy::Start() {
	
}

// 終了処理
void CEnemy::Uninit() {

}

// 更新
void CEnemy::Update() {

	ModelManager::GetInstance().Update(E_MODEL_ENEMY);

	if (m_transform->_43 < GameObject::Find("Player")->m_transform._43) {
		Destroy(m_gameObject);
	}
}

// 描画
void CEnemy::Draw() {

	ModelManager::GetInstance().Draw(E_MODEL_ENEMY, m_gameObject->m_transform);
}
