/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#pragma once

#include "OneSound\Export.h"
#include "OneSound\XAudio2Device.h"

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
        X3DAUDIO_LISTENER* getListener() const { return xListener; } 
        void setPositionOrientation(const X3DAUDIO_VECTOR& position, const X3DAUDIO_VECTOR& top, const X3DAUDIO_VECTOR& front);
        void setListenerVelocity(const X3DAUDIO_VECTOR& velocity);
    private:
        X3DAUDIO_LISTENER* xListener;
    };
}
