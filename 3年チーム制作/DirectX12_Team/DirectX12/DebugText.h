#pragma once
#include"Sprite.h"
#include<string>
#include <Windows.h>
//Debug用テキスト
class DebugText
{
public:
	DebugText();
	~DebugText();
	void Init(UINT texNum);
	void Print(const std::string& text, Vector2 pos, float size);
	void Draw(ID3D12GraphicsCommandList * cmdList);
private:
	static const int maxCharCount = 256;
	static const int fontWidth = 32;
	static const int fontHeight = 32;
	static const int fontLineCount = 14;
	//Sprite* sprite;
	Sprite* spriteDatas[maxCharCount];
	int spriteIndex = 0;
};
