#include "Material.h"


Material::Material() {
	m_diffuse	= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_ambient	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_specular	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_emissive	= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}


void Material::SetImGuiVal() {
#if _DEBUG
	ImGui::DragFloat4("Diffuse", (float*)&m_diffuse);
	ImGui::DragFloat4("Ambient", (float*)&m_ambient);
	ImGui::DragFloat4("Specular", (float*)&m_specular);
	ImGui::DragFloat4("Emissive", (float*)&m_emissive);
	ImGui::DragFloat("SpecularHighlight", (float*)&m_power);
#endif
}