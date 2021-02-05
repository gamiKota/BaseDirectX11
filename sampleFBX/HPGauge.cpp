#include "HPGauge.h"
#include "Transform.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "System.h"

void HPGauge::Start() {
	m_transform->m_position = float3(135.f, -38.f, 0.f);
	m_transform->m_scale = float3(110.f, 15.f, 0.f);
}

void HPGauge::Update() {
	m_maxHP = 0.f;
}

void HPGauge::LastUpdate() {
	SHADER_CHARACTER Char;
	Char.vHpRate = (m_maxHP <= 0.f) ? 1.f : m_HP / m_maxHP;
	ShaderManager::GetInstance().UpdateBuffer("Character", &Char);
}

// EOF