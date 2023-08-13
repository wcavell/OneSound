using System; 
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Threading.Tasks;

namespace OneSound
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector
    {
        public static Vector Top = new Vector(0,1,0);
        public static Vector Front = new Vector(0, 0, 1);
        public float X;
        public float Y;
        public float Z;

        public Vector(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    [Flags]
    public enum Speaker : uint
    {
        None = 0x00000000,
        FrontLeft = 0x00000001,
        FrontRight = 0x00000002,
        FrontCenter = 0x00000004,
        LowFrequency = 0x00000008,
        BackLeft = 0x00000010,
        BackRight = 0x00000020,
        FrontLeftOfCenter = 0x00000040,
        FrontRightOfCenter = 0x00000080,
        BackCenter = 0x00000100,
        SideLeft = 0x00000200,
        SideRight = 0x00000400,
        TopCenter = 0x00000800,
        TopFrontLeft = 0x00001000,
        TopFrontCenter = 0x00002000,
        TopFrontRight = 0x00004000,
        TopBackLeft = 0x00008000,
        TopBackCenter = 0x00010000,
        TopBackRight = 0x00020000,
        Reserved = 0x7FFC0000,
        All = 0x80000000,
    }

    public class OneObject : IDisposable
    {
        public IntPtr Handle { get; set; }
        protected bool Disposed { get;private set; }
        ~OneObject()
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
                CloseHandle(Handle);
                Handle = IntPtr.Zero; 
                Disposed = true;
            }
        }

        protected virtual void CloseHandle(IntPtr handle)
        {

        }
    }

    public class SoundEngine : OneObject
    {
        /// <summary>
        /// 左声道映射 
        /// 默认值 FrontLeft | FrontCenter | BackLeft | SideLeft 
        /// </summary>
        public static Speaker LeftSpeakerMap
        {
            get
            {
                return OneAPI.OneSound_GetLeftSpeakerMap();
            }
            set
            {
                OneAPI.OneSound_SetLeftSpeakerMap(value);
            }
        }

        /// <summary>
        /// 右省道映射
        /// 默认值 FrontRight | LowFrequency | BackRight | SideRight
        /// </summary>
        public static Speaker RightSpeakerMap
        {
            get
            {
                return OneAPI.OneSound_GetRightSpeakerMap();
            }
            set
            {
                OneAPI.OneSound_SetRightSpeakerMap(value);
            }
        }


        public SoundEngine()
        {
            Handle = OneAPI.OneSound_Create();
        }

        protected override void CloseHandle(IntPtr handle)
        {
            OneAPI.OneSound_Destroy(handle);
        }

        
        public  void XACT3DCalculate(IntPtr X3DInstance, IntPtr pListener, IntPtr pEmitter,
            IntPtr pDSPSettings)
        {

        }

        public void X3DAudioCalculate(IntPtr instance, IntPtr listener, IntPtr emitter, uint flags,
            IntPtr dspSettingsPtr)
        {

        }

        public IntPtr GetInstance()
        {
            return OneAPI.X3DAudioCore_GetInstance();
        }
    }

    public class SoundBuffer :OneObject
    {
        public SoundBuffer()
        {
            Handle = OneAPI.SoundBuffer_Create();
        }

        public SoundBuffer(string file)
        {
            Handle = OneAPI.SoundBuffer_Create_ByPath(file);
        }

        protected override void CloseHandle(IntPtr handle)
        {
             OneAPI.SoundBuffer_Destroy(handle);
        }

        public virtual void Load(string file)
        {
            OneAPI.SoundBuffer_Load(Handle, file);
        }
    }

    public class SoundStream : SoundBuffer
    {
        public SoundStream()
        {
            Handle=OneAPI.SoundStream_Create();
        }

        public SoundStream(string file)
        {
            Handle = OneAPI.SoundStream_Create_ByPath(file);
        }

        protected override void CloseHandle(IntPtr handle)
        {
            OneAPI.SoundStream_Destroy(handle);
        }

        public override void Load(string file)
        {
            OneAPI.SoundStream_Load(Handle, file);
        }
    }

    public class SoundObject:OneObject
    {
        public bool IsStreamable
        {
            get
            {
                return OneAPI.SoundObject_IsStreamable(Handle);
            }
        }

         
        public void SetSound(SoundBuffer sound, bool loop, bool play, float volume)
        {
            OneAPI.SoundObject_SetSound(Handle, sound.Handle, loop, play, volume);
        }



        public bool IsEOS
        {
            get { return OneAPI.SoundObject_IsEOS(Handle); }
        }

        public void Play()
        {
            OneAPI.SoundObject_Play(Handle);
        }

        public  void Play(SoundBuffer sound, bool loop, bool play, float volume)
        {
            OneAPI.SoundObject_PlaySound(Handle, sound.Handle, loop, play, volume);
        }

        public void  Stop()
        {
            OneAPI.SoundObject_Stop(Handle);
        }

        public void Pause()
        {
            OneAPI.SoundObject_Pause(Handle);
        }

        public void Rewind()
        {
            OneAPI.SoundObject_Rewind(Handle);
        }

        public bool IsPlaying
        {
            get { return OneAPI.SoundObject_IsPlaying(Handle); }
        }

        public bool IsStopped
        {
            get{ return OneAPI.SoundObject_IsStopped(Handle);}
        }

        public bool IsPaused
        {
            get{return OneAPI.SoundObject_IsPaused(Handle);}
        }

        public bool IsInitial
        {
            get{return OneAPI.SoundObject_IsInitial(Handle);}
        }

        public bool IsLooping
        {
            get { return OneAPI.SoundObject_IsLooping(Handle); }
            set { OneAPI.SoundObject_SetLooping(Handle, value); }
        }

        public void SetLooping(bool looping)
        {
            OneAPI.SoundObject_SetLooping(Handle,looping);
        }


        public float Volume
        {
            get { return OneAPI.SoundObject_GetVolume(Handle); }
            set { OneAPI.SoundObject_SetVolume(Handle, value); }
        }

        public int PlaybackPosition
        {
            get { return OneAPI.SoundObject_GetPlaybackPosition(Handle); }
            set { OneAPI.SoundObject_SetPlaybackPosition(Handle, value); }
        }
         

        public int GetPlaybackSize()
        {
            return OneAPI.SoundObject_GetPlaybackSize(Handle);
        }

        public int GetSamplesPerSecond()
        {
            return OneAPI.SoundObject_GetSamplesPerSecond(Handle);
        }
    }

    public class OneListener : OneObject
    {
        public OneListener()
        {
            Handle= OneAPI.Listener_Create();
        }

        protected override void CloseHandle(IntPtr handle)
        {
            OneAPI.Listener_Destroy(handle);
        }

        public IntPtr GetListenerPtr()
        {
            return OneAPI.Listener_GetListener(Handle);
        }

        public void SetPositionOrientation(Vector position, Vector top, Vector front)
        {
            OneAPI.Listener_SetPositionOrientation(Handle, position, top, front);
        }


        public void SetListenerVelocity(Vector velocity)
        {
            OneAPI.Listener_SetListenerVelocity(Handle, velocity);
        }


        public void Update()
        {
            OneAPI.Listener_Update(Handle);
        }

        public Speaker Speaker
        {
            get { return OneAPI.Listener_GetSpeaker(Handle); }
            set { OneAPI.Listener_SetSpeaker(Handle, value); }
        }

        public void AddSound(Sound3D sound)
        {
            OneAPI.Listener_AddSound(Handle, sound.Handle);
        }

        public void RemoveSound(Sound3D sound)
        {
            OneAPI.Listener_RemoveSound(Handle, sound.Handle);
        }
    }

    public class Sound2D : SoundObject
    {
        public Sound2D()
        {
            Handle = OneAPI.Sound2D_Create();
        }

        public Sound2D(SoundBuffer buffer, bool loop, bool play, float volume)
        {
            Handle = OneAPI.Sound2D_Create_ByBuffer(buffer.Handle, loop, play, volume);
        }

        protected override void CloseHandle(IntPtr handle)
        {
            OneAPI.Sound2D_Destroy(handle);
        }

        public Speaker Speaker
        {
            get{return OneAPI.Sound2D_GetSpeaker(Handle);}
            set { OneAPI.Sound2D_SetSpeaker(Handle, value); }
        }

        //public void SetSpeakerVolume(float[] volume, int count)
        //{
        //    OneAPI.Sound2D_SetSpeakerVolume(Handle, volume, count);
        //}


        public bool IsMono
        {
            get { return OneAPI.Sound2D_GetMono(Handle); }
            set { OneAPI.Sound2D_SetMono(Handle, value); }
        }
    }

    public class Sound3D : SoundObject
    {
        public Sound3D()
        {
            Handle = OneAPI.Sound3D_Create();
        }

        public Sound3D(SoundBuffer buffer, bool loop, bool play, float volume)
        {
            Handle = OneAPI.Sound3D_Create_ByBuffer(buffer.Handle, loop, play, volume);
        }
        protected override void CloseHandle(IntPtr handle)
        {
            OneAPI.Sound3D_Destroy(handle);
        }

        public void SetSourcePosition(Vector position)
        {
            OneAPI.Sound3D_SetSourcePosition(Handle,position);
        }
    }

    class OneAPI
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
        public static extern int OneSound_GetOutputChannels(IntPtr handle);

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
        public static extern void Sound2D_SetSpeakerVolume(IntPtr handle, [In]float[] volume, int count);
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
        public static extern void Sound3D_SetSourcePosition(IntPtr handle, Vector position);
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
        public static extern void SoundObject_SetLooping(IntPtr so,  bool looping);
        [DllImport(dllName)]
        public static extern void SoundObject_SetVolume(IntPtr so,  float gain);
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
        public static extern void Listener_SetPositionOrientation(IntPtr listener, Vector position, Vector top, Vector front);
        [DllImport(dllName)]
        public static extern IntPtr Listener_GetListener(IntPtr listener);

        [DllImport(dllName)]
        public static extern void Listener_SetListenerVelocity(IntPtr listener, Vector velocity);
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
        public static extern void X3DAudioCore_XACT3DCalculate(IntPtr X3DInstance, IntPtr pListener, IntPtr pEmitter, IntPtr pDSPSettings);
        [DllImport(dllName)]
        public static extern void X3DAudioCore_X3DAudioCalculate(IntPtr instance, IntPtr listener, IntPtr emitter, uint flags,
            IntPtr dspSettingsPtr);
        [DllImport(dllName)]
        public static extern IntPtr X3DAudioCore_GetInstance();
    }
}

