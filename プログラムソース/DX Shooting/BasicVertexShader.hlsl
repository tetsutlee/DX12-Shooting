#include "BasicShaderHeader.hlsli"

//For UV and XY coordinates
VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = pos;
	output.svpos = mul(mat, pos); // 座標に行列を乗算
	output.normal = normal;
	output.uv = uv;
	return output;
}

//Basic Setting
////// Entry Point //////
//float4 VSmain( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

