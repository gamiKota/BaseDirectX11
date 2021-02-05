#include "HPGauge.h"
#include "Transform.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "System.h"

void HPGauge::Start() {

}

void HPGauge::Update() {
	
}

void HPGauge::LastUpdate() {
	SHADER_CHARACTER Char;
	Char.vHpRate = 1.f;
	ShaderManager::GetInstance().UpdateBuffer("Character", &Char);
}

// EOF