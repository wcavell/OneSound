using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class SoundListener:NativeObject
    {
        public SoundListener()
        {
            Handle = NativeMethods.Listener_Create();
        }

        protected override void CloseHandle()
        {
            NativeMethods.Listener_Destroy(Handle);
        }

        public IntPtr GetListenerPtr()
        {
            return NativeMethods.Listener_GetListener(Handle);
        }

        public void SetPositionOrientation(Vector3 position, Vector3 top, Vector3 front)
        {
            NativeMethods.Listener_SetPositionOrientation(Handle, position, top, front);
        }


        public void SetListenerVelocity(Vector3 velocity)
        {
            NativeMethods.Listener_SetListenerVelocity(Handle, velocity);
        }


        public void Update()
        {
            NativeMethods.Listener_Update(Handle);
        }

        public Speaker Speaker
        {
            get { return NativeMethods.Listener_GetSpeaker(Handle); }
            set { NativeMethods.Listener_SetSpeaker(Handle, value); }
        }

        public void AddSound(Sound3D sound)
        {
            NativeMethods.Listener_AddSound(Handle, sound.Handle);
        }

        public void RemoveSound(Sound3D sound)
        {
            NativeMethods.Listener_RemoveSound(Handle, sound.Handle);
        }
    }
}
