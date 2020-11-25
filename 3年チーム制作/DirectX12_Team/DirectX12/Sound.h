#pragma once
#include<memory>
#include<windows.h>
#include<xaudio2.h>
#include"wrl.h"
#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;
struct Chunk
{
	char id[4];//チャンク毎のID
	std::int32_t size;
};
struct RiffHeader
{
	Chunk chunk;//RIFF
	char type[4];
};
struct FormatChunk
{
	Chunk chunk;//fmt
	WAVEFORMAT fmt;

};
class Sound
{
public:
	Sound();
	~Sound();
	void Init();
	void LoadSE(const char*filename);//再生（一回）
	void LoadBGM(const char*filename);//
	void Play(const char*filename);
	

private:
	ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	IXAudio2SourceVoice* sourceVoice;
};
