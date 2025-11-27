/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#pragma once

#include "OneSound\Export.h"

#include "..\ThirdParty\Include\XAudio2_7\XAudio2.h"
#include "..\ThirdParty\Include\XAudio2_7\X3DAudio.h"
#include "..\ThirdParty\Include\XAudio2_7\xact3d3.h"

#if defined (_WIN64)
#   pragma comment(lib, "X3DAudio64.lib")
#else
#   pragma comment(lib, "X3DAudio86.lib")
#endif

namespace onesnd
{
    class SoundBuffer;
    class AudioStream;
    class CMMNotificationClient;

    class XAudio2Device
    {
    public:
        XAudio2Device();
       ~XAudio2Device() = default;

    public:
        static XAudio2Device& instance()
        {
            static XAudio2Device xaudio2_device;
            return xaudio2_device;
        }

    public:
        void initialize();
        void finalize();    

        IXAudio2* getEngine() const { return xEngine; }
        IXAudio2MasteringVoice* getMaster() const { return xMaster; }
        unsigned int getChannelCount() const { return channelCount; }
        unsigned int refreshChannelCount();
        uint32_t getChannelMask() { return channelMask; }
        void getChannelInfo(uint32_t& count, uint32_t& mask);
        inline unsigned int getLeftSpeaker()const { return leftSpeaker; }
        inline unsigned int getRightSpeaker()const { return  rightSpeaker; }         
        void setLeftSpeaker(const unsigned int &lSpeaker)
        {
            leftSpeaker = lSpeaker;          
        }
        void setRightSpeaker(const unsigned int& rSpeaker)
        {
            rightSpeaker = rSpeaker;
        }
    public:
        X3DAUDIO_HANDLE X3DInstance;
    private:
        IXAudio2* xEngine;
        IXAudio2MasteringVoice* xMaster;
        unsigned int channelCount;
        uint32_t channelMask;
        X3DAUDIO_LISTENER xListener;
        unsigned int leftSpeaker;
        unsigned int rightSpeaker;
        CMMNotificationClient* mmClient;
        XAUDIO2_DEVICE_DETAILS deviceDetails;
    	
    };
    
    struct XABuffer : XAUDIO2_BUFFER
    {
        WAVEFORMATEX wf;		// wave format descriptor
        int nBytesPerSample;	// number of bytes per single audio sample (1 or 2 bytes)
        int nPCMSamples;		// number of PCM samples in the entire buffer
        unsigned wfHash;		// waveformat pseudo-hash

        static XABuffer* create(SoundBuffer* ctx, int size, AudioStream* strm, int* pos = nullptr);
        static void destroy(XABuffer*& buffer);

        static void stream(XABuffer* buffer, AudioStream* strm, int* pos = nullptr);

        static int getBuffersQueued(IXAudio2SourceVoice* source);
    };
}