using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public sealed class Sound2D:SoundObject
    {
        public Sound2D()
        {
            Handle = NativeMethods.Sound2D_Create();
        }

        public Sound2D(SoundBuffer buffer, bool loop, bool play, float volume)
        {
            Handle = NativeMethods.Sound2D_Create_ByBuffer(buffer.Handle, loop, play, volume);
        }

        protected override void CloseHandle()
        {
            NativeMethods.Sound2D_Destroy(Handle);
        }

        public Speaker Speaker
        {
            get { return NativeMethods.Sound2D_GetSpeaker(Handle); }
            set { NativeMethods.Sound2D_SetSpeaker(Handle, value); }
        }

        //public void SetSpeakerVolume(float[] volume, int count)
        //{
        //    OneAPI.Sound2D_SetSpeakerVolume(Handle, volume, count);
        //}


        public bool IsMono
        {
            get { return NativeMethods.Sound2D_GetMono(Handle); }
            set { NativeMethods.Sound2D_SetMono(Handle, value); }
        }
    }
}
