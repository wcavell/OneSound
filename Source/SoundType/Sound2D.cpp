/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\SoundType\Sound2D.h"

namespace onesnd
{
    Sound2D::Sound2D() : SoundObject(),
        channelMask(0),
        maskVolume(nullptr),
        maskCount(-1)
    {
    }

    Sound2D::Sound2D(const std::shared_ptr<SoundBuffer>& sound, const bool& loop, const bool& play, const float& volume) : SoundObject(sound, loop, play, volume),
        channelMask(0),
        maskVolume(nullptr),
        maskCount(-1)
    {
	    
    }
    Sound2D::~Sound2D()
    {
        if(maskVolume)
        {
            delete[] maskVolume;
            maskVolume = nullptr;
        }
    }

    void Sound2D::onSoundChanged()
    {

    }
   void Sound2D::setSpeakerVolume(float* volume, int count) {
       maskVolume = volume;
       maskCount = count;
    }
    void Sound2D::setSpeaker(const uint32_t& speaker)
    {
        uint32_t lSpeaker = XAudio2Device::instance().getLeftSpeaker();
        uint32_t rSpeaker = XAudio2Device::instance().getRightSpeaker();

        channelMask = speaker;
        if (soundChannel == 1) {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] & channelMask) {
                    channelMatrix[i] = maskCount > 0 ? maskVolume[i] : 1.0f;
                }
                else
                {
                    channelMatrix[i] = 0.0f;
                }
            }
        }
        else if (soundChannel == 2) 
        {
            for (int i = 0; i < outChannelCount; ++i)
            {                
                if (channelMap[i] & channelMask)
                {
                    if (isMono)
                    {
                        channelMatrix[i * 2 + 0] = maskCount > 0 ? maskVolume[i] : 1.0f;
                        channelMatrix[i * 2 + 1] = maskCount > 0 ? maskVolume[i] : 1.0f;
                    }
                    else if(channelMap[i] & lSpeaker)
                    {
                        channelMatrix[i * 2 + 0] = maskCount > 0 ? maskVolume[i] : 1.0f;
                        channelMatrix[i * 2 + 1] = 0.0f;
                    }
                    else if(channelMap[i] & rSpeaker)
                    {
                        channelMatrix[i * 2 + 0] = 0.0f;
                        channelMatrix[i * 2 + 1] = maskCount > 0 ? maskVolume[i] : 1.0f;
                    }
                   
                }                 
                else
                {
                    channelMatrix[i * 2 + 0] = 0.0f;
                    channelMatrix[i * 2 + 1] = 0.0f;
                }
            }
        }
        else
        {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] & channelMask)
                {
                    for (int n = 0; n < soundChannel; ++n)
                    {
                        channelMatrix[i * soundChannel + n] = maskCount > 0 ? maskVolume[i] : 1.0f;
                    }
                    /*if(isMono)
                    {
                       
                    }
                    if (channelMap[i] & lSpeaker)
                    {
                        for (int n = 0; n < soundChannel; ++n)
                        {
                            channelMatrix[i * soundChannel + n] = maskCount > 0 ? maskVolume[i] : 1.0f;
                        }
                    }
                    else if (channelMap[i] & rSpeaker)
                    {
                        for (int n = 0; n < soundChannel; ++n)
                        {
                            channelMatrix[i * soundChannel + n] = maskCount > 0 ? maskVolume[i] : 1.0f;
                        }
                    }*/
                } 
                else
                {
                    for (int n = 0; n < soundChannel; ++n)
                    {
                        channelMatrix[i * soundChannel + n] = 0.0f;
                    }
                }
            }
        }
        source->SetOutputMatrix(nullptr, soundChannel, outChannelCount, channelMatrix);
    } 
}