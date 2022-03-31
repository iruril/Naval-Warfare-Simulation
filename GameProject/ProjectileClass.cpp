///////////////////////////////////////////////////////////////////////////////
// Filename: ProjectileClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ProjectileClass.h"
#define _CRT_SECURE_NO_WARNINGS

ProjectileClass::ProjectileClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_projectile = 0;

	m_collider = 0;//콜라이더 변수 추가
	Max = May = Maz = 0.0f;//모델 최대 좌표
	Mix = Miy = Miz = 100.0f;//모델 최소 좌표
}


ProjectileClass::ProjectileClass(const ProjectileClass& other)
{
}


ProjectileClass::~ProjectileClass()
{
}


bool ProjectileClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	m_collider = new ColliderType;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ProjectileClass::Shutdown()
{
	delete m_collider;

	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ProjectileClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

XMMATRIX ProjectileClass::ProjectileShoot(XMVECTOR turretPos, float camYaw, float camPitch) {
	XMMATRIX projectilePos;

	if (shooted == true) {
		initProjectileControl(turretPos, camYaw, camPitch);
		shooted == false;
	}
	if (inWater == true) {
		scaleMatrix_projectile = XMMatrixScaling(0, 0, 0);
		ProjReload(turretPos);
	}
	else {
		scaleMatrix_projectile = XMMatrixScaling(2.2f, 2.2f, 2.2f);
	}

	projectilePos = ProjectileControl();

	return projectilePos;
}

void ProjectileClass::initProjectileControl(XMVECTOR turretPos, float camYaw, float camPitch) {
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	projectilePosition = turretPos;
	projectilePitch = camPitch + XMConvertToRadians(8);
	projectileYaw = camYaw;

	rotateMatrix_projectile = XMMatrixRotationRollPitchYaw(projectilePitch, camYaw, 0);

	projectileTarget = XMVector3TransformNormal(DefaultForward, rotateMatrix_projectile);

	projectileRight = XMVector3TransformCoord(DefaultRight, rotateMatrix_projectile);
	projectileForward = XMVector3TransformCoord(DefaultForward, rotateMatrix_projectile);
	projectileUp = XMVector3Cross(projectileForward, projectileRight);

	shooted = true;
}

XMMATRIX ProjectileClass::ProjectileControl()
{
	if (XMVectorGetY(projectilePosition) < -1.0f) {
		inWater = true;
	}

	projectilePitch -= XMConvertToRadians(0.03);

	rotateMatrix_projectile = XMMatrixRotationRollPitchYaw(projectilePitch, projectileYaw, 0);

	projectileTarget = XMVector3TransformNormal(DefaultForward, rotateMatrix_projectile);

	projectileRight = XMVector3TransformCoord(DefaultRight, rotateMatrix_projectile);
	projectileForward = XMVector3TransformCoord(DefaultForward, rotateMatrix_projectile);
	projectileUp = XMVector3Cross(projectileForward, projectileRight);

	moveUpDown -= gravityForce;
	moveBackForward = -speed;
	projectilePosition += moveLeftRight * projectileRight;
	projectilePosition += moveBackForward * projectileForward;

	projectileTarget = projectilePosition + projectileTarget;

	transMatrix_projectile = XMMatrixTranslation(XMVectorGetX(projectilePosition), XMVectorGetY(projectilePosition), XMVectorGetZ(projectilePosition));

	worldMatrix_projectile = scaleMatrix_projectile * rotateMatrix_projectile * transMatrix_projectile;

	return worldMatrix_projectile;
}

void  ProjectileClass::ProjReload(XMVECTOR turretPos) {
	shooted = false;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;
	moveUpDown = 0.0f;

	projectilePosition = turretPos;

	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	projectileTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	projectileUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	projectileForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	projectileRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

}


int ProjectileClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ProjectileClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ProjectileClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_projectile[i].x, m_projectile[i].y, m_projectile[i].z);
		vertices[i].texture = XMFLOAT2(m_projectile[i].tu, m_projectile[i].tv);
		vertices[i].normal = XMFLOAT3(m_projectile[i].nx, m_projectile[i].ny, m_projectile[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ProjectileClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ProjectileClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ProjectileClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void ProjectileClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ProjectileClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	char readfile[50];
	strcpy_s(readfile, 50, filename);
	strcat_s(readfile, 50, txt);

	ObjconvertToTxt(filename, readfile);

	// Open the model file.
	fin.open(readfile);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_projectile = new ModelType[m_vertexCount];
	if (!m_projectile)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_projectile[i].x >> m_projectile[i].y >> m_projectile[i].z;
		fin >> m_projectile[i].tu >> m_projectile[i].tv;
		fin >> m_projectile[i].nx >> m_projectile[i].ny >> m_projectile[i].nz;
	}

	// Close the model file.
	fin.close();

	remove((CHAR*)readfile);
	return true;
}

