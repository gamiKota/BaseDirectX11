/**
 * @file PhongPS.hlsl
 */


 // 頂点シェーダから渡されるデータ
struct PS_IN {
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL0;
	float3 wPos		: TEXCOORD1;
};


cbuffer ValueBuf : register(b5) {
	float4 value;
}


Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN PIN) : SV_Target{

	float4 color;
	color = tex.Sample(samp, PIN.uv);

	// 影の計算
	// 光源のベクトルと法線の内積から暗さを計算する
	float3 lightDir = float3(1, -1, 1);

	// 光源のベクトルを反転(-の値を掛ける)して、正しい見た目になるように計算する
	lightDir = normalize(-lightDir);
	float3 N = normalize(PIN.normal);

	// 内積
	float d = dot(lightDir, N);

	// 1～0の範囲に補正する
	d = d * 0.5f + 0.5f;

	// 照り返し(反射の計算)
	float3 V = PIN.wPos - value.xyz;
	// reflect... 第二引数で渡した法線のベクトルから、第一引数のベクトルがどのように反射するか計算
	float3 R = reflect(V, N);
	R = normalize(R);
	float s = dot(lightDir, R);
	// スペキュラーは1～0の範囲にない値を無視する
	// saturate... 引数を0～1の範囲に丸める
	s = saturate(s);
	// 照り返し部分はより強く際立つように計算結果を乗算する
	s = pow(s, 50);

	// 人間の目は、届いた光から色の判別を行う
	// 光源から進んだ光が直接物体に当たり、反射した光のことを拡散光(Diffuse)と呼ぶ
	// 周辺の物体にあった光が反射して、物体に当たり、さらに反射した光を環境光(Ambient)と呼ぶ
	// 物体そのものの光の反射のしやすさを表すマテリアル(Material)というパラメータもある
	// 輝いて見える光を反射光(Specular)と呼ぶ

	// 光源の色
	float4 diffuseLight		= float4(1, 1, 1, 1);
	// 周りから反射した色
	float4 ambientLight		= float4(0.5f, 0.5f, 0.5f, 1);
	// 輝いて見える光の色
	float4 specularLight	= float4(1, 1, 1, 1);
	// 物体が跳ね返しやすい光
	float4 materialDiffuse	= float4(1, 1, 1, 1);
	// 物体が跳ね返しやすい環境光
	float4 materialAmbient	= float4(1, 1, 1, 1);
	// 物体が金属っぽいか(反射光の係数)
	float4 materialSpecular = float4(50, 10, 10, 1);

	// 光マテリアルを考慮した影つけ
	float3 diffuse = (float3)(diffuseLight * materialDiffuse);
	float3 ambient = (float3)(ambientLight * materialAmbient);
	float3 specular = (float3)(specularLight * materialSpecular);
	float3 lambert = diffuse * d + ambient;
	float3 phong = specular * s;

	color.rgb *= lambert + phong;

	return color;
}


// EOF