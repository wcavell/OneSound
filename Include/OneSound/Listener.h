/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#pragma once

#include <cstdint>
#include <list>

#include "OneSound\Export.h"
#include "OneSound\XAudio2Device.h"
#include "SoundType\Sound3D.h"

namespace onesnd
{
    // REDO: Listener class.
    class ONE_SOUND_API Listener
    {
    public:
        Listener();
        ~Listener();

        Listener(const Listener&) = default;
        Listener(Listener&&) = default;

        Listener operator=(const Listener&) = delete;
        Listener operator=(Listener&&) = delete;
    public:
        void setVolume(float volume);
        float getVolume() const;
        X3DAUDIO_LISTENER* getListener() { return &xListener; } 
        void setPositionOrientation(X3DAUDIO_VECTOR position, X3DAUDIO_VECTOR top, X3DAUDIO_VECTOR front);
        void setListenerVelocity(const X3DAUDIO_VECTOR& velocity);
        void update();
        void setSpeaker(const uint32_t& speaker)
        {
            speakerMask = speaker;
        }
        uint32_t getSpeaker()
        {
            return speakerMask;
        }
        void addSound(Sound3D* sound);
        void removeSound(Sound3D* sound);
    private:
        void setMatrix(); 
    private:
        X3DAUDIO_LISTENER xListener;
        uint32_t speakerMask; 
        std::list<Sound3D*> sounds;
        X3DAUDIO_HANDLE x3DAudioHandle;
    };
}
