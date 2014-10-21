cbuffer MatrixBuffer : register (b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer CameraBuffer : register(b2)
{
	float3 cameraPos;
	float padding;
};

cbuffer LightBuffer : register(b1)
{
	float4 ambientColor;
	float4 diffuse;
	float3 lightdir;
	float specularPower;
	float4 specularColor;
};

struct VInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewdir : TEXCOORD1;
};

Texture2D g_tex;
SamplerState g_sample;

PInput VS(VInput i)
{
	PInput o;

	i.pos.w = 1.0f;

	o.pos = mul(i.pos, world);

	o.viewdir = cameraPos.xyz - o.pos.xyz;

	o.viewdir = normalize(o.viewdir);

	o.pos = mul(o.pos, view);
	o.pos = mul(o.pos, proj);

	o.tex = i.tex;

	o.normal = mul(i.normal, (float3x3)world);

	o.normal = normalize(o.normal);

	return o;
}

float4 PS(PInput i) : SV_TARGET
{
	float4 texColor;
	float3 ld;
	float lightIntensity;
	float4 color;

	float3 reflection;
	float4 specular;

	texColor = g_tex.Sample(g_sample, i.tex);

	color = ambientColor;

	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ld = -lightdir;

	lightIntensity = saturate(dot(i.normal, ld));

	if (lightIntensity > 0.0f)
	{
		color += (diffuse * lightIntensity);

		color = saturate(color);

		reflection = normalize(2 * lightIntensity * i.normal - lightdir);
		
		specular = pow(saturate(dot(reflection, i.viewdir)), specularPower);
	}

	//color = saturate(color);

	color = color * texColor;

	color = saturate(color + specular);

	return color;
}

