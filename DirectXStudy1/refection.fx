cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer ReflectionBuffer : register(b1)
{
	float reflectionMatrix;
};

Texture2D shaderTex;
SamplerState sampleType;

Texture2D reflecTex;

struct VInType
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PInType
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPos : TEXCOORD1;
};


PInType VS(VInType i)
{
	PInType o;
	matrix rprojworld;

	i.pos.w = 1.f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	rprojworld = mul(reflectionMatrix, proj);
	rprojworld = mul(world, rprojworld);

	o.reflectionPos = mul(i.pos, rprojworld);

	return o;
}


float4 PS(PInType i) : SV_TARGET
{
	float4 texcolor;
	float2 refTexcoord;
	float4 refcolor;
	float4 color;

	texcolor = shaderTex.Sample(sampleType, i.tex);

	refTexcoord.x = i.reflectionPos.x / i.reflectionPos.w / 2.0f + 0.5f;
	refTexcoord.y = -i.reflectionPos.y / i.reflectionPos.w / 2.0f + 0.5f;

	refcolor = reflecTex.Sample(sampleType, refTexcoord);

	color = lerp(texcolor, refcolor, 0.15f);

	return color;
}

