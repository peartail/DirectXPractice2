cbuffer MatrixBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer LightBuffer : register(b1)
{
	float4 difColor;
	float3 lightDir;
};

Texture2D shaderTexture[2];
SamplerState SampleType;

struct VInputT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tan : TANGENT;
	float3 binorm : BINORMAL;
};

struct PInputT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tan : TANGENT;
	float3 binorm : BINORMAL;
};

PInputT VS(VInputT i)
{
	PInputT o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);
	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);
	o.normal = normalize(o.normal);

	o.tan = mul(i.tan, (float3x3)world);
	o.tan = normalize(o.tan);

	o.binorm = mul(i.binorm, (float3x3)world);
	o.binorm = normalize(o.binorm);

	return o;
}

float4 PS(PInputT i) : SV_TARGET
{
	float4 texcolor;
	float4 bumpmap;
	float3 bumpnorm;
	float3 lightdir;
	float lintensity;
	float4 color;

	texcolor = shaderTexture[0].Sample(SampleType, i.tex);
	
	bumpmap = shaderTexture[1].Sample(SampleType, i.tex);

	bumpmap = (bumpmap * 2.0f) - 1.0f;

	bumpnorm = i.normal + bumpmap.x * i.tan + bumpmap.y * i.binorm;

	bumpnorm = normalize(bumpnorm);

	lightdir = -lightDir;

	lintensity = saturate(dot(bumpnorm, lightdir));

	color = saturate(difColor * lintensity);

	color = color * texcolor;

	return color;
}