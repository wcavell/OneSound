using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using XAudio.Utils;
using XAudio.X3DAudio;
using static Unity.IO.LowLevel.Unsafe.AsyncReadManagerMetrics;

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
        /// 5.1变种
        /// </summary>
        public static Speaker Speaker5Point2
        {
            get
            {
                return Speaker.FrontLeft | Speaker.FrontRight | Speaker.FrontCenter | Speaker.LowFrequency |
                       Speaker.BackLeft | Speaker.BackRight;
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

        private static int _SpCount;
        private static Speaker _speaker;

        public static Speaker SpeakerMask
        {
            get
            {
                NativeMethods.OneSound_GetChannelInfo(ref _SpCount, ref _speaker);
                return _speaker;
            }
        }

        public OneAudio()
        {
            Handle = NativeMethods.OneSound_Create();
        }

        protected override void CloseHandle()
        {
            NativeMethods.OneSound_Destroy(Handle);
        }


        public unsafe void XACT3DCalculate(Listener listener, Emitter emitter,
            DspSettings settings)
        {
            //NativeMethods.X3DAudioCore_XACT3DCalculate(instance, pListener, pEmitter, pDSPSettings);

            if (settings == null)
                throw new ArgumentNullException("settings");
            if (listener == null)
                throw new ArgumentNullException("listener");
            if (emitter == null)
                throw new ArgumentNullException("emitter"); 

            if (emitter.ChannelAzimuths == null)
                throw new ArgumentException("No ChannelAzimuths set for the specified emitter. The ChannelAzimuths property must not be null if the ChannelCount of the emitter is bigger than 1.");

            DspSettings.DspSettingsNative nativeSettings = settings.NativeInstance;
            Listener.ListenerNative nativeListener = listener.NativeInstance;
            Emitter.EmitterNative nativeEmitter = emitter.NativeInstance;

            #region setup listener

            //setup listener:
            Cone listenerCone = listener.Cone.HasValue ? listener.Cone.Value : default(Cone);
            IntPtr listenerConePtr = listener.Cone.HasValue ? (IntPtr)(&listenerCone) : IntPtr.Zero;
            nativeListener.ConePtr = listenerConePtr;

            #endregion

            #region setup emitter

            //setup emitter
            IntPtr channelAzimuthsPtr = IntPtr.Zero;
            if (emitter.ChannelAzimuths != null && emitter.ChannelAzimuths.Length > 0 && emitter.ChannelCount > 0)
            {
                const int sizeOfFloat = sizeof(float);
                int channelAzimuthsSize = sizeOfFloat *
                                          Math.Min(emitter.ChannelCount, emitter.ChannelAzimuths.Length);
                channelAzimuthsPtr = Marshal.AllocHGlobal(channelAzimuthsSize);
                ILUtils.WriteToMemory(channelAzimuthsPtr, emitter.ChannelAzimuths, 0,
                    channelAzimuthsSize / sizeOfFloat);
            }

            Cone emitterCone = emitter.Cone.HasValue ? emitter.Cone.Value : default(Cone);
            IntPtr emitterConePtr = emitter.Cone.HasValue ? (IntPtr)(&emitterCone) : IntPtr.Zero;

            nativeEmitter.ChannelAzimuthsPtr = channelAzimuthsPtr;
            nativeEmitter.ConePtr = emitterConePtr;
            nativeEmitter.LFECurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowFrequencyEffectCurve);
            nativeEmitter.LPFDirectCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowPassFilterDirectCurve);
            nativeEmitter.LPFReverbCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowPassFilterReverbCurve);
            nativeEmitter.ReverbCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.ReverbCurve);
            nativeEmitter.VolumeCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.VolumeCurve);

            #endregion

            #region setup settings

            //setup settings
            fixed (void* pmc = settings.MatrixCoefficients, pdt = settings.DelayTimes)
            {
                nativeSettings.MatrixCoefficientsPtr = new IntPtr(pmc);
                nativeSettings.DelayTimesPtr = new IntPtr(pdt);

                NativeMethods.OneSound_XACT3DCalculate( (IntPtr)(&nativeListener),
                    (IntPtr)(&nativeEmitter), new IntPtr(&nativeSettings));

                settings.NativeInstance = nativeSettings;
            }
            #endregion

            nativeEmitter.FreeMemory();
        }

        public unsafe void X3DAudioCalculate(Listener listener, Emitter emitter, CalculateFlags flags,
            DspSettings settings)
        {

            if (settings == null)
                throw new ArgumentNullException("settings");
            if (listener == null)
                throw new ArgumentNullException("listener");
            if (emitter == null)
                throw new ArgumentNullException("emitter");
            if ((int)flags == 0)
                throw new ArgumentOutOfRangeException("flags");

            if (emitter.ChannelCount > 1 && emitter.ChannelAzimuths == null)
                throw new ArgumentException(
                    "No ChannelAzimuths set for the specified emitter. The ChannelAzimuths property must not be null if the ChannelCount of the emitter is bigger than 1.");

            DspSettings.DspSettingsNative nativeSettings = settings.NativeInstance;
            Listener.ListenerNative nativeListener = listener.NativeInstance;
            Emitter.EmitterNative nativeEmitter = emitter.NativeInstance;

            #region setup listener

            //setup listener:
            Cone listenerCone = listener.Cone.HasValue ? listener.Cone.Value : default(Cone);
            IntPtr listenerConePtr = listener.Cone.HasValue ? (IntPtr)(&listenerCone) : IntPtr.Zero;
            nativeListener.ConePtr = listenerConePtr;

            #endregion

            #region setup emitter

            //setup emitter
            IntPtr channelAzimuthsPtr = IntPtr.Zero;
            if (emitter.ChannelAzimuths != null && emitter.ChannelAzimuths.Length > 0 && emitter.ChannelCount > 0)
            {
                const int sizeOfFloat = sizeof(float);
                int channelAzimuthsSize = sizeOfFloat *
                                          Math.Min(emitter.ChannelCount, emitter.ChannelAzimuths.Length);
                channelAzimuthsPtr = Marshal.AllocHGlobal(channelAzimuthsSize);
                ILUtils.WriteToMemory(channelAzimuthsPtr, emitter.ChannelAzimuths, 0,
                    channelAzimuthsSize / sizeOfFloat);
            }

            Cone emitterCone = emitter.Cone.HasValue ? emitter.Cone.Value : default(Cone);
            IntPtr emitterConePtr = emitter.Cone.HasValue ? (IntPtr)(&emitterCone) : IntPtr.Zero;

            nativeEmitter.ChannelAzimuthsPtr = channelAzimuthsPtr;
            nativeEmitter.ConePtr = emitterConePtr;
            nativeEmitter.LFECurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowFrequencyEffectCurve);
            nativeEmitter.LPFDirectCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowPassFilterDirectCurve);
            nativeEmitter.LPFReverbCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.LowPassFilterReverbCurve);
            nativeEmitter.ReverbCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.ReverbCurve);
            nativeEmitter.VolumeCurvePtr = CurveNative.AllocMemoryAndBuildCurve(emitter.VolumeCurve);

            #endregion

            #region setup settings

            //setup settings
            fixed (void* pmc = settings.MatrixCoefficients, pdt = settings.DelayTimes)
            {
                nativeSettings.MatrixCoefficientsPtr = new IntPtr(pmc);
                nativeSettings.DelayTimesPtr = new IntPtr(pdt);

                NativeMethods.OneSound_X3DAudioCalculate((IntPtr)(&nativeListener),
                    (IntPtr)(&nativeEmitter), flags,
                    new IntPtr(&nativeSettings));

                settings.NativeInstance = nativeSettings;
            }

            #endregion

            nativeEmitter.FreeMemory();
        }
    }
}
