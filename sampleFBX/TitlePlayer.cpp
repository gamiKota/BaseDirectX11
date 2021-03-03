#include "TitlePlayer.h"
#include "Transform.h"
#include "GameObject3D.h"
#include "Frame.h"
#include "System.h"

bool TitlePlayer::m_isCreate = false;

void TitlePlayer::Start() {
	m_time = 0.f;
	m_obj.clear();
}

void TitlePlayer::Update() {
	m_time += Frame::GetInstance().GetDeltaTime();
	m_transform->m_position.z += 50.f;
	m_transform->m_position.y += 8.f;
	m_transform->m_position.x += 15.f;
	m_transform->m_rotation = Quaternion::AngleAxis(-45.f * m_time, float3(0.f, 0.f, 1.f));

	if (m_time > 1.f && !m_isCreate) {
		float3 pos;
		GameObject3D* obj;
		for (int i = 0; i < 15; i++) {

			obj = new GameObject3D(E_MODEL_PLAYER, "Hoka", "Hoka");
	
			pos.x = (float)GetRandom(-3500, -1500);
			pos.z = (float)GetRandom(-3000, -4000);
			pos.y = -2937.f;
	
			m_obj.push_back(GameObject::Instantiate(obj, pos, Quaternion::identity, float3() + 0.5f));
			obj->AddComponent<TitlePlayer>();
			obj->m_vs = E_VS::VS_PROJSHADOW;
			obj->m_ps = E_PS::PS_DEPTHSHADOW;
			obj->m_isInstancing = true;
		}

		m_isCreate = true;
	}

	if (m_time > 8.f) {
		GameObject::Destroy(m_gameObject);
		for (auto obj : m_obj) {
			GameObject::Destroy(obj);
		}
	}
}