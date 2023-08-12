/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#pragma once

#include "OneSound\Export.h"

#include "OneSound\SoundType\SoundObject.h"

namespace onesnd
{
    class ONE_SOUND_API Sound2D : public SoundObject
    {
    public:
        Sound2D();
       ~Sound2D();

        Sound2D(SoundBuffer* sound, const bool& loop = false, const bool& play = false, const float& volume = 1.f);
        
        void setSpeaker(const uint32_t& speaker);
        void setSpeakerVolume(float* volume, int count);
        uint32_t getSpeaker() const{
            return  channelMask;
        }
        void setMono(const bool& mono){
            isMono = mono;
        }
        bool getMono() const{
            return isMono;
        }
    protected:
        virtual void onSoundChanged() override;
    private:
        uint32_t channelMask;
        float* maskVolume;
        int maskCount;
        bool isMono;
    };
}
