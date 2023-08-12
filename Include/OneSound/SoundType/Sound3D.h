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
    class ONE_SOUND_API Sound3D : public SoundObject
    {
    protected: 
        X3DAUDIO_DSP_SETTINGS dspSettings;
    public:
        Sound3D();
        ~Sound3D();

        Sound3D(SoundBuffer* sound, const bool& loop = false, const bool& play = false, const float& volume = 1.f);
          
        void setSourcePosition(const X3DAUDIO_VECTOR& position); 
        void removeSpeaker(const uint32_t& speaker) const;
        void setSpeakerVolume(const uint32_t& speaker, float lVolume, float rVolume) const;
        inline X3DAUDIO_DSP_SETTINGS* getDspSetting() { return &dspSettings; }
    protected:
        void onSoundChanged() override;
    };
}
