using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class Sound3D : SoundObject
    {
        public Sound3D()
        {
            Handle = NativeMethods.Sound3D_Create();
        }

        public Sound3D(SoundBuffer buffer, bool loop, bool play, float volume)
        {
            Handle = NativeMethods.Sound3D_Create_ByBuffer(buffer.Handle, loop, play, volume);
        }
        protected override void CloseHandle()
        {
            NativeMethods.Sound3D_Destroy(Handle);
        }

        public void SetSourcePosition(Vector3 position)
        {
            NativeMethods.Sound3D_SetSourcePosition(Handle, position);
        }

        public IntPtr GetDspSetting()
        {
            return NativeMethods.Sound3D_GetDspSetting(Handle);
        }
    }
}
