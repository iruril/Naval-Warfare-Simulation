///////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "textclass.h"


TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
	m_sentence3 = 0;
	m_sentence4 = 0;

	m_time = 0;
	m_object = 0;
	m_screenWidthTxt = 0;
	m_screenHeightTxt = 0;
	m_polygon = 0;
	m_AimState = 0;
	m_PitchState = 0;
	m_SpeedState = 0;
	m_reload = 0;

	m_Quests = 0;
	m_Flags = 0;
	m_Containers = 0;
	m_result = 0;
}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, 
	int screenWidth, int screenHeight, XMMATRIX baseViewMatrix)
{
	bool result;


	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, L"./data/fontdata.txt", L"./data/font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence1, 16, device);
	if(!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence2, 16, device);
	if(!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}
	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence3, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence3, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_sentence4, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence4, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_time, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_time, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_object, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_object, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_screenWidthTxt, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_screenWidthTxt, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_screenHeightTxt, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_screenHeightTxt, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_polygon, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_polygon, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_AimState, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_AimState, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_PitchState, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_PitchState, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_SpeedState, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_SpeedState, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_Quests, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Quests, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_Flags, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Flags, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_Containers, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Containers, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_EnemyShips, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_EnemyShips, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_CoastGuardGuns, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_CoastGuardGuns, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_result, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_result, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_reload, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_reload, (CHAR*)" ", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextClass::Shutdown()
{
	// Release the first sentence.
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);
	ReleaseSentence(&m_sentence3);
	ReleaseSentence(&m_sentence4);

	ReleaseSentence(&m_time);

	ReleaseSentence(&m_object);

	ReleaseSentence(&m_screenWidthTxt);
	ReleaseSentence(&m_screenHeightTxt);

	ReleaseSentence(&m_polygon);
	ReleaseSentence(&m_AimState);

	ReleaseSentence(&m_PitchState);
	ReleaseSentence(&m_SpeedState);

	ReleaseSentence(&m_Quests);
	ReleaseSentence(&m_Flags);
	ReleaseSentence(&m_Containers);
	ReleaseSentence(&m_EnemyShips);
	ReleaseSentence(&m_CoastGuardGuns);

	ReleaseSentence(&m_result);

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;


	// Draw the first sentence.
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Draw the second sentence.
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}
	// Draw the first sentence.
	result = RenderSentence(deviceContext, m_sentence3, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the second sentence.
	result = RenderSentence(deviceContext, m_sentence4, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_time, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_object, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_screenWidthTxt, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_screenHeightTxt, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_polygon, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_AimState, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_PitchState, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_SpeedState, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_Quests, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_Flags, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_Containers, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_EnemyShips, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_CoastGuardGuns, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_reload, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::RenderResult(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix) {
	bool result;

	result = RenderSentence(deviceContext, m_result, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, const char* text, int positionX, int positionY, 
	float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix, 
							   XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, 
		orthoMatrix, m_Font->GetTexture(), pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, mouseString, 20, 160, 0, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence2, mouseString, 20, 180, 0, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red = 0, green = 0, blue = 0;
	bool result;


	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "FPS: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence3, fpsString, 20, 10, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	float red = 0, green = 0, blue = 0;
	bool result;


	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	if (cpu >= 40)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	if (cpu < 30)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	if (cpu < 10)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 1.0f;
	}

	// Setup the cpu string.
	strcpy_s(cpuString, "CPU: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence4, cpuString, 90, 10, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetTime(float time, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char tempString_min[16];//분 string
	char tempString_sec[16];//초 string
	char timeString[16];
	bool result;
	int calc = (int)time;
	int min;//분 int
	int sec;//초int
	float r = 0, g = 0, b = 0;
	if (calc <= 20)//20초 이하 초록색
	{
		r = 0.0f;
		g = 1.0f;
		b = 1.0f;
	}
	else if (calc <= 40)//40초 이하 노란색
	{
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}
	else//그 이상 빨간색
	{
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}


	if (calc >= 60)
	{
		sec = calc % 60;
		min = calc / 60;
		_itoa_s(sec, tempString_sec, 10);
		_itoa_s(min, tempString_min, 10);
		strcpy_s(tempString, tempString_min);
		strcat_s(tempString, ":");
		if (sec < 10)
			strcat_s(tempString, "0");
		strcat_s(tempString, tempString_sec);
	}
	else
	{
		_itoa_s(calc, tempString, 10);
	}

	// Setup the cpu string.
	strcpy_s(timeString, "Time: ");
	strcat_s(timeString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_time, timeString, 20, 60, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetObject(int object, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char objString[16];
	bool result;


	// Convert the cpu integer to string format.
	_itoa_s(object, tempString, 10);

	// Setup the cpu string.
	strcpy_s(objString, "Object: ");
	strcat_s(objString, tempString);
	strcat_s(objString, "EA");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_object, objString, 20, 60, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetScreenWidth(ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char screenString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(m_screenWidth, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(screenString, "Width: ");
	strcat_s(screenString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_screenWidthTxt, screenString, 20, 100, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetScreenHeight(ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char screenString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(m_screenHeight, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(screenString, "Height: ");
	strcat_s(screenString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_screenHeightTxt, screenString, 20, 120, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetPolygon(float polygon, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char polygonString[16];
	bool result;
	int calc = (int)polygon;


	_itoa_s(calc, tempString, 10);

	// Setup the cpu string.
	strcpy_s(polygonString, "Polygon: ");
	strcat_s(polygonString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_polygon, polygonString, 20, 100, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetAimState(bool isAimed, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char aimString[16];
	float r = 0, g = 0, b = 0;
	bool result;

	if(isAimed) {
		strcpy_s(tempString, "Ready");
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
	else {
		strcpy_s(tempString, "Aimming");
		r = 0.0f;
		g = 1.0f;
		b = 1.0f;
	}

	// Setup the cpu string.
	strcpy_s(aimString, "State: ");
	strcat_s(aimString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_AimState, aimString,
		m_screenWidth / 2 + 80, m_screenHeight / 2 + 20, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetPitchState(int pitch, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char pitchString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	_itoa_s(pitch, tempString, 10);
	if (pitch >= 0) {
		strcpy_s(pitchString, "Pitch: +");
	}
	else {
		strcpy_s(pitchString, "Pitch: ");
	}
	strcat_s(pitchString, tempString);
	if (pitch == 3) {
		strcat_s(pitchString, " (MAX)");
	}
	if (pitch == -1) {
		strcat_s(pitchString, " (MIN)");
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_PitchState, pitchString,
		205, m_screenHeight - 220, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetSpeedState(float speed, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char speedString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	int m_speed = (int)speed;
	bool result;

	if (m_speed > 30) m_speed = 30;
	else if ((m_speed > 20) && (m_speed < 22)) m_speed = 20;
	else if ((m_speed > 8) && (m_speed < 12)) m_speed = 10;
	else if ((m_speed > -2) && (m_speed < 2)) m_speed = 0;
	else if ((m_speed < -8)) m_speed = -10;

	_itoa_s(m_speed, tempString, 10);

	// Setup the cpu string.
	strcpy_s(speedString, "Speed: ");
	strcat_s(speedString, tempString);
	strcat_s(speedString, " knt");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_SpeedState, speedString,
		205 , m_screenHeight - 200, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetQuestState(bool isCleared, bool isFailed, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char questString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	if (isCleared == true) {
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
		strcpy_s(tempString, " Clear");
	}
	else {
		if (isFailed == true) {
			r = 1.0f;
			g = 0.0f;
			b = 0.0f;
			strcpy_s(tempString, " Fail");
		}
		else {
			r = 0.0f;
			g = 1.0f;
			b = 1.0f;
			strcpy_s(tempString, " ");
		}
	}

	// Setup the cpu string.
	strcpy_s(questString, "<Quests>");
	strcat_s(questString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Quests, questString,
		m_screenWidth - 200, 20, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFlagsState(int numFlags, bool fail, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char flagString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	_itoa_s(4 - numFlags, tempString, 10);
	if (!fail) {
		if (4 - numFlags == 4) {
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else if (4 - numFlags > 2){
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else {
			r = 0.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	else {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	// Setup the cpu string.
	strcpy_s(flagString, "- Flags (");
	strcat_s(flagString, tempString);
	strcat_s(flagString, "/4)");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Flags, flagString,
		m_screenWidth - 200, 40, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetContainersState(int numContainers, bool fail, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char boxString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	_itoa_s(3 - numContainers, tempString, 10);
	if (!fail) {
		if (3 - numContainers == 3) {
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else if (3 - numContainers > 1) {
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else {
			r = 0.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	else {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	// Setup the cpu string.
	strcpy_s(boxString, "- Boxs (");
	strcat_s(boxString, tempString);
	strcat_s(boxString, "/3)");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_Containers, boxString,
		m_screenWidth - 200, 60, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetEnemyShipsState(int numShips, bool fail, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char flagString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	_itoa_s(3 - numShips, tempString, 10);
	if (!fail) {
		if (3 - numShips == 3) {
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else if (3 - numShips > 1) {
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else {
			r = 0.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	else {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	// Setup the cpu string.
	strcpy_s(flagString, "- Ships (");
	strcat_s(flagString, tempString);
	strcat_s(flagString, "/3)");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_EnemyShips, flagString,
		m_screenWidth - 200, 80, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetEnemyCoastGunsState(int numCoastGuardGun, bool fail, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char boxString[16];
	float r = 0, g = 1.0f, b = 1.0f;
	bool result;

	_itoa_s(3 - numCoastGuardGun, tempString, 10);
	if (!fail) {
		if (3 - numCoastGuardGun == 3) {
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else if (3 - numCoastGuardGun > 1) {
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
		}
		else {
			r = 0.0f;
			g = 1.0f;
			b = 1.0f;
		}
	}
	else {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	// Setup the cpu string.
	strcpy_s(boxString, "- Guns (");
	strcat_s(boxString, tempString);
	strcat_s(boxString, "/3)");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_CoastGuardGuns, boxString,
		m_screenWidth - 200, 100, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetResultState(bool isClear, bool isFail, ID3D11DeviceContext* deviceContext) {
	char resultString[16];
	char tempString[16];
	float r = 1.0f, g = 1.0f, b = 1.0f;
	bool result;

	if (isClear) {
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
		strcpy_s(tempString, "Clear");
	}
	else if (isFail) {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
		strcpy_s(tempString, "Fail");
	}
	else {
		r = 0.0f;
		g = 1.0f;
		b = 1.0f;
		strcpy_s(tempString, "Quit");
	}

	// Setup the cpu string.
	strcpy_s(resultString, "results: ");
	strcat_s(resultString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_result, resultString,
		m_screenWidth / 2, m_screenHeight / 2, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetReloadState(int isReloded, ID3D11DeviceContext* deviceContext) {
	char reloadString[16];
	char tempString[16];
	float r = 1.0f, g = 1.0f, b = 1.0f;
	int time = 0;
	bool result;

	if (isReloded >= 5) {
		strcpy_s(tempString, "Loaded");
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
	}

	else if(isReloded < 0){
		strcpy_s(tempString, "5");
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}

	else {
		time = 5 - isReloded;
		_itoa_s(time, tempString, 10);
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}

	// Setup the cpu string.
	strcpy_s(reloadString, "Shells: ");
	strcat_s(reloadString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_reload, reloadString,
		m_screenWidth / 2 - 80, m_screenHeight / 2 + 20, r, g, b, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}