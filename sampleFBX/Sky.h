// スカイドーム/スカイボックス [Sky.h]
#pragma once
#include "main.h"
#include "Component.h"

class CSky : public Component
{
private:

public:
	CSky();
	~CSky();

	void Awake();
	void Start();
	void Uninit();
	void Update();

	void SetPos(DirectX::XMFLOAT3 pos);
};