void ProjectileClass::ReleaseModel()
{
	if (m_projectile)
	{
		delete[] m_projectile;
		m_projectile = 0;
	}

	return;
}

//콜라이더 값 반환 함수 추가*********************************
float ProjectileClass::GetMaxX()
{
	return m_collider->Mx;
}

float ProjectileClass::GetMinX()
{
	return m_collider->mx;
}

float ProjectileClass::GetMaxY()
{
	return m_collider->My;
}

float ProjectileClass::GetMinY()
{
	return m_collider->my;
}

float ProjectileClass::GetMaxZ()
{
	return m_collider->Mz;
}

float ProjectileClass::GetMinZ()
{
	return m_collider->mz;
}
//***********************************************************

void ProjectileClass::SetCollider(XMMATRIX scale, XMMATRIX trans)
{
	//x-----------------------
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (Max < m_projectile[i].x)
		{
			Max = m_projectile[i].x;
		}
	}
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (Mix > m_projectile[i].x)
		{
			Mix = m_projectile[i].x;
		}
	}
	//y-----------------------
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (May < m_projectile[i].y)
		{
			May = m_projectile[i].y;
		}
	}
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (Miy > m_projectile[i].y)
		{
			Miy = m_projectile[i].y;
		}
	}
	//z-----------------------
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (Maz < m_projectile[i].z)
		{
			Maz = m_projectile[i].z;
		}
	}
	for (int i = 0; i < m_vertexCount; i++)
	{
		if (Miz > m_projectile[i].z)
		{
			Miz = m_projectile[i].z;
		}
	}
	XMMATRIX m_ColliderInfo = trans;

	XMVECTOR c_row_3 = m_ColliderInfo.r[3];

	float c_m30 = XMVectorGetX(c_row_3);
	float c_m31 = XMVectorGetY(c_row_3);
	float c_m32 = XMVectorGetZ(c_row_3);

	XMVECTOR s_row_0 = scale.r[0];
	XMVECTOR s_row_1 = scale.r[1];
	XMVECTOR s_row_2 = scale.r[2];

	float s_m00 = XMVectorGetX(s_row_0);
	float s_m11 = XMVectorGetY(s_row_1);
	float s_m22 = XMVectorGetZ(s_row_2);

	m_collider->Mx = c_m30 + ((Max - Mix) * s_m00) / 2.0f;
	m_collider->mx = c_m30 - ((Max - Mix) * s_m00) / 2.0f;
	m_collider->My = c_m31 + ((May - Miy) * s_m11) / 2.0f;
	m_collider->my = c_m31 - ((May - Miy) * s_m11) / 2.0f;
	m_collider->Mz = c_m32 + ((Maz - Miz) * s_m22) / 2.0f;
	m_collider->mz = c_m32 - ((Maz - Miz) * s_m22) / 2.0f;
}

void ProjectileClass::SetCollider() {
	m_collider->Mx = XMVectorGetX(projectilePosition) + 10 / 2;
	m_collider->mx = XMVectorGetX(projectilePosition) - 10 / 2;
	m_collider->My = XMVectorGetY(projectilePosition) + 10 / 2;
	m_collider->my = XMVectorGetY(projectilePosition) - 10 / 2;
	m_collider->Mz = XMVectorGetZ(projectilePosition) + 10 / 2;
	m_collider->mz = XMVectorGetZ(projectilePosition) - 10 / 2;
}

bool ProjectileClass::checkCollider(float max, float mix, float may, float miy, float maz, float miz)
{
	if ((mix <= m_collider->Mx && m_collider->Mx <= max) || (mix <= m_collider->mx && m_collider->mx <= max))
	{
		if ((miy <= m_collider->My && m_collider->My <= may) || (miy <= m_collider->my && m_collider->my <= may))
		{
			if ((miz <= m_collider->Mz && m_collider->Mz <= maz) || (miz <= m_collider->mz && m_collider->mz <= maz))
			{
				return true;
			}
		}
	}

	return false;
}

int ProjectileClass::getPolygon()
{
	return m_indexCount / 3;
}


void ProjectileClass::ObjconvertToTxt(char* filename, char* readfile)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	GetModelFilename(filename);

	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return;
	}

	// Now read the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount, readfile);
	if (!result)
	{
		return;
	}
}


void ProjectileClass::GetModelFilename(char* filename)
{
	bool done;
	ifstream fin;

	// Loop until we have a file name.
	done = false;
	while (!done)
	{
		// Attempt to open the file.
		fin.open(filename);

		if (fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
		}
	}
	return;
}


bool ProjectileClass::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	return true;
}


bool ProjectileClass::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount, char* readfile)
{
	VertexFile* vertices, * texcoords, * normals;
	FaceFile* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;


	// Initialize the four data structures.
	vertices = new VertexFile[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexFile[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexFile[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceFile[faceCount];
	if (!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	// Close the file.
	fin.close();

	// Open the output file.
	fout.open(readfile);

	// Write out the file header that our model format uses.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	// Close the output file.
	fout.close();

	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}