#pragma once

#include "OneSound.h"

using namespace onesnd;

extern "C" {
    ONE_SOUND_API OneSound* OneSound_Create();
    ONE_SOUND_API unsigned long long OneSound_GetLibraryVersion(OneSound* one);
    ONE_SOUND_API char* OneSound_GetLibraryVersionStr(OneSound* one);
    ONE_SOUND_API char* OneSound_GetLibraryStatus(OneSound* one);
    ONE_SOUND_API char* OneSound_GetLibraryName(OneSound* one);
    ONE_SOUND_API int32_t OneSound_GetOutputChannels();
    ONE_SOUND_API void OneSound_SetLeftSpeakerMap(uint32_t lSpeaker);
    ONE_SOUND_API void OneSound_SetRightSpeakerMap(uint32_t rSpeaker);
    ONE_SOUND_API int32_t OneSound_GetLeftSpeakerMap();
    ONE_SOUND_API int32_t OneSound_GetRightSpeakerMap();
    ONE_SOUND_API void OneSound_XACT3DCalculate(X3DAUDIO_LISTENER* pListener, X3DAUDIO_EMITTER* pEmitter, X3DAUDIO_DSP_SETTINGS* pDSPSettings);
    ONE_SOUND_API void OneSound_X3DAudioCalculate(X3DAUDIO_LISTENER* listener, X3DAUDIO_EMITTER* emitter, uint32_t flags,
        X3DAUDIO_DSP_SETTINGS* dspSettingsPtr);
    ONE_SOUND_API void OneSound_Destroy(OneSound* one);

    ONE_SOUND_API SoundBuffer* SoundBuffer_Create();
    ONE_SOUND_API SoundBuffer* SoundBuffer_Create_ByPath(char* file);
    ONE_SOUND_API void SoundBuffer_Load(SoundBuffer* buffer, char* file);
    ONE_SOUND_API void SoundBuffer_Destroy(SoundBuffer* buffer);

    ONE_SOUND_API SoundStream* SoundStream_Create();
    ONE_SOUND_API SoundStream* SoundStream_Create_ByPath(char* file);
    ONE_SOUND_API void SoundStream_Load(SoundStream* stream, char* file);
    ONE_SOUND_API void SoundStream_Destroy(SoundStream* stream);

    ONE_SOUND_API Sound2D* Sound2D_Create();
    ONE_SOUND_API Sound2D* Sound2D_Create_ByBuffer(SoundBuffer* buff, bool loop, bool play, float volume);
    ONE_SOUND_API uint32_t Sound2D_GetSpeaker(Sound2D* sound);
    ONE_SOUND_API void Sound2D_SetSpeaker(Sound2D* sound, uint32_t mask);
    ONE_SOUND_API void Sound2D_SetSpeakerVolume(Sound2D* sound, float* volume, int count);
    ONE_SOUND_API void Sound2D_SetMono(Sound2D* sound, bool mono);
    ONE_SOUND_API bool Sound2D_GetMono(Sound2D* sound);
    ONE_SOUND_API void Sound2D_Destroy(Sound2D* sound);

    ONE_SOUND_API Sound3D* Sound3D_Create();
    ONE_SOUND_API Sound3D* Sound3D_Create_ByBuffer(SoundBuffer* buff, bool loop, bool play, float volume);
    ONE_SOUND_API void Sound3D_SetSourcePosition(Sound3D* sound, X3DAUDIO_VECTOR position);
    ONE_SOUND_API X3DAUDIO_DSP_SETTINGS* Sound3D_GetDspSetting(Sound3D* sound);
    ONE_SOUND_API void Sound3D_Destroy(Sound3D* sound);

    ONE_SOUND_API void SoundObject_SetSound(SoundObject* so, SoundBuffer* sound, bool loop, bool play, float volume);
    ONE_SOUND_API bool SoundObject_IsStreamable(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsEOS(SoundObject* so);
    ONE_SOUND_API void SoundObject_Play(SoundObject* so);
    ONE_SOUND_API void SoundObject_PlaySound(SoundObject* so, SoundBuffer* sound, bool loop, bool play, float volume);
    ONE_SOUND_API void SoundObject_Stop(SoundObject* so);
    ONE_SOUND_API void SoundObject_Pause(SoundObject* so);
    ONE_SOUND_API void SoundObject_Rewind(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsPlaying(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsStopped(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsPaused(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsInitial(SoundObject* so);
    ONE_SOUND_API bool SoundObject_IsLooping(SoundObject* so);
    ONE_SOUND_API void SoundObject_SetLooping(SoundObject* so, bool looping);
    ONE_SOUND_API void SoundObject_SetVolume(SoundObject* so, float gain);
    ONE_SOUND_API float SoundObject_GetVolume(SoundObject* so);
    ONE_SOUND_API int SoundObject_GetPlaybackPosition(SoundObject* so);
    ONE_SOUND_API void SoundObject_SetPlaybackPosition(SoundObject* so, int seekpos);
    ONE_SOUND_API int SoundObject_GetPlaybackSize(SoundObject* so);
    ONE_SOUND_API int SoundObject_GetSamplesPerSecond(SoundObject* so);
    ONE_SOUND_API X3DAUDIO_EMITTER* SoundObject_GetEmitter(SoundObject* so);
    ONE_SOUND_API void SoundObject_SetOutputMatrix(SoundObject* so, UINT32 SourceChannels, UINT32 DestinationChannels, float* pLevelMatrix);

    ONE_SOUND_API Listener* Listener_Create();
    ONE_SOUND_API void Listener_Destroy(Listener* listener);
    ONE_SOUND_API void Listener_SetPositionOrientation(Listener* listener, X3DAUDIO_VECTOR position, X3DAUDIO_VECTOR top, X3DAUDIO_VECTOR front);
    ONE_SOUND_API X3DAUDIO_LISTENER* Listener_GetListener(Listener* listener);
    ONE_SOUND_API void Listener_SetListenerVelocity(Listener* listener, X3DAUDIO_VECTOR velocity);
    ONE_SOUND_API void Listener_Update(Listener* listener);
    ONE_SOUND_API void Listener_SetSpeaker(Listener* listener, uint32_t speaker);
    ONE_SOUND_API uint32_t Listener_GetSpeaker(Listener* listener);
    ONE_SOUND_API void Listener_AddSound(Listener* listener, Sound3D* sound);
    ONE_SOUND_API void Listener_RemoveSound(Listener* listener, Sound3D* sound);


}