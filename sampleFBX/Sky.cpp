// スカイドーム/スカイボックス [Sky.cpp]
#include "Sky.h"
#include "Camera.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "ModelManager.h"
#include "System.h"


// コンストラクタ
CSky::CSky() {

}

// デストラクタ
CSky::~CSky() {
}


void CSky::Awake() {

}

// 初期化
void CSky::Start() {
}

// 終了処理
void CSky::Uninit() {
}

// 更新
void CSky::Update() {
	ModelManager::GetInstance().Update(E_MODEL_MISSILE);
}




void CSky::SetPos(XMFLOAT3 pos) {
	m_transform->m_position = pos;
}