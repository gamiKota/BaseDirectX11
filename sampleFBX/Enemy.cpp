// 敵機クラス [Enemy.cpp]
#include "Enemy.h"
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

// 初期化
void CEnemy::Start() {
	
}

// 終了処理
void CEnemy::Uninit() {

}

// 更新
void CEnemy::Update() {
	//if (m_transform->m_position.z < GameObject::Find("Player")->m_transform->m_position.z) {
	//	Destroy(m_gameObject);
	//}
}


// EOF