#include "BasicShaderHeader.hlsli"

//サンプラーとUV座標を指定して、テクスチャの色を取り出す
Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 PSmain(VSOutput input) : SV_TARGET
{
    /////For texture mapping only
    //return float4(tex.Sample(smp, input.uv));
    //return float4(input.normal, 1); // RGBをそれぞれ法線のXYZ、Aを1で出力

    /////For diffuse calculation
    //float3 light = normalize(float3(1, -1, 1)); // 右下奥 向きのライト
    //float brightness = dot(-light, input.normal); // 光源へのベクトルと法線ベクトルの内積
    //return float4(brightness, brightness, brightness, 1); // 輝度をRGBに代入して出力

    /////Adding Ambient
    float4 texcolor = float4(tex.Sample(smp,input.uv));
    float3 light = normalize(float3(1, -1, 1)); // 右下奥 向きのライト
    float diffuse = saturate(dot(-light, input.normal)); // diffuseを[0,1]の範囲にClampする
    float brightness = diffuse + 0.3f; // アンビエント項を0.3として計算
    return float4(texcolor.rgb * brightness, texcolor.a);//Mapping with texture
    //return float4(brightness, brightness, brightness, 1); // 輝度をRGBに代入して出力
    //return float4(texcolor.rgb * brightness, 1) * color;//Mapping with texture
    //return float4(0, 0, 0, 1);//All black
}

//Basic pixel shader setting

////// Entry Point //////
//float4 PSmain(float4 pos:SV_Position) : SV_TARGET
//{
//	//Original with pixel shader color
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);//R G B A
//}

//To display UV color on the screen

//float4 PSmain(VSOutput input) : SV_TARGET
//{
//	return float4(input.uv, 1, 1);
//}

