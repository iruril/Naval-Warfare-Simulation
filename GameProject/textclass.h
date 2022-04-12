////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "fontclass.h"
#include "fontshaderclass.h"
#include "AlignedAllocationPolicy.h"

#include <string>

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
class TextClass : public AlignedAllocationPolicy<16>
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);
	bool RenderResult(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetTime(float, ID3D11DeviceContext*);
	bool SetObject(int, ID3D11DeviceContext*);
	bool SetScreenHeight(ID3D11DeviceContext*);
	bool SetScreenWidth(ID3D11DeviceContext*);
	bool SetPolygon(float, ID3D11DeviceContext*);
	bool SetAimState(bool, ID3D11DeviceContext*);
	bool SetPitchState(int, ID3D11DeviceContext*);
	bool SetSpeedState(float, ID3D11DeviceContext*);

	bool SetQuestState(bool, bool, ID3D11DeviceContext*);
	bool SetFlagsState(int, bool, ID3D11DeviceContext*);
	bool SetContainersState(int, bool, ID3D11DeviceContext*);

	bool SetEnemyShipsState(int, bool, ID3D11DeviceContext*);
	bool SetEnemyCoastGunsState(int, bool, ID3D11DeviceContext*);

	bool SetReloadState(int, ID3D11DeviceContext*);

	bool SetResultState(bool, bool, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX, XMMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	XMMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
	SentenceType* m_sentence3;
	SentenceType* m_sentence4;
	SentenceType* m_time;
	SentenceType* m_object;

	SentenceType* m_screenWidthTxt;
	SentenceType* m_screenHeightTxt;

	SentenceType* m_polygon;

	SentenceType* m_AimState;

	SentenceType* m_PitchState;
	SentenceType* m_SpeedState;

	SentenceType* m_Quests;
	SentenceType* m_Flags;
	SentenceType* m_Containers;

	SentenceType* m_EnemyShips;
	SentenceType* m_CoastGuardGuns;

	SentenceType* m_reload;

	SentenceType* m_result;
};

#endif