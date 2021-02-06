struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

cbuffer Material : register(b4) {
	float4	g_Ambient;	// �A���r�G���g�F
	float4	g_Diffuse;	// �f�B�t���[�Y�F
	float4	g_Specular;	// �X�y�L�����F
	float4	g_Emissive;	// �G�~�b�V�u�F
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// �f�o�b�O�p�q�b�g�{�b�N�X�̓e�N�X�`�����ݒ肳��ĂȂ��̂ŁA
// �f�B�q���[�Y�����f����Ȃ�

float4 main(PS_IN PIN) : SV_Target
{
	float4 color = tex.Sample(samp, PIN.uv);
	if (color.a > 0.f) {
		color = g_Diffuse;
	}
	return color;
}
