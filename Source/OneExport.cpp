#include "OneSound\Export.h"
#include "OneSound\OneSound.h"
#define WINAPI      __stdcall
using namespace onesnd;
extern "C"
{
	ONE_SOUND_API OneSound* OneSound_Create()
	{
		OneSound* one = new OneSound();
		return one;
	}
	ONE_SOUND_API void OneSound_Destroy(OneSound* one)
	{
		delete one;
		one = nullptr;
	}

	ONE_SOUND_API SoundBuffer* SoundBuffer_Create()
	{
		SoundBuffer* buffer = new SoundBuffer();
		return buffer;
	}
	ONE_SOUND_API  SoundBuffer* SoundBufferByPath_Create(const char* file)
	{ 
		SoundBuffer* buffer = new SoundBuffer(file);
		return buffer;
	}
	ONE_SOUND_API  void SoundBuffer_Load(SoundBuffer* buffer, const char* file)
	{
		buffer->Load(file);
	}
	ONE_SOUND_API void SoundBuffer_Destroy(SoundBuffer* buffer)
	{
		delete buffer;
		buffer = nullptr;
	}

}