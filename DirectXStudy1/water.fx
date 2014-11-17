cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ReflectionBuffer : register(b1)
{
	matrix refMat;
};

cbuffer WaterBuffer
{
	float waterTrans;
	float refRefractScale;
	float2 padding;
};

Texture2D reflectionTex;
Texture2D refractionTex;
Texture2D normalTex;

SamplerState sampleType;

struct VInType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectPos : TEXCOORD1;
	float4 reflactPos : TEXCOORD2;
};

PInType VS(VInType i)
{
	PInType o;
	matrix refprojworld;
	matrix viewprojworld;

	i.pos.w = 1.f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	refprojworld = mul(refMat, proj);
	refprojworld = mul(world, refprojworld);

	o.reflectPos = mul(i.pos, refprojworld);

	viewprojworld = mul(view, proj);
	viewprojworld = mul(world, viewprojworld);

	o.reflactPos = mul(i.pos, viewprojworld);

	return o;
}


float4 PS(PInType i) : SV_TARGET
{
	float2 refTexcoord;
	float2 refractTexcoord;
	float4 normalMap;
	float3 normal;
	float4 refColor;
	float4 refractColor;
	float4 color;

	i.tex.y += waterTrans;

	refTexcoord.x = i.reflectPos.x / i.reflectPos.w / 2.0f + 0.5f;
	refTexcoord.y = -i.reflectPos.y / i.reflectPos.w / 2.0f + 0.5f;

	refractTexcoord.x = i.reflactPos.x / i.reflactPos.w / 2.0f + 0.5f;
	refractTexcoord.y = -i.reflactPos.y / i.reflactPos.w / 2.0f + 0.5f;

	normalMap = normalTex.Sample(sampleType, i.tex);
	normal = (normalMap.xyz * 2.0f) - 1.0f;

	refTexcoord = refTexcoord + (normal.xy * refRefractScale);
	refractTexcoord = refractTexcoord + (normal.xy + refRefractScale);

	refColor = reflectionTex.Sample(sampleType, refTexcoord);
	refractColor = refractionTex.Sample(sampleType, refractTexcoord);

	color = lerp(refColor, refractColor, 0.6f);

	//color = float4(1, 1, 1, 1);

	return color;
}

