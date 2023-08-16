using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class SoundBuffer :NativeObject
    {
        public SoundBuffer()
        {
            Handle = NativeMethods.SoundBuffer_Create(); 
        }

        public SoundBuffer(string file)
        {
            Handle = NativeMethods.SoundBuffer_Create_ByPath(file);
        }

        protected override void CloseHandle()
        {
            NativeMethods.SoundBuffer_Destroy(Handle);
        }

        public virtual void Load(string file)
        {
            NativeMethods.SoundBuffer_Load(Handle, file);
        }
    }
}
