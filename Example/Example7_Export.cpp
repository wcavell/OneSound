/*
 * Example 6 "DynamicMixing" for OneSound.
 * Copyright ⓒ 2018 Valentyn Bondarenko. All rights reserved.
 * License: https://github.com/weelhelmer/OneSound/master/LICENSE
 */

#include "OneSound\OneExport.h"

#include <iostream>
#include <thread>
#include <array>

using namespace std;
using namespace onesnd;

int main()
{ 
        auto one_sound = OneSound_Create();

        // Print library info.
        cout << one_sound->getLibraryName() << ends
            << "v." << one_sound->getLibraryVersionStr() << ends
            << one_sound->getLibraryStatus() << endl;

        // We can initialize it in this way, but look at the OneSound arg.
        one_sound->initialize();
        //auto instance = X3DAudioCore_GetInstance();
        // The sound buffer is good for short effects like shots, steps or small nature details and so on.
        // Any sound plays in another thread.
        //auto sound_1 = make_unique<Sound2D>(make_unique<SoundBuffer>("Sound\\shot.wav"),false,true,0.3f);
        auto sb = SoundBuffer_Create_ByPath("Sound\\Crysis 1.ogg");
        auto sound = Sound3D_Create_ByBuffer(sb, false, true, 0.8f);
        //sound->setSpeaker(SPEAKER_FRONT_LEFT);
        X3DAUDIO_VECTOR sp{ 10.0F, 0.0F, 5.0F };
        X3DAUDIO_VECTOR zerop{ 0.0F, 0.0F, 0.0F };
        X3DAUDIO_VECTOR top{ 0.0F, 1.0F, 0.0F };
        X3DAUDIO_VECTOR front{ 0.0F, 0.0F, 1.0F };
        sound->setSourcePosition(sp);
        auto listener = Listener_Create();
        listener->setSpeaker(SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT);
        listener->addSound(sound);
        listener->setPositionOrientation(X3DAUDIO_VECTOR(0.0f,0.0f,0.0f), top, front);
        listener->setListenerVelocity(X3DAUDIO_VECTOR{ 0.0f,0.0f,0.0f });
        listener->update();
        
        //SoundObject_SetOutChannel(sound, 0, SPEAKER_FRONT_CENTER);
		/*  Sound2D_SetSpeaker(sound, SPEAKER_FRONT_RIGHT| SPEAKER_FRONT_CENTER);*/
        // The sound stream is good for the large effects like ambients, talks, unbreakable things.
        // NOTE: If we'll take a look at WAV formar then we'll see that it's not a good choice for streams.
        //       Many sounds file get decades of MBs for a few minutes.
        //       It would be prefer to use .ogg or .mp3 files in this case.
        /*
        auto sound_2 = make_unique<Sound2D>(make_unique<SoundStream>("Sound\\shot.wav"),false,true,0.2f);
        */

        bool left = false;
        while (true)
        {
            if (left)
                sp.x += 0.03f;
            else
                sp.x -= 0.03f;

            if (sp.x > 10.0f)
                left = false;
            else if (sp.x < -10.0f)
                left = true;
            sound->setSourcePosition(sp);
            listener->update();
            _sleep(20);
            if(sound->isStopped())
                break;
        }
        cout << "Press any key to quit." << endl;
        auto i = char();
        cin >> i;       
        Sound3D_Destroy(sound);		
		SoundBuffer_Destroy(sb); 
		OneSound_Destroy(one_sound);        

    return 0;
}