using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OneSound;

namespace Managed
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var engine = new OneSound.SoundEngine();
            var listen = new Listener();
            var buff = new SoundBuffer("Sound\\voice.mp3");
            var sound = new Sound3D(buff,false,true,1f);
            sound.SetSourcePosition(new Vector(10,0,10));
            //sound.Speaker = Speaker.FrontLeft;
            listen.AddSound(sound);
            listen.Speaker = Speaker.FrontLeft | Speaker.LowFrequency;
            listen.SetPositionOrientation(new Vector(0,0,0),Vector.Top, Vector.Front);
            listen.SetListenerVelocity(new Vector(0,0,0));
            listen.Update();
            Console.Read();
            sound.Stop();
            sound.Dispose();
            buff.Dispose();
            engine.Dispose();

            Console.ReadLine();
            Console.ReadLine();
            Console.ReadLine();
        }
    }
}
