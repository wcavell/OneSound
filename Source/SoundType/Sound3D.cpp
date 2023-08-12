/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\SoundType\Sound3D.h"
#include "OneSound\XAudio2Device.h"

namespace onesnd
{
    Sound3D::Sound3D() : SoundObject()
    {   
        ZeroMemory(&dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
        dspSettings.SrcChannelCount = 1;
        dspSettings.DstChannelCount = 2;
        float* mat = new float[2];
        dspSettings.pMatrixCoefficients = mat;
    }
    Sound3D::~Sound3D() 
    {
        if (dspSettings.pMatrixCoefficients != nullptr)
        {
            delete[] dspSettings.pMatrixCoefficients;
            dspSettings.pMatrixCoefficients = nullptr;
        }
    }
    Sound3D::Sound3D(SoundBuffer* sound, const bool& loop, const bool& play, const float& volume) : SoundObject(sound, loop, play, volume)
    {
         
        ZeroMemory(&dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
        dspSettings.SrcChannelCount = 1;
        dspSettings.DstChannelCount = 2;
        float* mat = new float[2];
        dspSettings.pMatrixCoefficients = mat;
    }
    

    void Sound3D::setSourcePosition(const X3DAUDIO_VECTOR& position)
    {
        emitter.Position = position;
    }

    void Sound3D::onSoundChanged()
    {
	    
    }
    void Sound3D::removeSpeaker(const uint32_t& speaker) const
    {
        for (int i = 0; i < outChannelCount; ++i)
        {
            if (channelMap[i] & speaker)
            {
                for (int n = 0; n < soundChannel; ++n)
                {
                    channelMatrix[i * soundChannel + n] = 0.0f;
                }
            }
        }
        source->SetOutputMatrix(nullptr, soundChannel, outChannelCount, channelMatrix);
    }
    void Sound3D::setSpeakerVolume(const uint32_t& speaker, float lVolume, float rVolume) const
    {
        uint32_t lSpeaker = XAudio2Device::instance().getLeftSpeaker();
        uint32_t rSpeaker = XAudio2Device::instance().getRightSpeaker();

        if (soundChannel == 1) {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] & speaker) 
                {
                    if (channelMap[i] & lSpeaker)
                        channelMatrix[i] = lVolume;
                    else if (channelMap[i] & rSpeaker)
                        channelMatrix[i] = rVolume;
                }
                 
            }
        }
        else if (soundChannel == 2) {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] & speaker)
                {
                    if (channelMap[i] & lSpeaker)
                    {
                        channelMatrix[i * 2 + 0] = channelMatrix[i] = lVolume;
                        channelMatrix[i * 2 + 1] = channelMatrix[i] = 0;
                    }
                    else if(channelMap[i]&rSpeaker)
                    {
                        channelMatrix[i * 2 + 0] = channelMatrix[i] = 0;
                        channelMatrix[i * 2 + 1] = channelMatrix[i] = rVolume;
                    }

                } 
            }
        }
        else
        {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] & speaker)
                {
                    if (channelMap[i] & lSpeaker)
                    {
                        for (int n = 0; n < soundChannel; ++n)
                        {
                            channelMatrix[i * soundChannel + n] = lVolume;
                        }
                    }
                    else if (channelMap[i] & rSpeaker)
                    {
                        for (int n = 0; n < soundChannel; ++n)
                        {
                            channelMatrix[i * soundChannel + n] = rVolume;
                        }
                    }                    
                } 
            }
        }
        source->SetOutputMatrix(nullptr, soundChannel, outChannelCount, channelMatrix);
    }
}