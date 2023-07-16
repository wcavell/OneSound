OneSound
==========
����Ŀ�ǻ���[OneSound](https://github.com/xnhunter/OneSound) ��չ3D��Ч����Ҫ����������Unity3D�ϣ�2D/3D��Ч������ض�������(5.1 7.1��)
Modern library for audio playback supporting 3 common audio formats. The software provides a **high-performance** audio sources, it is written in C++17. The library is generally suited for a game engine or media applications.

����
--------
3Dδ���


Overview
--------

- **Version**: 1.0.6
- **License**: [MIT](https://github.com/wcavell/OneSound/blob/master/LICENSE)
- **Status**: Alpha

Supported Platforms
-------------------
- **Windows** 7
- **Windows** 8/8.1
- **Windows** 10

Used Technology
---------------
- **XAudio2**

Supported Audio File Formats
----------------------
- **WAV** (Waveform Audio File Format) Playing buffers/Streaming
- **MP3** (MPEG-1 Audio Layer 3) Playing buffers/Streaming
- **OGG** (Ogg-Vorbis) Playing buffers/Streaming

Dependencies
---------------
- [**OGG Vorbis**](https://github.com/xiph/vorbis)
- [**MPG123**](https://github.com/georgi/mpg123)

Getting Started
---------------

Play a sound with less code as possible:

```cpp
#include <iostream>
#include <OneSound/OneSound.h>

using namespace std;
using namespace onesnd;

int main()
{ 
    // Create OneSound that initialize XAudio2 Device.
    auto one_sound = make_unique<OneSound>();

    // Create a sound that not loops, plays at once with 75% volume.
    auto sound_1 = make_unique<Sound2D>(make_unique<SoundBuffer>("Sound\\shot.wav"),
                                        false, // Looping
                                        true, // Playing
                                        0.75f); // Volume

    cout << "Press any key to quit: ";
    auto i = char();
    cin >> i;
    
    return 0;
}
```
