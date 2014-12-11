#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

#include "TextureClass.h"

class ParticleSystemClass
{
private:
	struct ParticleType
	{
		float posx, posy, posz;
		float red, green, blue;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
		D3DXVECTOR4 color;
	};


public:
	ParticleSystemClass();
	~ParticleSystemClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	bool Frame(float, ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

private:
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool InitializeParticleSystem();
	void ShutdownParticleSystem();

	bool InitializeBuffer(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

private:

	float _particleDeviationX, _particleDeviationY, _particleDeviationZ;
	float _particleVelocity, _particleVelocityVariation;
	float _particleSize, particlePerSecond;
	int _maxParticles;

	int _currentParticleCount;
	float _accumuateTime;

	TextureClass* _texture;
	ParticleType *_particleList;

	int _vertexCnt, _indexCnt;
	VertexType* _vertices;
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
};

