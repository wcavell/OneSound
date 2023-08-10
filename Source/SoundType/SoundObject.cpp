/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#pragma once

#include "OneSound\SoundType\SoundObject.h"

#include "OneSound\SoundType\SoundBuffer.h"
#include "OneSound\SoundType\SoundStream.h"

#include "OneSound\XAudio2Device.h"

#include "OneSound\StreamType\AudioStream.h"

namespace onesnd
{
    SoundObject::SoundObject() : 
        sound(nullptr), 
        source(nullptr), 
        state(nullptr),
        channelMatrix(nullptr),
        leftChannel(0), 
        rightChannel(0),
		channelMap(nullptr),
        outChannelCount(0),
        soundChannel(0),matrixAvailable(false)
    {
        memset(&Emitter, 0, sizeof(Emitter));
        Emitter.ChannelCount = 1;
        Emitter.CurveDistanceScaler = FLT_MIN;
        setChannelMap();
    }

    SoundObject::SoundObject(const std::shared_ptr<SoundBuffer>& sound, const bool& looping, const bool& playing, const float& volume) :
        sound(nullptr), 
        source(nullptr), 
        state(nullptr),
        channelMatrix(nullptr),
        leftChannel(0),
        rightChannel(0),
        channelMap(nullptr),
		outChannelCount(0),
        soundChannel(0), matrixAvailable(false)
    {
        memset(&Emitter, 0, sizeof(Emitter));
        Emitter.ChannelCount = 1;
        Emitter.CurveDistanceScaler = FLT_MIN;

        if (sound)
            setSound(sound); 

        setLooping(looping);

        setVolume(volume);

        if (playing)
            play();

        setChannelMap();
    }

    SoundObject::~SoundObject()
    {  
        if (channelMatrix)
        {
            delete[] channelMatrix;
            channelMatrix = nullptr;
        }
        if(channelMap)
        {
            delete[] channelMap;
            channelMap = nullptr;
        }
        if (sound) 
            setSound(nullptr);

        if (source)
        {
            source->DestroyVoice();
            source = nullptr;
        }

      
    }

    void SoundObject::setSound(const std::shared_ptr<SoundBuffer>& sound_buf, const bool& loop, const bool& play, const float& volume)
    {
        if (sound) 
            sound->UnbindSource(this); // unbind old, but still keep it around

        if (sound_buf) // new sound?
        {
            if (!source) // no Source object created yet? First init.
                state = new SoundObjectState(this);
            else if (sound_buf->WaveFormatHash() != sound->WaveFormatHash()) // WaveFormat has changed?
                source->DestroyVoice(); // Destroy old and re-create with new

            XAudio2Device::instance().getEngine()->CreateSourceVoice(&source, sound_buf->WaveFormat(), 0, 2.0F, state);

            sound_buf->BindSource(this);

            state->isInitial = true;
            state->isPlaying = play;
            state->isLoopable = loop;
            state->isPaused = false;           
            sound = sound_buf; // set new Sound
            setVolume(volume);
            if (soundChannel == 0)
                soundChannel = sound->Channels();
            onSoundChanged();
        }
    }

    bool SoundObject::isStreamable() const
    {
        return sound && sound->IsStream();
    }

    bool SoundObject::isEOS() const
    {
        return sound && sound->IsStream() && ((SoundStream*)sound.get())->IsEOS(this);
    }

    void SoundObject::play()
    {
        if (state->isPlaying) 
            rewind(); // rewind to start of stream and continue playing
        else if (source)
        {
            state->isPlaying = true;
            state->isPaused = false;

            if (!XABuffer::getBuffersQueued(source)) // no buffers queued (track probably finished)
            {
                state->isInitial = true;
                sound->ResetBuffer(this); // reset buffer to beginning
            }

            source->Start(); // continue if paused or suspended
        }
    }

    void SoundObject::play(const std::shared_ptr<SoundBuffer>& sound, const bool& loop, const bool& play, const float& volume)
    {
        if (sound)
            setSound(sound);

        setLooping(loop);

        setVolume(volume);

        if (play)
            this->play();
        //source->SetOutputMatrix()
       
    }

    void SoundObject::stop()
    {
        if (source && state->isPlaying)
        { 
            // only if isPlaying, to avoid rewind
            state->isPlaying = false;
            state->isPaused = false;

            source->Stop();
            source->FlushSourceBuffers();
        }
    }

    void SoundObject::pause()
    {
        if (source)
        {
            state->isPlaying = false;
            state->isPaused = true;

            source->Stop(); // Stop() effectively pauses playback
        }
    }

    void SoundObject::rewind()
    {
        sound->ResetBuffer(this); // reset stream or buffer to initial state

        state->isInitial = true;
        state->isPaused = false;

        if (state->isPlaying) // should we continue playing?
            source->Start();
    }

    bool SoundObject::isPlaying() const
    {
        return state && state->isPlaying;
    }

    bool SoundObject::isStopped() const
    {
        return !state || !state->isPlaying;
    }

    bool SoundObject::isPaused() const
    {
        return state && state->isPaused;
    }

    bool SoundObject::isInitial() const
    {
        return state && state->isInitial;
    }


    bool SoundObject::isLooping() const
    {
        return state && state->isLoopable;
    }

    void SoundObject::setLooping(const bool& looping)
    {
        if (state) 
            state->isLoopable = looping;
    }

    void SoundObject::setVolume(const float& volume)
    {
        // HACK: Check the current volume if offered value is more than 1.0 or less than 0.
        //       It can be even 1000.0f, but it couldn't be looked like the sound.
        if (volume > 1.f)
            source->SetVolume(1.f);
        else if (volume < 0.f)
            source->SetVolume(0.f);
        else
            source->SetVolume(volume);
    }

