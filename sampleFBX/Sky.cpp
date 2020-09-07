// スカイドーム/スカイボックス [Sky.cpp]
#include "Sky.h"
#include "Camera.h"
#include "D3DClass.h"
#include "GameObject.h"
#include "ModelManager.h"
#include "System.h"

// マクロ定義
#define PATH_MODELSKY	"data/model/sky.fbx"

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

// 描画
void CSky::Draw()
{
	ModelManager::GetInstance().Draw(E_MODEL_SKY, m_gameObject->m_transform.m_world);
}


void CSky::SetPos(XMFLOAT3 pos) {
	m_transform->m_world._41 = pos.x;
	m_transform->m_world._42 = pos.y;
	m_transform->m_world._43 = pos.z;
}