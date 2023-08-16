using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    public class NativeObject : IDisposable
    {
        public IntPtr Handle { get; set; }
        protected bool Disposed { get; private set; }

        ~NativeObject()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);

            GC.SuppressFinalize(this);
        }

        void Dispose(bool disposing)
        {
            if (!this.Disposed)
            {
                CloseHandle();
                Handle = IntPtr.Zero;
                Disposed = true;
            }
        }

        protected virtual void CloseHandle()
        {

        }
    }
}