    float SoundObject::getVolume() const
    {
        float volume;
        source->GetVolume(&volume);
        return volume;
    }

    int SoundObject::getPlaybackPosition() const
    {
        if (!source) 
            return 0;

        XAUDIO2_VOICE_STATE state;
        source->GetState(&state);

        return (int)state.SamplesPlayed;
    }

    void SoundObject::setPlaybackPosition(int seekpos)
    {
        if (!sound) 
            return;
        
        if (sound->IsStream()) // stream objects
            ((SoundStream*)sound.get())->Seek(this, seekpos); // seek the stream
        else // single buffer objects
        {
            // first create a shallow copy of the xaBuffer:
            auto& shallow = state->shallow = *sound->getXABuffer();
            shallow.PlayBegin = seekpos;
            shallow.PlayLength = shallow.nPCMSamples - seekpos;

            state->isPaused = false;
            source->Stop();

            if (XABuffer::getBuffersQueued(source)) // only flush if there is something to flush
                source->FlushSourceBuffers();

            source->SubmitSourceBuffer(&shallow);
        }

        if (state->isPlaying)
            source->Start(); 
    }

    int SoundObject::getPlaybackSize() const
    {
        return sound ? sound->Size() : 0;
    }


    int SoundObject::getSamplesPerSecond() const
    {
        return sound ? sound->Frequency() : 0;
    } 
    void SoundObject::setOutChannel(const uint32_t& speakerLeftChannel, const uint32_t& speakerRightChannel)
    {
        leftChannel = speakerLeftChannel;
        rightChannel = speakerRightChannel;
        if (sound)
        {
        	

			channelMatrix =new float[soundChannel * outChannelCount]; 
           
            if(matrixAvailable)
            {
                setOutChannelVolume(1.0f, 1.0f);
            }
        }
    }
    void SoundObject::setOutChannelVolume(const float& leftVolume, const float& rightVolume)
    {
        if (soundChannel == 1)
        {
            for (int i = 0; i < outChannelCount; ++i)
            {
                if (channelMap[i] == leftChannel)
                {
                    channelMatrix[i] = leftVolume;
                }
                else if (channelMap[i] == rightChannel)
                {
                    channelMatrix[i] = rightVolume;
                }
                else
                {
                    channelMatrix[i] = 0.0f;
                }
            }
        }
        else  if (soundChannel == 2)
        {
            for (int i = 0; i < outChannelCount; ++i)
            {
                for (int n = 0; n < 2; ++n)
                {
                    //channelMatrix[i * soundChannels + n];
                }
                if (channelMap[i] == leftChannel)
                {
                    channelMatrix[i * 2 + 0] = 1.0f;
                    channelMatrix[i * 2 + 1] = 0.0f;
                }
                else if (channelMap[i] == rightChannel)
                {
                    channelMatrix[i * 2 + 0] = 0.0f;
                    channelMatrix[i * 2 + 1] = rightVolume;
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
                if (channelMap[i] == leftChannel)
                {
                    for (int n = 0; n < soundChannel; ++n)
                    {
                        channelMatrix[i * soundChannel + n] = 1.0f;
                    }
                }
                else if (channelMap[i] == rightChannel)
                {
                    for (int n = 0; n < soundChannel; ++n)
                    {
                        channelMatrix[i * soundChannel + n] = 1.0f;
                    }
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
    void SoundObject::setSoundChannel(const int& channel) {
        soundChannel = channel;
    }
    void SoundObject::onSoundChanged()
    {
	    
    }
    void SoundObject::setChannelMap()
    {
        outChannelCount = XAudio2Device::instance().getChannelCount();
        channelMap = new uint32_t[outChannelCount];
        matrixAvailable = true;

        switch (outChannelCount)
        {
            //Speaker   Left Source           Right Source
        case 2://2.0 
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            break;
        case 3:
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_LOW_FREQUENCY;
            break;
        case 4: // 4.0 
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_BACK_LEFT;
            channelMap[3] = SPEAKER_BACK_RIGHT;
            break;
        case 5: // 5.0  
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_LOW_FREQUENCY;
            channelMap[3] = SPEAKER_SIDE_LEFT;
            channelMap[4] = SPEAKER_SIDE_RIGHT;
            break;
        case 6: // 5.1 
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_FRONT_CENTER;
            channelMap[3] = SPEAKER_LOW_FREQUENCY;
            channelMap[4] = SPEAKER_SIDE_LEFT;
            channelMap[5] = SPEAKER_SIDE_RIGHT;
            break;
        case 7: // 6.1 
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_FRONT_CENTER;
            channelMap[3] = SPEAKER_LOW_FREQUENCY;
            channelMap[4] = SPEAKER_SIDE_LEFT;
            channelMap[5] = SPEAKER_SIDE_RIGHT;
            channelMap[6] = SPEAKER_BACK_CENTER;
            break;
        case 8: // 7.1 
            channelMap[0] = SPEAKER_FRONT_LEFT;
            channelMap[1] = SPEAKER_FRONT_RIGHT;
            channelMap[2] = SPEAKER_FRONT_CENTER;
            channelMap[3] = SPEAKER_LOW_FREQUENCY;
            channelMap[4] = SPEAKER_BACK_LEFT;
            channelMap[5] = SPEAKER_BACK_RIGHT;
            channelMap[6] = SPEAKER_SIDE_LEFT;
            channelMap[7] = SPEAKER_SIDE_RIGHT;
            break;
        default:
            matrixAvailable = false;
            break;
        }
    }

}
