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
        soundChannel = 1;
    }
    Sound3D::~Sound3D() 
    {
        if (dspSettings.pMatrixCoefficients != nullptr)
        {
            delete[] dspSettings.pMatrixCoefficients;
            dspSettings.pMatrixCoefficients = nullptr;
        }
    }
    Sound3D::Sound3D(const std::shared_ptr<SoundBuffer>& sound, const bool& loop, const bool& play, const float& volume) : SoundObject(sound, loop, play, volume)
    { }

    void Sound3D::apply3D(X3DAUDIO_LISTENER* xListener)
    {
        listener = xListener;
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
    void Sound3D::update3D()
    {
        auto handle = XAudio2Device::instance().x3DAudioHandle;
        XACT3DCalculate(handle, listener, &emitter, &dspSettings);
        //setOutChannelVolume(dspSettings.pMatrixCoefficients[0], dspSettings.pMatrixCoefficients[1]);
    }
    void Sound3D::onSoundChanged()
    {
	    
    }

}