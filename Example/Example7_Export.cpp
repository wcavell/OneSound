/*
 * Example 6 "DynamicMixing" for OneSound.
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\OneSound.h"

#include <iostream>
#include <thread>
#include <array>

using namespace std;
using namespace onesnd;

int main()
{
    try
    {
        auto one_sound = OneSound_Create();

        // Print library info.
        cout << one_sound->getLibraryName() << ends
            << "v." << one_sound->getLibraryVersionStr() << ends
            << one_sound->getLibraryStatus() << endl;

        // We can initialize it in this way, but look at the OneSound arg.
        one_sound->initialize();

        // The sound buffer is good for short effects like shots, steps or small nature details and so on.
        // Any sound plays in another thread.
        //auto sound_1 = make_unique<Sound2D>(make_unique<SoundBuffer>("Sound\\shot.wav"),false,true,0.3f);
        auto sb = SoundBuffer_Create_ByPath("Sound\\Crysis 1.ogg");
        auto sound = Sound2D_Create_ByBuffer(sb, false, true, 0.8f); 

        //SoundObject_SetOutChannel(sound, 0, SPEAKER_FRONT_CENTER);
        Sound2D_SetChannelMask(sound, SPEAKER_FRONT_RIGHT| SPEAKER_FRONT_CENTER);
        // The sound stream is good for the large effects like ambients, talks, unbreakable things.
        // NOTE: If we'll take a look at WAV formar then we'll see that it's not a good choice for streams.
        //       Many sounds file get decades of MBs for a few minutes.
        //       It would be prefer to use .ogg or .mp3 files in this case.
        /*
        auto sound_2 = make_unique<Sound2D>(make_unique<SoundStream>("Sound\\shot.wav"),
                                            false,
                                            true,
                                            0.2f);
        */

        cout << "Press any key to quit." << endl;
        auto i = char();
        cin >> i;
        OneSound_Destroy(one_sound);
        Sound2D_Destroy(sound);
        SoundBuffer_Destroy(sb);
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}