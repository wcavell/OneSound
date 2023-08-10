#include "OneSound\Export.h"
#include "OneSound\OneSound.h"
#include "OneSound\SoundType\Sound3D.h"

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
	ONE_SOUND_API  SoundBuffer* SoundBuffer_Create_ByPath(char* file)
	{
		SoundBuffer* buffer = new SoundBuffer(file);
		return buffer;
	}
	ONE_SOUND_API  void SoundBuffer_Load(SoundBuffer* buffer, char* file)
	{
		buffer->Load(file);
	}
	ONE_SOUND_API void SoundBuffer_Destroy(SoundBuffer* buffer)
	{
		delete buffer;
		buffer = nullptr;
	}
	ONE_SOUND_API SoundStream* SoundStream_Create()
	{
		SoundStream* stream = new SoundStream();
		return stream;
	}
	ONE_SOUND_API SoundStream* SoundStream_Create_ByPath(char* file)
	{
		SoundStream* stream = new SoundStream(file);
		return stream;
	}
	ONE_SOUND_API  void SoundStream_Load(SoundStream* stream, char* file)
	{
		stream->Load(file);
	}
	ONE_SOUND_API void SoundStream_Destroy(SoundStream* stream)
	{
		delete stream;
		stream = nullptr;
	}
	ONE_SOUND_API Sound2D* Sound2D_Create()
	{
		Sound2D* sound = new Sound2D();
		return sound;
	}
	ONE_SOUND_API Sound2D* Sound2D_Create_ByBuffer(SoundBuffer* buff, bool loop, bool play, float volume)
	{
		std::shared_ptr<SoundBuffer> ptr(buff);
		Sound2D* sound = new Sound2D(ptr, loop, play, volume);
		return sound;
	}
	ONE_SOUND_API void Sound2D_Destroy(Sound2D* sound)
	{
		delete sound;
		sound = nullptr;
	}
	ONE_SOUND_API Sound3D* Sound3D_Create()
	{
		Sound3D* sound = new Sound3D();
		return sound;
	}
	ONE_SOUND_API Sound3D* Sound3D_Create_ByBuffer(SoundBuffer* buff, bool loop, bool play, float volume)
	{
		std::shared_ptr<SoundBuffer> ptr(buff);
		Sound3D* sound = new Sound3D(ptr, loop, play, volume);
		return sound;
	}
	ONE_SOUND_API void Sound3D_Apply3D(Sound3D* sound, X3DAUDIO_LISTENER* xListener) {
		sound->apply3D(xListener);
	}
	ONE_SOUND_API void Sound3D_SetSourcePosition(Sound3D* sound, X3DAUDIO_VECTOR position) {
		sound->setSourcePosition(position);
	}
	ONE_SOUND_API void Sound3D_Update3D(Sound3D* sound) {
		sound->update3D();
	}
	ONE_SOUND_API void Sound3D_Destroy(Sound3D* sound)
	{
		delete sound;
		sound = nullptr;
	}

	ONE_SOUND_API void SoundObject_SetSound(SoundObject* so, SoundBuffer* sound, bool loop, bool play, float volume)
	{
		std::shared_ptr<SoundBuffer> ptr(sound);
		so->setSound(ptr, loop, play, volume);
	}
	ONE_SOUND_API bool SoundObject_IsStreamable(SoundObject* so) {
		return so->isStreamable();
	}
	ONE_SOUND_API bool SoundObject_IsEOS(SoundObject* so) {
		return so->isEOS();
	}
	ONE_SOUND_API void SoundObject_Play(SoundObject* so) {
		so->play();
	}
	ONE_SOUND_API void SoundObject_PlaySound(SoundObject* so, SoundBuffer* sound, bool loop, bool play, float volume) {
		const std::shared_ptr<SoundBuffer> ptr(sound);
		so->play(ptr, loop, play, volume);
	}
	ONE_SOUND_API void SoundObject_Stop(SoundObject* so) {
		so->stop();
	}
	ONE_SOUND_API void SoundObject_Pause(SoundObject* so) {
		so->pause();
	}
	ONE_SOUND_API void SoundObject_Rewind(SoundObject* so) {
		so->rewind();
	}
	ONE_SOUND_API bool SoundObject_IsPlaying(SoundObject* so) {
		return so->isPlaying();
	}
	ONE_SOUND_API bool SoundObject_IsStopped(SoundObject* so) {
		return so->isStopped();
	}
	ONE_SOUND_API bool SoundObject_IsPaused(SoundObject* so) {
		return so->isPaused();
	}
	ONE_SOUND_API bool SoundObject_IsInitial(SoundObject* so) {
		return so->isInitial();
	}
	ONE_SOUND_API bool SoundObject_IsLooping(SoundObject* so) {
		return so->isLooping();
	}
	ONE_SOUND_API void SoundObject_SetLooping(SoundObject* so, bool looping) {
		so->setLooping(looping);
	}
	ONE_SOUND_API void SoundObject_SetVolume(SoundObject* so, float gain) {
		so->setVolume(gain);
	}
	ONE_SOUND_API float SoundObject_GetVolume(SoundObject* so) {
		return so->getVolume();
	}
	ONE_SOUND_API int SoundObject_GetPlaybackPosition(SoundObject* so) {
		return so->getPlaybackPosition();
	}
	ONE_SOUND_API void SoundObject_SetPlaybackPosition(SoundObject* so, int seekpos) {
		so->setPlaybackPosition(seekpos);
	}
	ONE_SOUND_API int SoundObject_GetPlaybackSize(SoundObject* so) {
		return so->getPlaybackSize();
	}
	ONE_SOUND_API int SoundObject_GetSamplesPerSecond(SoundObject* so) {
		return so->getSamplesPerSecond();
	}
	ONE_SOUND_API void SoundObject_SetOutChannel(SoundObject* so, uint32_t speakerLeftChannel, uint32_t speakerRightChannel) {
		so->setOutChannel(speakerLeftChannel, speakerRightChannel);
	}
	ONE_SOUND_API void SoundObject_SetSoundChannel(SoundObject* so, int channel) {
		so->setSoundChannel(channel);
	}

	ONE_SOUND_API Listener* Listener_Create() {
		Listener* listener = new Listener();
		return listener;
	}
	ONE_SOUND_API void Listener_Destroy(Listener* listener) {
		delete listener;
		listener = nullptr;
	}
	ONE_SOUND_API void Listener_SetPositionOrientation(Listener* listener, X3DAUDIO_VECTOR position, X3DAUDIO_VECTOR top,  X3DAUDIO_VECTOR front) {
		listener->setPositionOrientation(position, top, front);
	}
	ONE_SOUND_API X3DAUDIO_LISTENER* Listener_GetListener(Listener* listener) {
		return listener->getListener();
	}

}