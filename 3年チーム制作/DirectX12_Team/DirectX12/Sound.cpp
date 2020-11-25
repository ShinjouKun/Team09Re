#include "Sound.h"
#include<fstream>
#include<cassert>

Sound::Sound()
{
	Init();
}

Sound::~Sound()
{
	sourceVoice->Stop();
	sourceVoice->DestroyVoice();
	masterVoice->DestroyVoice();
	masterVoice = nullptr;
	CoUninitialize();
}

void Sound::Init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudio2->CreateMasteringVoice(&masterVoice);
}

void Sound::LoadSE(const char*filename)
{
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	if (file.fail())
	{
		assert(0);
	}
	//RIFFヘッダー読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがリフかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	//Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));
	//Dataチャンクのデータ部（波形）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	//波形フォーマットのせってい
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	//波形フォーマットをもとにSoureceVoiceの生成
	
	xAudio2->CreateSourceVoice(&sourceVoice, &wfex);
	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	//buf.LoopCount = XAUDIO2_LOOP_INFINITE;//無限ループ

	//baffwoセット
	sourceVoice->SubmitSourceBuffer(&buf);
}

void Sound::LoadBGM(const char * filename)
{
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	if (file.fail())
	{
		assert(0);
	}
	//RIFFヘッダー読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがリフかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	//Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));
	//Dataチャンクのデータ部（波形）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	//波形フォーマットのせってい
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	//波形フォーマットをもとにSoureceVoiceの生成

	xAudio2->CreateSourceVoice(&sourceVoice, &wfex);
	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;//無限ループ

	//baffwoセット
	sourceVoice->SubmitSourceBuffer(&buf);
}

void Sound::Play(const char*filename)
{
	sourceVoice->Start();
}
