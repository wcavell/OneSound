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
        rightChannel(0)
    {
        memset(&Emitter, 0, sizeof(Emitter));
        Emitter.ChannelCount = 1;
        Emitter.CurveDistanceScaler = FLT_MIN;
    }

    SoundObject::SoundObject(const std::shared_ptr<SoundBuffer>& sound, const bool& looping, const bool& playing, const float& volume) :
        sound(nullptr), 
        source(nullptr), 
        state(nullptr),
        channelMatrix(nullptr),
        leftChannel(0),
        rightChannel(0)
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
    }

    SoundObject::~SoundObject()
    {  
        if (channelMatrix != nullptr)
        {
            free(channelMatrix);
            channelMatrix = nullptr;
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
            XAUDIO2_DEVICE_DETAILS dd;
            ZeroMemory(&dd, sizeof(dd));
            XAudio2Device::instance().getEngine()->GetDeviceDetails(0, &dd);
            channelMatrix = (float*)malloc(sizeof(float) * sound->Channels() * dd.OutputFormat.Format.nChannels);
            auto matrix = channelMatrix;
            bool matrixAvailable = true;
            if (sound->Channels() == 1) 
            {

            }
            else
            {
				switch (dd.OutputFormat.Format.nChannels)
				{
                //Speaker   Left Source           Right Source
				case 2://2.0
                    /*Front L*/	
                    if (leftChannel == SPEAKER_FRONT_LEFT)
                        matrix[0] = matrix[1] = 1.0000f;
                    else 
                        matrix[0] = matrix[1] = 0.0000f;
                    /*Front R*/	
                    if (rightChannel == SPEAKER_FRONT_RIGHT)
                        matrix[2] = matrix[3] = 1.0f;
                    else 
                        matrix[2] = matrix[3] = 0.0f;
					break;
				case 4: // 4.0
					
					/*Front L*/	
                    matrix[0] = 1.0000f;  matrix[1] = 0.0000f;
					/*Front R*/	
                    matrix[2] = 0.0000f;  matrix[3] = 1.0000f;
					/*Back  L*/	
                    matrix[4] = 1.0000f;  matrix[5] = 0.0000f;
					/*Back  R*/	
                    matrix[6] = 0.0000f;  matrix[7] = 1.0000f;
					break;
				case 5: // 5.0
					//Speaker \ Left Source           Right Source
					/*Front L*/	
                    matrix[0] = 1.0000f;  matrix[1] = 0.0000f;
					/*Front R*/	
                    matrix[2] = 0.0000f;  matrix[3] = 1.0000f;
					/*Front C*/	
                    matrix[4] = 0.7071f;  matrix[5] = 0.7071f;
					/*Side  L*/	
                    matrix[6] = 1.0000f;  matrix[7] = 0.0000f;
					/*Side  R*/	
                    matrix[8] = 0.0000f;  matrix[9] = 1.0000f;
					break;
				case 6: // 5.1
					//Speaker \ Left Source           Right Source
					/*Front L*/	matrix[0] = 1.0000f;  matrix[1] = 0.0000f;
					/*Front R*/	matrix[2] = 0.0000f;  matrix[3] = 1.0000f;
					/*Front C*/	matrix[4] = 0.7071f;  matrix[5] = 0.7071f;
					/*LFE    */	matrix[6] = 0.0000f;  matrix[7] = 0.0000f;
					/*Side  L*/	matrix[8] = 1.0000f;  matrix[9] = 0.0000f;
					/*Side  R*/	matrix[10] = 0.0000f;  matrix[11] = 1.0000f;
					break;
				case 7: // 6.1
					//Speaker \ Left Source           Right Source
					/*Front L*/	matrix[0] = 1.0000f;  matrix[1] = 0.0000f;
					/*Front R*/	matrix[2] = 0.0000f;  matrix[3] = 1.0000f;
					/*Front C*/	matrix[4] = 0.7071f;  matrix[5] = 0.7071f;
					/*LFE    */	matrix[6] = 0.0000f;  matrix[7] = 0.0000f;
					/*Side  L*/	matrix[8] = 1.0000f;  matrix[9] = 0.0000f;
					/*Side  R*/	matrix[10] = 0.0000f;  matrix[11] = 1.0000f;
					/*Back  C*/	matrix[12] = 0.7071f;  matrix[13] = 0.7071f;
					break;
				case 8: // 7.1
					//Speaker \ Left Source           Right Source
					/*Front L*/	matrix[0] = 1.0000f;  matrix[1] = 0.0000f;
					/*Front R*/	matrix[2] = 0.0000f;  matrix[3] = 1.0000f;
					/*Front C*/	matrix[4] = 0.7071f;  matrix[5] = 0.7071f;
					/*LFE    */	matrix[6] = 0.0000f;  matrix[7] = 0.0000f;
					/*Back  L*/	matrix[8] = 1.0000f;  matrix[9] = 0.0000f;
					/*Back  R*/	matrix[10] = 0.0000f;  matrix[11] = 1.0000f;
					/*Side  L*/	matrix[12] = 1.0000f;  matrix[13] = 0.0000f;
					/*Side  R*/	matrix[14] = 0.0000f;  matrix[15] = 1.0000f;
					break;
				default:
					matrixAvailable = false;
					break;
				}
            }
            
        }       
    }
}
