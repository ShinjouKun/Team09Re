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
	//RIFF�w�b�_�[�ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C�������t���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char*)&data, sizeof(data));
	//Data�`�����N�̃f�[�^���i�g�`�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//wave�t�@�C�������
	file.close();

	WAVEFORMATEX wfex{};
	//�g�`�t�H�[�}�b�g�̂����Ă�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	//�g�`�t�H�[�}�b�g�����Ƃ�SoureceVoice�̐���
	
	xAudio2->CreateSourceVoice(&sourceVoice, &wfex);
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	//buf.LoopCount = XAUDIO2_LOOP_INFINITE;//�������[�v

	//baffwo�Z�b�g
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
	//RIFF�w�b�_�[�ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C�������t���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char*)&data, sizeof(data));
	//Data�`�����N�̃f�[�^���i�g�`�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//wave�t�@�C�������
	file.close();

	WAVEFORMATEX wfex{};
	//�g�`�t�H�[�}�b�g�̂����Ă�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
	//�g�`�t�H�[�}�b�g�����Ƃ�SoureceVoice�̐���

	xAudio2->CreateSourceVoice(&sourceVoice, &wfex);
	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;//�������[�v

	//baffwo�Z�b�g
	sourceVoice->SubmitSourceBuffer(&buf);
}

void Sound::Play(const char*filename)
{
	sourceVoice->Start();
}
