using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class SoundStream : SoundBuffer
    {
        public SoundStream()
        {
            Handle = NativeMethods.SoundStream_Create();
        }

        public SoundStream(string file)
        {
            Handle = NativeMethods.SoundStream_Create_ByPath(file);
        }

        protected override void CloseHandle()
        {
            NativeMethods.SoundStream_Destroy(Handle);
        }

        public override void Load(string file)
        {
            NativeMethods.SoundStream_Load(Handle, file);
        }
    }
}
