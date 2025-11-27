using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using UnityEditor.VersionControl;
using XAudio.Utils;
using XAudio.X3DAudio;

namespace XAudio
{
    internal class NativeMethods
    {
#if DEBUG
        private const string dllName = "OneSoundD.dll";
#else
        private const string dllName = "OneSound.dll";
#endif


        [DllImport(dllName)]
        public static extern IntPtr OneSound_Create();

        [DllImport(dllName)]
        public static extern void OneSound_Destroy(IntPtr handle);

        [DllImport(dllName)]
        public static extern ulong OneSound_GetLibraryVersion(IntPtr handle);
        [DllImport(dllName)]
        public static extern string OneSound_GetLibraryVersionStr(IntPtr handle);
        [DllImport(dllName)]
        public static extern string OneSound_GetLibraryStatus(IntPtr handle);
        [DllImport(dllName)]
        public static extern string OneSound_GetLibraryName(IntPtr handle);
        [DllImport(dllName)]
        public static extern int OneSound_GetOutputChannels();

        [DllImport(dllName)]
        public static extern void OneSound_GetChannelInfo(ref int count, ref Speaker mask);
        [DllImport(dllName)]
        public static extern void OneSound_SetLeftSpeakerMap(Speaker lSpeakers);

        [DllImport(dllName)]
        public static extern void OneSound_SetRightSpeakerMap(Speaker rSpeaker);
        [DllImport(dllName)]
        public static extern Speaker OneSound_GetLeftSpeakerMap();
        [DllImport(dllName)]
        public static extern Speaker OneSound_GetRightSpeakerMap();

        [DllImport(dllName)]
        public static extern IntPtr SoundBuffer_Create();

        [DllImport(dllName)]
        public static extern IntPtr SoundBuffer_Create_ByPath(string file);

        [DllImport(dllName)]
        public static extern void SoundBuffer_Load(IntPtr handle, string file);

        [DllImport(dllName)]
        public static extern void SoundBuffer_Destroy(IntPtr handle);



        [DllImport(dllName)]
        public static extern IntPtr SoundStream_Create();

        [DllImport(dllName)]
        public static extern IntPtr SoundStream_Create_ByPath(string file);

        [DllImport(dllName)]
        public static extern void SoundStream_Load(IntPtr handle, string file);

        [DllImport(dllName)]
        public static extern void SoundStream_Destroy(IntPtr handle);



        [DllImport(dllName)]
        public static extern IntPtr Sound2D_Create();

        [DllImport(dllName)]
        public static extern IntPtr Sound2D_Create_ByBuffer(IntPtr buff /*SoundBuffer/SoundStream*/, bool loop,
            bool play, float volume);

        [DllImport(dllName)]
        public static extern void Sound2D_Destroy(IntPtr handle);

        [DllImport(dllName)]
        public static extern Speaker Sound2D_GetSpeaker(IntPtr handle);
        [DllImport(dllName)]
        public static extern void Sound2D_SetSpeaker(IntPtr handle, Speaker mask);
        [DllImport(dllName)]
        public static extern void Sound2D_SetSpeakerVolume(IntPtr handle, [In] float[] volume, int count);
        [DllImport(dllName)]
        public static extern void Sound2D_SetMono(IntPtr handle, bool mono);
        [DllImport(dllName)]
        public static extern bool Sound2D_GetMono(IntPtr handle);



        [DllImport(dllName)]
        public static extern IntPtr Sound3D_Create();

        [DllImport(dllName)]
        public static extern IntPtr Sound3D_Create_ByBuffer(IntPtr buff /*SoundBuffer/SoundStream*/, bool loop,
            bool play, float volume);

        [DllImport(dllName)]
        public static extern void Sound3D_SetSourcePosition(IntPtr handle, Vector3 position);
        [DllImport(dllName)]
        public static extern IntPtr Sound3D_GetDspSetting(IntPtr handle);
        [DllImport(dllName)]
        public static extern void Sound3D_Destroy(IntPtr handle);


        [DllImport(dllName)]
        public static extern void SoundObject_SetSound(IntPtr so, IntPtr sound, bool loop, bool play, float volume);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsStreamable(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsEOS(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_Play(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_PlaySound(IntPtr so, IntPtr sound, bool loop, bool play, float volume);
        [DllImport(dllName)]
        public static extern void SoundObject_Stop(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_Pause(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_Rewind(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsPlaying(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsStopped(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsPaused(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsInitial(IntPtr so);
        [DllImport(dllName)]
        public static extern bool SoundObject_IsLooping(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_SetLooping(IntPtr so, bool looping);
        [DllImport(dllName)]
        public static extern void SoundObject_SetVolume(IntPtr so, float gain);
        [DllImport(dllName)]
        public static extern float SoundObject_GetVolume(IntPtr so);
        [DllImport(dllName)]
        public static extern int SoundObject_GetPlaybackPosition(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_SetPlaybackPosition(IntPtr so, int seekpos);
        [DllImport(dllName)]
        public static extern int SoundObject_GetPlaybackSize(IntPtr so);
        [DllImport(dllName)]
        public static extern int SoundObject_GetSamplesPerSecond(IntPtr so);
        [DllImport(dllName)]
        public static extern IntPtr SoundObject_GetEmitter(IntPtr so);
        [DllImport(dllName)]
        public static extern void SoundObject_SetOutputMatrix(IntPtr so, uint SourceChannels, uint DestinationChannels,
            [In, Out] float[] pLevelMatrix);

        [DllImport(dllName)]
        public static extern IntPtr Listener_Create();
        [DllImport(dllName)]
        public static extern void Listener_Destroy(IntPtr listener);
        [DllImport(dllName)]
        public static extern void Listener_SetPositionOrientation(IntPtr listener, Vector3 position, Vector3 top, Vector3 front);
        [DllImport(dllName)]
        public static extern IntPtr Listener_GetListener(IntPtr listener);

        [DllImport(dllName)]
        public static extern void Listener_SetListenerVelocity(IntPtr listener, Vector3 velocity);
        [DllImport(dllName)]
        public static extern void Listener_Update(IntPtr listener);
        [DllImport(dllName)]
        public static extern void Listener_SetSpeaker(IntPtr listener, Speaker speaker);
        [DllImport(dllName)]
        public static extern Speaker Listener_GetSpeaker(IntPtr listener);
        [DllImport(dllName)]
        public static extern void Listener_AddSound(IntPtr listener, IntPtr sound);
        [DllImport(dllName)]
        public static extern void Listener_RemoveSound(IntPtr listener, IntPtr sound);


        [DllImport(dllName)]
        public static extern void OneSound_XACT3DCalculate(IntPtr pListener, IntPtr pEmitter, IntPtr pDSPSettings);
        [DllImport(dllName)]
        public static extern void OneSound_X3DAudioCalculate( IntPtr listener, IntPtr emitter, CalculateFlags flags,
            IntPtr dspSettingsPtr);
    }
}
