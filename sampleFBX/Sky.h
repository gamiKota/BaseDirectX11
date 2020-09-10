// スカイドーム/スカイボックス [Sky.h]
#pragma once
#include "main.h"
#include "Component.h"
#include "Transform.h"

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

	void SetPos(float3 pos);
};

