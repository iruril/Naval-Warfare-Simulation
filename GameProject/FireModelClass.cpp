////////////////////////////////////////////////////////////////////////////////
// Filename: firemodelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "firemodelclass.h"

#define _CRT_SECURE_NO_WARNINGS


FireModelClass::FireModelClass()
{
}


FireModelClass::FireModelClass(const FireModelClass& other)
{
}


FireModelClass::~FireModelClass()
{
}


bool FireModelClass::Initialize(ID3D11Device* device, const char* modelFilename, const WCHAR* textureFilename1, const WCHAR* textureFilename2,
							const WCHAR* textureFilename3)
{
	// �� �����͸� �ε��մϴ�.
	if(!LoadModel(modelFilename))
	{
		return false;
	}

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	return LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
}


void FireModelClass::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTextures();

	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();

	// �� ������ ��ȯ
	ReleaseModel();
}


void FireModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
	RenderBuffers(deviceContext);
}


int FireModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool FireModelClass::InitializeBuffers(ID3D11Device* device)
{
	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void FireModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void FireModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ����������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ����������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool FireModelClass::LoadTextures(ID3D11Device* device, const WCHAR* textureFilename1, const WCHAR* textureFilename2, const WCHAR* textureFilename3)
{
	// ù��° �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture1 = new TextureClass;
	if (!m_Texture1)
	{
		return false;
	}

	// ù��° �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�
	bool result = m_Texture1->Initialize(device, textureFilename1);
	if(!result)
	{
		return false;
	}

	// �ι�° �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture2 = new TextureClass;
	if(!m_Texture2)
	{
		return false;
	}

	// �ι�° �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�
	result = m_Texture2->Initialize(device, textureFilename2);
	if(!result)
	{
		return false;
	}

	// ����° �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture3 = new TextureClass;
	if(!m_Texture3)
	{
		return false;
	}

	// ����° �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�
	result = m_Texture3->Initialize(device, textureFilename3);
	if(!result)
	{
		return false;
	}

	return true;
}


void FireModelClass::ReleaseTextures()
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�
	if(m_Texture1)
	{
		m_Texture1->Shutdown();
		delete m_Texture1;
		m_Texture1 = 0;
	}

	if(m_Texture2)
	{
		m_Texture2->Shutdown();
		delete m_Texture2;
		m_Texture2 = 0;
	}

	if(m_Texture3)
	{
		m_Texture3->Shutdown();
		delete m_Texture3;
		m_Texture3 = 0;
	}
}


ID3D11ShaderResourceView* FireModelClass::GetTexture1()
{
	return m_Texture1->GetTexture();
}


ID3D11ShaderResourceView* FireModelClass::GetTexture2()
{
	return m_Texture2->GetTexture();
}


ID3D11ShaderResourceView* FireModelClass::GetTexture3()
{
	return m_Texture3->GetTexture();
}


bool FireModelClass::LoadModel(const char* filename)
{
	// �� ������ ���ϴ�.
	ifstream fin;
	fin.open(filename);
	
	// ������ �� �� ������ �����մϴ�.
	if(fin.fail())
	{
		return false;
	}

	// ���ؽ� ī��Ʈ�� ������ �д´�.
	char input = 0;
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// ���ؽ� ī��Ʈ�� �д´�.
	fin >> m_vertexCount;

	// �ε����� ���� ���� ���� ���� �����մϴ�.
	m_indexCount = m_vertexCount;

	// �о� ���� ���� ������ ����Ͽ� ���� ����ϴ�.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// �������� ���� �κб��� �д´�.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// ���ؽ� �����͸� �н��ϴ�.
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// �� ������ �ݴ´�.
	fin.close();

	return true;
}


void FireModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}
}