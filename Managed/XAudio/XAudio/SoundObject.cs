using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class SoundObject:NativeObject
    {
        internal SoundObject()
        {

        }
        public bool IsStreamable
        {
            get
            {
                return NativeMethods.SoundObject_IsStreamable(Handle);
            }
        }


        public void SetSound(SoundBuffer sound, bool loop, bool play, float volume)
        {
            NativeMethods.SoundObject_SetSound(Handle, sound.Handle, loop, play, volume);
        }



        public bool IsEOS
        {
            get { return NativeMethods.SoundObject_IsEOS(Handle); }
        }

        public void Play()
        {
            NativeMethods.SoundObject_Play(Handle);
        }

        public void Play(SoundBuffer sound, bool loop, bool play, float volume)
        {
            NativeMethods.SoundObject_PlaySound(Handle, sound.Handle, loop, play, volume);
        }

        public void Stop()
        {
            NativeMethods.SoundObject_Stop(Handle);
        }

        public void Pause()
        {
            NativeMethods.SoundObject_Pause(Handle);
        }

        public void Rewind()
        {
            NativeMethods.SoundObject_Rewind(Handle);
        }

        public bool IsPlaying
        {
            get { return NativeMethods.SoundObject_IsPlaying(Handle); }
        }

        public bool IsStopped
        {
            get { return NativeMethods.SoundObject_IsStopped(Handle); }
        }

        public bool IsPaused
        {
            get { return NativeMethods.SoundObject_IsPaused(Handle); }
        }

        public bool IsInitial
        {
            get { return NativeMethods.SoundObject_IsInitial(Handle); }
        }

        public bool IsLooping
        {
            get { return NativeMethods.SoundObject_IsLooping(Handle); }
            set { NativeMethods.SoundObject_SetLooping(Handle, value); }
        }

        public void SetLooping(bool looping)
        {
            NativeMethods.SoundObject_SetLooping(Handle, looping);
        }


        public float Volume
        {
            get { return NativeMethods.SoundObject_GetVolume(Handle); }
            set { NativeMethods.SoundObject_SetVolume(Handle, value); }
        }

        public int PlaybackPosition
        {
            get { return NativeMethods.SoundObject_GetPlaybackPosition(Handle); }
            set { NativeMethods.SoundObject_SetPlaybackPosition(Handle, value); }
        }


        public int GetPlaybackSize()
        {
            return NativeMethods.SoundObject_GetPlaybackSize(Handle);
        }

        public int GetSamplesPerSecond()
        {
            return NativeMethods.SoundObject_GetSamplesPerSecond(Handle);
        }

        public IntPtr GetEmitter()
        {
            return NativeMethods.SoundObject_GetEmitter(Handle);
        }

        public void SetOutputMatrix(uint SourceChannels, uint DestinationChannels,
            float[] pLevelMatrix)
        {
            NativeMethods.SoundObject_SetOutputMatrix(Handle, SourceChannels, DestinationChannels, pLevelMatrix);
        }
    }
}
