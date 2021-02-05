// 2D�p�s�N�Z���V�F�[�_

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

cbuffer world : register(b3) {
	matrix g_mWorld;
	matrix g_mTexture;
};

cbuffer Material : register(b4) {
	float4	g_Ambient;	// �A���r�G���g�F
	float4	g_Diffuse;	// �f�B�t���[�Y�F
	float4	g_Specular;	// �X�y�L�����F
	float4	g_Emissive;	// �G�~�b�V�u�F
}

Texture2D    g_texture : register(t0);	// �e�N�X�`��
SamplerState g_sampler : register(s0);	// �T���v��

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = input.Diffuse;
	Color *= g_texture.Sample(g_sampler, input.TexCoord);
	Color *= g_Diffuse;
	if (Color.a <= 0.f) {
		discard;
	}
	return Color;
}
