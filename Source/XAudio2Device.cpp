/*
 * OneSound - Modern C++17 audio library for Windows OS with XAudio2 API
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\XAudio2Device.h"
#include "OneSound\StreamType\AudioStream.h"
#include <iostream>
#include <mmdeviceapi.h>  
#include <Functiondiscoverykeys_devpkey.h>

using namespace std;

#define LEFT_SPEAKER        SPEAKER_FRONT_LEFT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_SIDE_LEFT
#define RIGHT_SPEAKER       SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_RIGHT

#define SAFE_RELEASE(punk)  \
 if ((punk) != NULL)  \
 { (punk)->Release(); (punk) = NULL; }   


namespace onesnd
{
    class CMMNotificationClient : public IMMNotificationClient
    {
    public:
        IMMDeviceEnumerator* m_pEnumerator;
        CMMNotificationClient() :
            _cRef(1),
            m_pEnumerator(NULL)
        {
            // 初始化COM  
            //::CoInitialize(NULL);
            HRESULT hr = S_OK;

            // 创建接口
            hr = CoCreateInstance(
                __uuidof(MMDeviceEnumerator), NULL,
                CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                (void**)&m_pEnumerator);

            if (hr == S_OK)
            {
	            std::cout << "接口创建成功" << endl;
            }
            else
            {
                cout << "接口创建失败" << endl;
            }

            // 注册事件
            hr = m_pEnumerator->RegisterEndpointNotificationCallback((IMMNotificationClient*)this);
            if (hr == S_OK)
            {
                cout << "注册成功" << endl;
            }
            else
            {
                cout << "注册失败" << endl;
            }
        }

        ~CMMNotificationClient()
        {
            SAFE_RELEASE(m_pEnumerator)
                ::CoUninitialize();
        }

        // IUnknown methods -- AddRef, Release, and QueryInterface   
    private:
        LONG _cRef;

        // Private function to print device-friendly name
        HRESULT _PrintDeviceName(LPCWSTR  pwstrId);

        ULONG STDMETHODCALLTYPE AddRef()
        {
            return InterlockedIncrement(&_cRef);
        }

        ULONG STDMETHODCALLTYPE Release()
        {
            ULONG ulRef = InterlockedDecrement(&_cRef);
            if (0 == ulRef)
            {
                delete this;
            }
            return ulRef;
        }

        HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID riid, VOID** ppvInterface)
        {
            if (IID_IUnknown == riid)
            {
                AddRef();
                *ppvInterface = (IUnknown*)this;
            }
            else if (__uuidof(IMMNotificationClient) == riid)
            {
                AddRef();
                *ppvInterface = (IMMNotificationClient*)this;
            }
            else
            {
                *ppvInterface = NULL;
                return E_NOINTERFACE;
            }
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
            EDataFlow flow, ERole role,
            LPCWSTR pwstrDeviceId)
        {
            cout << "OnDefaultDeviceChanged:"<< endl;
            cout << pwstrDeviceId << endl;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
        {
            cout << "OnDeviceAdded" << endl;
            return S_OK;
        };

        HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
        {
            cout << "OnDeviceRemoved" << endl;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
            LPCWSTR pwstrDeviceId,
            DWORD dwNewState)
        {
            cout << "OnDeviceStateChanged" << endl;
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
            LPCWSTR pwstrDeviceId,
            const PROPERTYKEY key)
        {
            cout << "OnPropertyValueChanged" << endl;
            _PrintDeviceName(pwstrDeviceId);
            return S_OK;
        }
    };

    // Given an endpoint ID string, print the friendly device name.
    HRESULT CMMNotificationClient::_PrintDeviceName(LPCWSTR pwstrId)
    {
        HRESULT hr = S_OK;
        IMMDevice* pDevice = NULL;
        IPropertyStore* pProps = NULL;
        PROPVARIANT varString;

        CoInitialize(NULL);
        PropVariantInit(&varString);

        if (m_pEnumerator == NULL)
        {
            // Get enumerator for audio endpoint devices.
            hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                NULL, CLSCTX_INPROC_SERVER,
                __uuidof(IMMDeviceEnumerator),
                (void**)&m_pEnumerator);
        }
        if (hr == S_OK)
        {
            hr = m_pEnumerator->GetDevice(pwstrId, &pDevice);
        }
        if (hr == S_OK)
        {
            hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        }
        if (hr == S_OK)
        {
            // Get the endpoint device's friendly-name property.
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varString);
        }
        printf("----------------------\nDevice name: \"%S\"\n"
            "  Endpoint ID string: \"%S\"\n",
            (hr == S_OK) ? varString.pwszVal : L"null device",
            (pwstrId != NULL) ? pwstrId : L"null ID");

        PropVariantClear(&varString);

        SAFE_RELEASE(pProps)
            SAFE_RELEASE(pDevice)
            return hr;
    }

    XAudio2Device::XAudio2Device() :
        leftSpeaker(LEFT_SPEAKER),
        rightSpeaker(RIGHT_SPEAKER)
    {
        SetConsoleOutputCP(CP_UTF8);
    }

    void XAudio2Device::initialize()
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        auto flags = long();

    /*#if defined (_WIN32) && defined (_DEBUG)
        flags |= XAUDIO2_DEBUG_ENGINE;
    #else
        flags |= 0;
    #endif*/
        flags |= 0;


        XAudio2Create(&xEngine, flags);
        xEngine->CreateMasteringVoice(&xMaster, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE);
        X3DAudioInitialize(SPEAKER_STEREO, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);

        XAUDIO2_DEVICE_DETAILS dd;
        ZeroMemory(&dd, sizeof(dd));
        xEngine->GetDeviceDetails(0, &dd);
        channelCount = dd.OutputFormat.Format.nChannels;
        mmClient = new CMMNotificationClient();

    }

    void XAudio2Device::finalize()
    {
        xMaster->DestroyVoice();
        xEngine->Release();

        if (xMaster != nullptr)
            xMaster = nullptr;

        if (xEngine != nullptr)
            xEngine = nullptr;
        if(mmClient!=nullptr)
        {
            delete mmClient;
            mmClient = nullptr;
        }
    }

    XABuffer* XABuffer::create(SoundBuffer* ctx, int size, AudioStream* strm, int* pos)
    {
        if (pos) 
            strm->Seek(*pos); // seek to specified pos, let the AudioStream handle error conditions
        if (strm->IsEOS()) 
            return nullptr; // EOS(), failed!

        int bytesToRead = strm->Available();
        if (size < bytesToRead) 
            bytesToRead = size;

        auto* buffer = (XABuffer*)malloc(sizeof(XABuffer) + bytesToRead);
        if (!buffer) 
            return nullptr; // out of memory

        auto* data = (BYTE*)buffer + sizeof(XABuffer); // sound data follows after the XABuffer header

        int bytesRead = strm->ReadSome(data, bytesToRead);
        if (pos) 
            *pos += bytesRead; // update position

        buffer->Flags = strm->IsEOS() ? XAUDIO2_END_OF_STREAM : 0;
        buffer->AudioBytes = bytesRead;
        buffer->pAudioData = data;
        buffer->PlayBegin = 0;		// first sample to play
        buffer->PlayLength = 0;		// number of samples to play
        buffer->LoopBegin = 0;		// first sample to loop
        buffer->LoopLength = 0;		// number of samples to loop
        buffer->LoopCount = 0;		// how many times to loop the region
        buffer->pContext = ctx;		// context of the buffer

        auto sampleSize = strm->SingleSampleSize();
        buffer->nBytesPerSample = sampleSize;

        auto& wf = buffer->wf;
        wf.wFormatTag = WAVE_FORMAT_PCM;
        wf.nChannels = strm->Channels();
        buffer->nPCMSamples = bytesRead / (sampleSize * wf.nChannels);
        wf.nSamplesPerSec = strm->Frequency();
        wf.wBitsPerSample = sampleSize * 8;
        wf.nBlockAlign = (wf.nChannels * sampleSize);
        wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;
        wf.cbSize = sizeof(WAVEFORMATEX);

        // this is enough to create an somewhat unique pseudo-hash:
        buffer->wfHash = wf.nSamplesPerSec + (wf.nChannels * 25) + (wf.wBitsPerSample * 7);
        return buffer;
    }

    void XABuffer::stream(XABuffer* buffer, AudioStream* strm, int* pos)
    {
        if (pos) 
            strm->Seek(*pos); // seek to specified pos, let the AudioStream handle error conditions

        buffer->AudioBytes = strm->ReadSome((void*)buffer->pAudioData, buffer->AudioBytes);

        if (strm->IsEOS()) // end of stream was reached
            buffer->Flags = XAUDIO2_END_OF_STREAM;

        if (pos) 
            *pos += buffer->AudioBytes; // update position
    }

    void XABuffer::destroy(XABuffer*& buffer)
    {
        free(reinterpret_cast<void*>(buffer));
        buffer = nullptr;
    }

    int XABuffer::getBuffersQueued(IXAudio2SourceVoice* source)
    {
        XAUDIO2_VOICE_STATE state;
        source->GetState(&state);

        return state.BuffersQueued;
    }
}