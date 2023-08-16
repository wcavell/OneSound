using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using XAudio.X3DAudio;

namespace XAudio
{
    public class OneAudio:NativeObject
    {
        /// <summary>
        /// 2.0
        /// </summary>
        public static Speaker SpeakerStereo
        {
            get
            {
                return Speaker.FrontLeft | Speaker.FrontRight;
            }
        }

        /// <summary>
        /// 4.0
        /// </summary>
        public static Speaker SpeakerQuad
        {
            get
            {
                return Speaker.FrontLeft | Speaker.FrontRight | Speaker.BackLeft | Speaker.BackRight;
            }
        }

        /// <summary>
        /// 5.1
        /// </summary>
        public static Speaker Speaker5Point1
        {
            get
            {
                return Speaker.FrontLeft | Speaker.FrontRight | Speaker.FrontCenter | Speaker.LowFrequency |
                       Speaker.SideLeft | Speaker.SideRight;
            }
        }

        /// <summary>
        /// 7.1
        /// </summary>
        public static Speaker Speaker7Point1
        {
            get
            {
                return Speaker.FrontLeft | Speaker.FrontRight | Speaker.FrontCenter | Speaker.LowFrequency |
                       Speaker.BackLeft | Speaker.BackRight | Speaker.SideLeft | Speaker.SideRight;
            }
        }

        /// <summary>
        /// 左声道映射 
        /// 默认值 FrontLeft | FrontCenter | BackLeft | SideLeft 
        /// </summary>
        public static Speaker LeftSpeakerMap
        {
            get
            {
                return NativeMethods.OneSound_GetLeftSpeakerMap();
            }
            set
            {
                NativeMethods.OneSound_SetLeftSpeakerMap(value);
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
                return NativeMethods.OneSound_GetRightSpeakerMap();
            }
            set
            {
                NativeMethods.OneSound_SetRightSpeakerMap(value);
            }
        }

        public static int SpeakerCount
        {
            get { return NativeMethods.OneSound_GetOutputChannels(); }
        }

        public OneAudio()
        {
            Handle = NativeMethods.OneSound_Create();
        }

        protected override void CloseHandle()
        {
            NativeMethods.OneSound_Destroy(Handle);
        }


        public static void XACT3DCalculate(IntPtr X3DInstance, IntPtr pListener, IntPtr pEmitter,
            IntPtr pDSPSettings)
        {
            NativeMethods.X3DAudioCore_XACT3DCalculate(X3DInstance, pListener, pEmitter, pDSPSettings);
        }

        public static void X3DAudioCalculate(IntPtr instance, IntPtr listener, IntPtr emitter, CalculateFlags flags,
            IntPtr dspSettingsPtr)
        {
            NativeMethods.X3DAudioCore_X3DAudioCalculate(instance, listener, emitter, flags, dspSettingsPtr);
        }

        public static IntPtr GetX3DAudioInstance()
        {
            return NativeMethods.X3DAudioCore_GetInstance();
        }
    }
}
