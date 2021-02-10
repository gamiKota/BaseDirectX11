#pragma once
#include "Component.h"
#include <functional>

class Material;

constexpr DirectX::XMFLOAT4 PLAYER_UI_DIFFUSE = DirectX::XMFLOAT4(0.f, 0.9f, 1.f, 1.f);
constexpr DirectX::XMFLOAT4 PLAYER_UI_IN_DIFFUSE = DirectX::XMFLOAT4(0.f, 0.9f, 1.f, 0.2f);
constexpr DirectX::XMFLOAT4 PLAYER_UI_ERROR = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);

class PlayerUI : public Component {
private:
	bool m_isError;
	float m_errorTime;

	bool m_isSwith;
	float m_swithTime;

	Material* m_material;

public:
	void Start();
	void Update();

	void InitError();
	PlayerUI* SetError(float errorTime = 2.f);

	void OnComplete(std::function<void()> func) {
		m_errorFunc = func;
	}

	std::function<void()> m_errorFunc;
};

// EOF