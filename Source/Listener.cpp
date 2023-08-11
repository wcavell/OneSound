/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\Listener.h"

#include "OneSound\XAudio2Device.h"

namespace onesnd
{
    Listener::Listener()
    {
        X3DAUDIO_LISTENER m_listener;
        ZeroMemory(&m_listener, sizeof(X3DAUDIO_LISTENER));
        xListener = &m_listener; 
    }
    Listener::~Listener()
    {
        xListener = nullptr;
        for (auto it = sounds.begin(); it != sounds.end(); ++it)
        {
            if (*it)
                (*it)->removeSpeaker(speakerMask);
        }
        sounds.clear();
    }

    void Listener::setVolume(float volume)
    {
        if (volume < 0.f)
            volume = 0.f;
        else if (volume > 1.f)
            volume = 1.f;
        else
            XAudio2Device::instance().getMaster()->SetVolume(volume);
    }

    float Listener::getVolume() const
    {
        float value;
        XAudio2Device::instance().getMaster()->GetVolume(&value);

        return value;
    }
    void Listener::setPositionOrientation(const X3DAUDIO_VECTOR& position, const X3DAUDIO_VECTOR& top, const X3DAUDIO_VECTOR& front)
    {
        xListener->Position = position;
        xListener->OrientTop = top;
        xListener->OrientFront = front;
    }
    void Listener::setListenerVelocity(const X3DAUDIO_VECTOR& velocity)
    {
        xListener->Velocity = velocity;
    }

    void Listener::update()
    {
        for (auto it = sounds.begin(); it != sounds.end(); )
        {
            if (*it == nullptr)
            {
                it = sounds.erase(it);
            }
            else
            {
                auto sound = *it;
                auto dsp = sound->getDspSetting();
                XACT3DCalculate(XAudio2Device::instance().X3DInstance, xListener, &sound->getEmitter(), dsp);
                sound->setSpeakerVolume(speakerMask, dsp->pMatrixCoefficients[0], dsp->pMatrixCoefficients[1]);
                ++it;
            }
        }
    }

    void  Listener::addSound(Sound3D* sound)
    {
        sounds.push_back(sound);
    }

    void  Listener::removeSound(Sound3D* sound)
    {
        sounds.remove(sound);
        sound->removeSpeaker(speakerMask);
    }
}
